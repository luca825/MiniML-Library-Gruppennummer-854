#ifndef CASTINGHELPER_H
#define CASTINGHELPER_H

#include <vector>
#include <string>
#include <Eigen/Dense>
#include "DataFrame.hpp"


namespace castingHelper {
        /**
         * @brief Casts a DataFrame to Matrix
         * @param df A DataFrame
         * @return A vector of double vectors
         */
        std::vector<std::vector<double>> dataFrameToMatrix(DataFrame& df);
        /**
         * @brief Casts a DataFrame to an MatrixXd
         * @param df A DataFrame
         * @return A MatrixXd
         */
        Eigen::MatrixXd dataFrameToEigenMatrix(DataFrame& df);
        /**
         * @brief Casts a MatrixXd to a DataFrame
         * @param mat A MatrixXd
         * @return A DataFrame
         */
        DataFrame eigenMatrixToDataFrame(Eigen::MatrixXd& mat, std::vector<std::string> colNames = {});
        /**
         * @brief Checks and gets a symmetric MatriXd from a DataFrame
         * @param df A DataFrame
         * @param tol Double, allowed tolerance for values to be determined equal
         * @return A MatrixXd
         * @throws std::invalid_argument If the input DataFrame is not symmetric
         */
        Eigen::MatrixXd getSymEigenMatrix(DataFrame& df, double tol = 1e-8);

}

#endif // CASTINGHELPER_H