
#include "PCA.hpp"


PCA::PCA() = default;

void PCA::fitHelper(Eigen::MatrixXd X, bool svd) {
    
    std::pair<Eigen::VectorXd, Eigen::MatrixXd> eigenDecomp;
    if(svd) {
        eigenDecomp = linAlgOps::singularValueDecomposition(X);
    } else {
        eigenDecomp = linAlgOps::spectralDecomposition(X);
    }

    Eigen::VectorXd eigenvalues = eigenDecomp.first;
    Eigen::MatrixXd eigenvectors = eigenDecomp.second;

    // If an old fit already happened, clear it before adding the newly calculated info
    if (!eigenPairs.empty()) {
        eigenPairs.clear();
    }
    // Save and sort the Eigenvalues (the biggest eigenvalue has the component with the largest variance)
    for (int i = 0; i < eigenvalues.size(); ++i) {
        eigenPairs.emplace_back(eigenvalues(i), eigenvectors.col(i));
    }
    std::sort(eigenPairs.rbegin(), eigenPairs.rend(),
                [](const auto& a, const auto& b) { return a.first < b.first; });
}

DataFrame PCA::transformHelper(DataFrame& df, int dim) {
    Eigen::MatrixXd data = castingHelper::dataFrameToEigenMatrix(df);

    if(dim <= 0) throw std::invalid_argument("Dim has to be greater than 0");

    if(eigenPairs.size() < dim) {
        dim = eigenPairs.size();
        std::cerr << "Warning: Only " + std::to_string(dim) + " Principal Components available" << std::endl;
    }

    Eigen::MatrixXd projectionMatrix(df.getDim().second, dim);
    std::vector<std::string> colNames(dim);
    for (int i = 0; i < dim; ++i) {
        colNames[i] = "PrincipalComponent" + std::to_string(i);
        projectionMatrix.col(i) = eigenPairs[i].second;
    }

    Eigen::MatrixXd reducedData = data * projectionMatrix;

    return(castingHelper::eigenMatrixToDataFrame(reducedData, colNames));
}


void PCA::constantColWarning(DataFrame& df, bool isCovariance) {
    std::vector<std::string> colNames = df.getColNames();

    for(unsigned int i = 0; i < colNames.size(); i++) {
        Column col = df.getColumn(colNames[i]);
        bool constant = descriptiveStatistics::isConstant(col);
        if(constant && this->centerAndScale && !isCovariance) {
            throw std::invalid_argument("DataFrame has constant columns and can thus not be centered and scaled, because of 0 variance of constant Columns. Please remove constant columns");
            break;
        } else if(constant && !isCovariance) {
            std::cerr << "Warning: DataFrame has constant columns. Constant columns do not influence the variance and thus the PCA, consider removing them." << std::endl;
            break;
        }
    }
}


void PCA::fit(DataFrame& df, bool centerAndScale, bool svd, bool isCovariance) {

    if(df.getDim().first == 0 || df.getDim().second < 2) throw std::invalid_argument("DataFrame has to have at least two variables for the PCA to be calculated");

    this->centerAndScale = centerAndScale;
    this->colsOfFit = df.getDim().second;

    constantColWarning(df, isCovariance);

    DataFrame dfScaled;
    if(centerAndScale) {
        stdscaler = StandardScaler();
        dfScaled = stdscaler.fitTransform(df);
    }

    // If the matrix is already the covariance matrix, don't use the SVD
    if(svd && !isCovariance) {
        if(centerAndScale) fitHelper(castingHelper::dataFrameToEigenMatrix(dfScaled), true);
        else fitHelper(castingHelper::dataFrameToEigenMatrix(df), true);
    } else {
        Eigen::MatrixXd covMat;
        if(isCovariance) {
            covMat = castingHelper::getSymEigenMatrix(df);
        } else {
            if(centerAndScale) covMat = descriptiveStatistics::covariancesEigen(dfScaled);
            else covMat = descriptiveStatistics::covariancesEigen(df);
        }
        fitHelper(covMat, false);
    }
    hasFitted = true;
}

DataFrame PCA::transform(DataFrame& df, int dim) {

    if(!hasFitted) throw std::runtime_error("PCA has to be fitted first, before transforming data!");

    if(colsOfFit != df.getDim().second) {
        throw std::runtime_error("Input data needs to have the same number of columns, then the data, which was used in fit!");
    }
    
    if(this->centerAndScale) {
        constantColWarning(df, false);
        // This case is for security, but should never happen, since if centerAndScale == true, the scaler has be fitted
        if(stdscaler.getMeans().empty()) throw std::runtime_error("StandardScaler was not fitted!");
        // Note: The input for transform is scaled with the standardscaler fitted on the training data
        DataFrame dfNew = stdscaler.transform(df);
        return(transformHelper(dfNew, dim));
    } else {
        // The use of the transformHelper here, is that if no scaling is needed, no copy of the dataframe has to be made
        return(transformHelper(df, dim));
    }
}

DataFrame PCA::fitTransform(DataFrame& df, bool centerAndScale, bool svd, bool isCovariance, int dim) {
    fit(df, centerAndScale, svd, isCovariance);
    return(transform(df, dim));
}

std::vector<double> PCA::getEigenvalues() {
    std::vector<double> eigenvalues;
    for(unsigned int i = 0; i < eigenPairs.size(); i++) {
        eigenvalues.push_back(eigenPairs[i].first);
    }
    return(eigenvalues);
}

std::vector<std::vector<double>> PCA::getEigenvectors() {
    std::vector<std::vector<double>> eigenvectors;
    for(unsigned int i = 0; i < eigenPairs.size(); i++) {
        std::vector<double> eigenVec(eigenPairs[i].second.data(), eigenPairs[i].second.data() + eigenPairs[i].second.size());
        eigenvectors.push_back(eigenVec);
    }
    return(eigenvectors);
}

DataFrame PCA::getEigenInformation() {

    std::vector<std::vector<double>> eigenVecs = getEigenvectors();
    DataFrame eigenInformation;
    for(unsigned int i = 0; i < eigenPairs.size(); i++) {
        std::string colname = "EV=" + std::to_string(eigenPairs[i].first);
        eigenInformation.addColumn(eigenVecs[i], colname);
    }

    return(eigenInformation);
}
