
#include "CastingHelper.hpp"

namespace castingHelper {

    Eigen::MatrixXd dataFrameToEigenMatrix(DataFrame& df) {

        std::pair<int, int> dims = df.getDim();
        Eigen::MatrixXd mat(dims.first, dims.second);

        for (unsigned int i = 0; i < dims.second; ++i) {
            std::vector<double> col = df.getColumn(i).getDataAsDouble();
            for (unsigned int j = 0; j < dims.first; ++j) {
                mat(j,i) = col[j];
            }
        }
        return(mat);
    }

    DataFrame eigenMatrixToDataFrame(Eigen::MatrixXd& mat, std::vector<std::string> colNames) {
        std::vector<std::pair<std::string, Column>> colsData;

        int rows = mat.rows();
        int cols = mat.cols();

        if(rows == 0 || cols == 0) {
            DataFrame emptyDf;
            return(emptyDf);
        }

        std::vector<std::string> finalColNames = colNames;

        if (finalColNames.size() != cols) {
            finalColNames.clear();
            for (int j = 0; j < cols; j++) {
                finalColNames.push_back("Col" + std::to_string(j));
            }
        }

        for (int j = 0; j < cols; j++) {
            std::vector<double> colData(rows);
            colsData.emplace_back(finalColNames[j], Column(ColumnType::Double));
            for (int i = 0; i < rows; i++) {
                colsData.at(j).second.addValueFromDouble(mat(i, j));
            }
        }

        return(DataFrame(colsData));
    }

    std::vector<std::vector<double>> dataFrameToMatrix(DataFrame& df) {
        int n = df.getDim().second;
        int m = df.getDim().second;

        std::vector<std::vector<double>> mat(m);

        for(unsigned int i = 0; i < m; i++) {
            mat[i] = df.getColumn(i).getDataAsDouble();
        }

        return(mat);
    }

    Eigen::MatrixXd getSymEigenMatrix(DataFrame& df, double tol) {
        std::vector<std::vector<double>> mat = dataFrameToMatrix(df);

        unsigned int n = mat.size();

        for(unsigned int i = 0; i < n; i++) {
            if(mat[i].size() != n) throw std::invalid_argument("Input is not an nxn matrix");
        }

        Eigen::MatrixXd covMat(n, n);

        for (unsigned int i = 0; i < n; ++i) {
            for (unsigned int j = i + 1; j < n; ++j) {
                if (std::abs(df.get(i,j) - df.get(j,i)) > tol) {
                    throw std::invalid_argument("Input matrix is not symmetric");
                } else {
                    covMat(i,j) = df.get(i,j);
                    covMat(j,i) = df.get(i,j);
                }
            }
        }

        for(unsigned int i = 0; i < n; i++) {
            covMat(i,i) = mat[i][i];
        }

        return covMat;
    }
}
