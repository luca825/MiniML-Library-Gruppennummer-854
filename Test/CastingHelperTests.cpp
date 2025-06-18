#include "catch2/catch.hpp"

#include "CastingHelper.hpp"

TEST_CASE("dataFrameToEigenMatrix converts DataFrame to Eigen::MatrixXd", "[castingHelper][dataFrameToEigenMatrix]") {
    DataFrame df({
        {"col1", {1.0, 2.0, 3.0}},
        {"col2", {4.0, 5.0, 6.0}},
        {"col3", {7.0, 8.0, 9.0}},
    });

    Eigen::MatrixXd expectedMatrix(3, 3);
    expectedMatrix << 1.0, 4.0, 7.0,
                      2.0, 5.0, 8.0,
                      3.0, 6.0, 9.0;

    Eigen::MatrixXd result = castingHelper::dataFrameToEigenMatrix(df);

    REQUIRE(result.isApprox(expectedMatrix, 0.0001));
}

TEST_CASE("eigenMatrixToDataFrame converts Eigen::MatrixXd to DataFrame", "[castingHelper][eigenMatrixToDataFrame]") {
    Eigen::MatrixXd mat(3, 3);
    mat << 1.0, 4.0, 7.0,
           2.0, 5.0, 8.0,
           3.0, 6.0, 9.0;
    std::vector<std::string> colNames = {"col1", "col2", "col3"};

    DataFrame result = castingHelper::eigenMatrixToDataFrame(mat, colNames);

    REQUIRE(result.getDim().first == 3);
    REQUIRE(result.getDim().second == 3);
    REQUIRE(result.getColumn(0).getDataAsDouble() == std::vector<double>({1.0, 2.0, 3.0}));
    REQUIRE(result.getColumn(1).getDataAsDouble() == std::vector<double>({4.0, 5.0, 6.0}));
    REQUIRE(result.getColumn(2).getDataAsDouble() == std::vector<double>({7.0, 8.0, 9.0}));
}

TEST_CASE("dataFrameToMatrix converts DataFrame to std::vector<std::vector<double>>", "[castingHelper][dataFrameToMatrix]") {
    // Setup
    DataFrame df({
        {"col1", {1.0, 2.0, 3.0}},
        {"col2", {4.0, 5.0, 6.0}},
        {"col3", {7.0, 8.0, 9.0}},
    });

    std::vector<std::vector<double>> expectedMatrix = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0}
    };
    std::vector<std::vector<double>> result = castingHelper::dataFrameToMatrix(df);

    REQUIRE(result == expectedMatrix);
}


TEST_CASE("getSymEigenMatrix converts DataFrame to symmetric Eigen matrix", "[castingHelper][getSymEigenMatrix]") {
    DataFrame df({
        {"col1", {1.0, 2.0, 3.0}},
        {"col2", {2.0, 4.0, 5.0}},
        {"col3", {3.0, 5.0, 6.0}},
    });

    Eigen::MatrixXd expectedMatrix(3, 3);
    expectedMatrix << 1.0, 2.0, 3.0,
                      2.0, 4.0, 5.0,
                      3.0, 5.0, 6.0;

    Eigen::MatrixXd result = castingHelper::getSymEigenMatrix(df, 0.0001);

    REQUIRE(result.isApprox(expectedMatrix, 0.0001));
}


TEST_CASE("getSymEigenMatrix throws exception for asymmetric matrix", "[castingHelper][getSymEigenMatrix]") {
    DataFrame df({
        {"col1", {1.0, 2.0, 3.0}},
        {"col2", {2.0, 4.0, 6.0}},
        {"col3", {5.0, 6.0, 7.0}},
    });

    REQUIRE_THROWS_AS(castingHelper::getSymEigenMatrix(df, 0.0001), std::invalid_argument);
}

TEST_CASE("dataFrameToEigenMatrix handles empty DataFrame", "[castingHelper][dataFrameToEigenMatrix]") {
    DataFrame emptyDf;
    Eigen::MatrixXd result = castingHelper::dataFrameToEigenMatrix(emptyDf);

    REQUIRE(result.rows() == 0);
    REQUIRE(result.cols() == 0);
}

TEST_CASE("eigenMatrixToDataFrame handles empty Eigen::MatrixXd", "[castingHelper][eigenMatrixToDataFrame]") {
    Eigen::MatrixXd emptyMat(0, 0);

    DataFrame result = castingHelper::eigenMatrixToDataFrame(emptyMat, {});

    REQUIRE(result.getDim().first == 0);
    REQUIRE(result.getDim().second == 0);
}

TEST_CASE("dataFrameToMatrix handles empty DataFrame", "[castingHelper][dataFrameToMatrix]") {
    DataFrame emptyDf;
    std::vector<std::vector<double>> result = castingHelper::dataFrameToMatrix(emptyDf);
    REQUIRE(result.empty());
}

TEST_CASE("getSymEigenMatrix handles empty DataFrame", "[castingHelper][getSymEigenMatrix]") {
    DataFrame emptyDf;
    REQUIRE_NOTHROW(castingHelper::getSymEigenMatrix(emptyDf)); 
}