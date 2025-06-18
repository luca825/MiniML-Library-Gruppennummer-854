#ifndef LinAlgOps_H
#define LinAlgOps_H

#include <vector>
#include <Eigen/Dense>
#include "DataFrame.hpp"


namespace linAlgOps {
        /**
         * @brief Calculates the SVD
         * @param df A DataFrame
         * @return A pair of the eigenvalues as vector and the matrix with the eigenvectors
         */
        std::pair<Eigen::VectorXd, Eigen::MatrixXd> singularValueDecomposition(Eigen::MatrixXd& X);        
        /**
         * @brief Calculates the Spectraldecomposition
         * @param df A DataFrame
         * @return A pair of the eigenvalues as vector and the matrix with the eigenvectors
         */
        std::pair<Eigen::VectorXd, Eigen::MatrixXd> spectralDecomposition(Eigen::MatrixXd& X);
}

#endif // LINALGOPS_H