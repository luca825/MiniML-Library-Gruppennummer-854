#ifndef DESCRIPTIVESTATISTICS_H
#define DESCRIPTIVESTATISTICS_H

#include <vector>
#include <string>
#include <Eigen/Dense>
#include "DataFrame.hpp"


namespace descriptiveStatistics {
        /**
         * @brief Calculates the means of the columns of a DataFrame
         * @param df A DataFrame
         * @return A double vector
         */
        std::vector<double> means(DataFrame& df);
        /**
         * @brief Calculates the variances of the columns of a DataFrame
         * @param df A DataFrame
         * @return A double vector
         */
        std::vector<double> variances(DataFrame& df);
        /**
         * @brief Calculates the standard deviations of the columns of a DataFrame
         * @param df A DataFrame
         * @return A double vector
         */
        std::vector<double> standardDeviations(DataFrame& df);
        /**
         * @brief Calculates the Covariance matrix as a DataFrame of a DataFrame
         * @param df A DataFrame
         * @return A DataFrame
         */
        DataFrame covariances(DataFrame& df);
        /**
         * @brief Calculates the Covariance matrix as a MatrixXd of a DataFrame
         * @param df A DataFrame
         * @return A DataFrame
         */
        Eigen::MatrixXd covariancesEigen(DataFrame& df);
        /**
         * @brief Checks if a Column is constant
         * @param col A Column object
         * @return true if the values in the column are constant, false otherwise
         */
        bool isConstant(Column& col);


}

#endif // DESCRIPTIVESTATISTICS_H