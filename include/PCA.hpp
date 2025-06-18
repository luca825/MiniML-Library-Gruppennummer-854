#ifndef PCA_H
#define PCA_H

#include <vector>
#include <utility>  // for std::pair
#include <algorithm>
#include <Eigen/Dense>
#include "DataFrame.hpp"
#include "StandardScaler.hpp"
#include "CastingHelper.hpp"
#include "LinAlgOps.hpp"


class PCA {
private:
    std::vector<std::pair<double, Eigen::VectorXd>> eigenPairs;
    bool centerAndScale = false;
    StandardScaler stdscaler; // For optional normalization of data
    bool hasFitted = false;
    int colsOfFit = 0;

    /**
     * @brief Helper function for fit, which does the actual calculation of the Eigendecomposition
     * @param covMat Symmetryc MatrixXd
     */
    void fitHelper(Eigen::MatrixXd X, bool svd);
    /**
     * @brief Helper function for transform, which does the actual calculation of the projection
     * @param df Dataframe
     * @param dim Int, number of Principal Components to be returned
     * @return A DataFrame with Principal Components
     */
    DataFrame transformHelper(DataFrame& df, int dim);
    /**
     * @brief Print a warning if Input data has a constant column
     * @param df Dataframe
     * @param isCovariance Bool, true if the df is a symmetric (covariance) matrix
     */
    void constantColWarning(DataFrame& df, bool isCovariance);

public:
    PCA();

    /**
     * @brief Fits PCA to the data
     * @param df DataFrame
     * @param centerAndScale Bool, true if the input should be normalized (centered and scaled). Input is ignored, if isCovariance = true
     * @param isCovariance Bool, true if the df is a symmetric (covariance) matrix
     */
    void fit(DataFrame& df, bool centerAndScale = false, bool svd = false, bool isCovariance = false);

    /**
     * @brief Projects the input data
     * @param df Dataframe
     * @param dim Int, number of Principal Components to be returned
     * @return A DataFrame with Principal Components
     */
    DataFrame transform(DataFrame& df, int dim = 2);
    /**
     * @brief Fits PCA to the data and transforms the input data
     * @param df DataFrame
     * @param centerAndScale Bool, true if the input should be normalized (centered and scaled). Input is ignored, if isCovariance = true
     * @param isCovariance Bool, true if the df is a symmetric (covariance) matrix
     * @param dim Int, number of Principal Components to be returned
     * @return A DataFrame with Principal Components
     */
    DataFrame fitTransform(DataFrame& df, bool centerAndScale = false, bool svd = false, bool isCovariance = false, int dim = 2);

    std::vector<double> getEigenvalues();
    std::vector<std::vector<double>> getEigenvectors();
    DataFrame getEigenInformation();
};

#endif // PCA_H
