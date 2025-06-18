#include "LinAlgOps.hpp"

namespace linAlgOps {

    std::pair<Eigen::VectorXd, Eigen::MatrixXd> singularValueDecomposition(Eigen::MatrixXd& X) {
        Eigen::JacobiSVD<Eigen::MatrixXd> svd(X, Eigen::ComputeThinU | Eigen::ComputeThinV);

        Eigen::VectorXd eigenvalues = svd.singularValues();
        Eigen::MatrixXd eigenvectors = svd.matrixV();

        int n = X.rows();
        for (int i = 0; i < eigenvalues.size(); ++i) {
            eigenvalues(i) = (eigenvalues(i) * eigenvalues(i)) / (n - 1);
        }

        return(std::make_pair(eigenvalues, eigenvectors));
    }
    
    std::pair<Eigen::VectorXd, Eigen::MatrixXd> spectralDecomposition(Eigen::MatrixXd& X) {
        // Calculate the Eigendecomposition on a symmetric matrix
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eig(X);


        if (eig.info() != Eigen::Success) {
            throw std::runtime_error("Eigenvalue decomposition failed.");
        }

        Eigen::VectorXd eigenvalues = eig.eigenvalues();
        Eigen::MatrixXd eigenvectors = eig.eigenvectors();

        return(std::make_pair(eigenvalues, eigenvectors));
    }

}  // namespace linAlgOps
