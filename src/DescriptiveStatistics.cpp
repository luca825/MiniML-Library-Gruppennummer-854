#include "DescriptiveStatistics.hpp"

namespace descriptiveStatistics {

    Eigen::MatrixXd covariancesEigen(DataFrame& df) {
        if (df.getDim().first == 0 || df.getDim().second < 1) {
            throw std::invalid_argument("Cannot calculate Covariance of fewer than 2 variables.");
        }

        std::vector<std::string> colnames = df.getColNames();
        auto dims = df.getDim();
        int n = dims.first;
        Eigen::MatrixXd covarianceMat(dims.second, dims.second);

        std::vector<double> meansVec = means(df);

        for (unsigned int i = 0; i < colnames.size(); i++) {
            auto column1 = df.getColumn(colnames[i]).getDataAsDouble();
            for (unsigned int j = 0; j <= i; j++) {  // only compute lower triangle
                auto column2 = df.getColumn(colnames[j]).getDataAsDouble();
                double covSum = 0;
                for (int k = 0; k < n; ++k) {
                    covSum += (column1[k] - meansVec[i]) * (column2[k] - meansVec[j]);
                }
                double cov = covSum / (n - 1);
                covarianceMat(i, j) = cov;
                covarianceMat(j, i) = cov;
            }
        }

        return covarianceMat;
    }

    std::vector<double> means(DataFrame& df) {
        if (df.getDim().first == 0 || df.getDim().second == 0) {
            throw std::invalid_argument("Cannot calculate means on empty DataFrame.");
        }

        std::vector<std::string> colnames = df.getColNames();
        std::vector<double> meansVec;
        int n = df.getDim().first;

        for (const auto& colname : colnames) {
            double sum = 0;
            auto column = df.getColumn(colname).getDataAsDouble();
            for (int i = 0; i < n; ++i) sum += column[i];
            meansVec.push_back(sum / n);
        }

        return meansVec;
    }

    std::vector<double> variances(DataFrame& df) {
        if (df.getDim().first == 0 || df.getDim().second == 0) {
            throw std::invalid_argument("Cannot calculate variances on empty DataFrame.");
        }

        std::vector<double> meansVec = means(df);
        std::vector<double> vars;
        int n = df.getDim().first;
        auto colnames = df.getColNames();

        for (size_t i = 0; i < colnames.size(); ++i) {
            double sum = 0;
            auto column = df.getColumn(colnames[i]).getDataAsDouble();
            for (int j = 0; j < n; ++j) {
                sum += std::pow(column[j] - meansVec[i], 2);
            }
            vars.push_back(sum / (n - 1));
        }

        return vars;
    }

    std::vector<double> standardDeviations(DataFrame& df) {
        std::vector<double> vars = variances(df);
        std::vector<double> stddevs;
        for (double v : vars) {
            stddevs.push_back(std::sqrt(v));
        }
        return stddevs;
    }

    DataFrame covariances(DataFrame& df) {
        if (df.getDim().first == 0 || df.getDim().second < 1) {
            throw std::invalid_argument("Cannot calculate covariance for an empty or single-variable DataFrame.");
        }

        Eigen::MatrixXd covMat = covariancesEigen(df);
        DataFrame result;
        auto colnames = df.getColNames();

        for (size_t i = 0; i < colnames.size(); ++i) {
            std::vector<double> colVec(covMat.col(i).data(), covMat.col(i).data() + covMat.col(i).size());
            result.addColumn(colVec, colnames[i]);
        }

        result.setRowNames(colnames);
        return result;
    }

    bool isConstant(Column& col) {
        std::vector<double> column = col.getDataAsDouble();
        for(unsigned int i = 1; i < col.size(); i++) {
            if(column[i-1] != column[i]) return(false);
        }
        return(true);
    }

}  // namespace descriptiveStatistics
