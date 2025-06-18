
#include "catch2/catch.hpp"

#include <cmath>

#include "DataFrame.hpp"
#include "Column.hpp"
#include "DescriptiveStatistics.hpp"
#include "CsvHandler.hpp"
#include "StandardScaler.hpp"
#include "PCA.hpp"
#include "CastingHelper.hpp"
#include "LinAlgOps.hpp"



TEST_CASE("PCA throws on empty DataFrame", "[PCA]") {
    PCA pca;

    SECTION("Empty Dataframe") {
        DataFrame emptyDf;
        REQUIRE_THROWS_AS(pca.fit(emptyDf), std::invalid_argument);
    }

    SECTION("Single Column") {
        DataFrame df {
            {"A", {1, 2, 3, 4, 5, 6}}
        };
        DataFrame emptyDf;
        REQUIRE_THROWS_AS(pca.fit(emptyDf), std::invalid_argument);
    }
}

TEST_CASE("PCA fit and transform simple data", "[PCA]") {
    DataFrame df({
        {"X", {1.0, 2.0, 3.0}},
        {"Y", {1.0, 2.0, 3.0}}
    });

    PCA pca;
    DataFrame result = pca.fitTransform(df);

    REQUIRE(result.getDim().second == 2);

    auto pc1 = result.getColumn(0).getDataAsDouble();
    REQUIRE(pc1[0] < pc1[1]);
    REQUIRE(pc1[1] < pc1[2]);
}


void checkAfterNormalizing(PCA pca, std::vector<double> eigenvaluesExpected, std::vector<std::vector<double>> expectedEigenvectors) {
    std::vector<double> eigenvaluesPCA = pca.getEigenvalues();
    std::vector<std::vector<double>> eigenvectorsPCA = pca.getEigenvectors();

    REQUIRE(expectedEigenvectors.size() == eigenvaluesPCA.size());

    for (int i = 0; i < expectedEigenvectors.size(); ++i) {
        REQUIRE(eigenvaluesPCA[i] == Approx(eigenvaluesExpected[i]).epsilon(0.001));
    }

    // Normalize the Eigenvectors, because their only unique up to a scalar multiple, and Eigen/Dense does not bring the exact same solution
    // as the matrix calculator (or R)
    for(unsigned int i = 0; i < expectedEigenvectors.size(); i++) {
        double normExpected = 0.0;
        double normPCA = 0.0;

        for (unsigned int j = 0; j < expectedEigenvectors[i].size(); j++) {
            normExpected += expectedEigenvectors[i][j] * expectedEigenvectors[i][j];
            normPCA += eigenvectorsPCA[i][j] * eigenvectorsPCA[i][j];
        }
        normExpected = std::sqrt(normExpected);
        normPCA = std::sqrt(normPCA);

        if (normPCA == 0.0 || normExpected == 0.0) {
            throw std::runtime_error("Cannot normalize a zero vector");
        }
    
        for (unsigned int j = 0; j < expectedEigenvectors[i].size(); j++) {
            expectedEigenvectors[i][j] /= normExpected;
            eigenvectorsPCA[i][j] /= normPCA;
        }
    }

    for (int i = 0; i < expectedEigenvectors.size(); ++i) {
        for (int j = 0; j < expectedEigenvectors[0].size(); ++j) {
            // Absolute value because of uniquness up to a scalar includes the sign
            REQUIRE(std::abs(eigenvectorsPCA[i][j]) == Approx(std::abs(expectedEigenvectors[i][j])).epsilon(0.001));
        }
    }

} 

TEST_CASE("PCA fit check eigenvalues and eigenvectors", "[PCA]") {
    // Test on symmetrix (covariance) matrix
    // Eigenvalues and Eigenvectors calculated with https://matrixcalc.org/vectors.html#eigenvectors%28%7B%7B1,2,3%7D,%7B2,1,4%7D,%7B3,4,1%7D%7D%29
    std::vector<double> eigenvaluesExpected = { 7.07467, -0.886791, -3.18788};

    std::vector<std::vector<double>> expectedEigenvectors = {
        {0.797, 0.921, 1},
        {-5.317, 3.516, 1},
        {-0.337, -0.794, 1}
    };

    std::vector<std::pair<std::string, std::vector<double>>> cols = {
        {"Col1", {1.0, 2.0, 3.0}},
        {"Col2", {2.0, 1.0, 4.0}},
        {"Col3", {3.0, 4.0, 1.0}}
    };
    std::vector<std::string> rowNames = {"RowA", "RowB", "RowC"};

    DataFrame df(cols, rowNames);

    PCA pca;

    SECTION("SVD is false") {
        pca.fit(df, false, false, true);
        checkAfterNormalizing(pca, eigenvaluesExpected, expectedEigenvectors);
    }

    SECTION("SVD is true") {
        // In this case it should still be the Eigendecomposition be used, since we have a Covariance-Matrix!
        pca.fit(df, false, true, true);
        checkAfterNormalizing(pca, eigenvaluesExpected, expectedEigenvectors);
    }

}


TEST_CASE("PCA fit check eigenvalues and eigenvectors on Iris, with automatic Scaling", "[PCA]") {
    // Eigenvalues and Eigenvectors calculated with R eigen()
    std::vector<double> eigenvaluesExpected = {2.91849782, 0.91403047, 0.14675688, 0.02071484};

    std::vector<std::vector<double>> expectedEigenvectors = {
        {0.5210659, -0.2693474,  0.58041310,  0.56485654},
        {-0.3774176, -0.9232957, -0.02449161, -0.06694199},
        {0.7195664, -0.2443818, -0.14212637, -0.63427274},
        {0.2612863, -0.1235096, -0.80144925,  0.52359713}
    };

    DataFrame df = csvHandler::fromCSV("../../Data/Iris.txt");

    PCA pca;

    SECTION("SVD is false") {
        pca.fit(df, true, false, false);
        checkAfterNormalizing(pca, eigenvaluesExpected, expectedEigenvectors);
    }

    SECTION("SVD is true") {
        // In this case it should still be the Eigendecomposition be used, since we have a Covariance-Matrix!
        pca.fit(df, true, true, false);
        checkAfterNormalizing(pca, eigenvaluesExpected, expectedEigenvectors);
    }

}

TEST_CASE("PCA transform throw exception when dim is 0", "[PCA]") {

    std::vector<std::pair<std::string, std::vector<double>>> cols = {
        {"Col1", {1.0, 2.0, 3.0}},
        {"Col2", {2.0, 1.0, 4.0}},
        {"Col3", {3.0, 4.0, 1.0}}
    };
    std::vector<std::string> rowNames = {"RowA", "RowB", "RowC"};

    DataFrame df(cols, rowNames);

    PCA pca;
    pca.fit(df, false, false, true);
    REQUIRE_THROWS_AS(pca.transform(df, 0), std::invalid_argument);
}


TEST_CASE("PCA fit throw exception when centerAndScale is true with constant column", "[PCA]") {

    std::vector<std::pair<std::string, std::vector<double>>> cols = {
        {"Col1", {1.0, 1.0, 1.0}},
        {"Col2", {2.0, 1.0, 4.0}},
        {"Col3", {3.0, 4.0, 1.0}}
    };
    std::vector<std::string> rowNames = {"RowA", "RowB", "RowC"};

    DataFrame df(cols, rowNames);

    PCA pca;
    REQUIRE_THROWS_AS(pca.fit(df, true), std::invalid_argument);
}

TEST_CASE("PCA transform", "[PCA]") {

    // Calculated in R, with using eigen() and then multiplying the scaled Iris data two of the Eigenvectors with the greatest eigenvalues
    std::vector<std::pair<std::string, std::vector<double>>> projectedData = {
        {"Col1", {-2.2571412, -2.0740130, -2.3563351, -2.2917068, -2.3818627, -2.068701, -2.43586845, -2.225392, -2.326845, -2.1770349, -2.159077, -2.318364, -2.2110437, -2.6243090,
            -2.191399, -2.254661, -2.200217, -2.1830361, -1.892233, -2.335545, -1.9079312, -2.1996438, -2.7650814, -1.81259716, -2.2197270, -1.9453293, -2.044303, -2.1613365,
            -2.132420, -2.2576980, -2.1329765, -1.8254792, -2.606217, -2.438010, -2.1029299, -2.2004372, -2.0383177, -2.5188934, -2.4215203, -2.1624662, -2.2788408, -1.851918,
            -2.545112, -1.9578886, -2.129924, -2.0628336, -2.376771, -2.3863817, -2.2220026, -2.196475040,  1.098102,  0.7288956,  1.2368358, 0.4061225, 1.0718838, 0.3873895,
            0.7440371, -0.4856956,  0.92480346, 0.01138804, -0.1098283, 0.43922201, 0.5602315, 0.7171593, -0.03324333,  0.8724843, 0.3490822, 0.1582798, 1.221003, 0.1643673,
            0.7352196, 0.4746969, 1.2300573, 0.6307451, 0.70031506,  0.8713545, 1.25231375,  1.3538695, 0.6625807, -0.04012419, 0.1303585, 0.02337438, 0.2407318, 1.0575517,
            0.2232309,  0.4277063, 1.0452264, 1.041044, 0.06935597, 0.2825307, 0.278146,  0.62248441, 0.3354067, -0.3609741, 0.2876227, 0.09105561, 0.2269565, 0.5744638,
            -0.4461723, 0.2558734,  1.8384100, 1.1540156,  2.197904, 1.4353421, 1.8615758, 2.7426851, 0.3657922, 2.294752, 1.9999863, 2.252232, 1.3596206, 1.5973275,
            1.8776105, 1.255908, 1.4627449, 1.5847682, 1.4665185, 2.418228, 3.29964148, 1.259547, 2.0309126, 0.9747153, 2.887977, 1.3287806, 1.695055, 1.947801,
            1.1711801, 1.01754169, 1.7823788, 1.8574250, 2.4278203, 2.297232, 1.8564838, 1.1104277, 1.1984584, 2.7894256, 1.570993, 1.3417970, 0.92173701, 1.8458612,
            2.0080832, 1.8954342, 1.1540156, 2.033745, 1.991475, 1.864258, 1.5593565, 1.5160915, 1.368204, 0.95744849}},
        {"Col2", {-0.4784238, 0.6718827, 0.3407664, 0.5953999, -0.6446757, -1.484205, -0.04748512, -0.222403, 1.111604, 0.4674476, -1.040206, -0.132634, 0.7262432, 0.9582963,
            -1.853847, -2.677315, -1.478656, -0.4872061, -1.400328, -1.124084, -0.4074906, -0.9210359, -0.4568133, -0.08527285, -0.1367962, 0.6235297, -0.241355, -0.5253894,
            -0.312172, 0.3366042, 0.5028561, -0.4222804, -1.787587, -2.143547, 0.4586653, 0.2054192, -0.6593492, -0.5903152, 0.9011611, -0.2679812, -0.4402405, 2.329611,
            0.477501, -0.4707496, -1.138415, 0.7086786, -1.116689, 0.3849572, -0.9946277, -0.009185585, -0.860091, -0.5926294, -0.6142399, 1.7485462, 0.2077251, 0.5913027,
            -0.7704383, 1.8462440, -0.03211848, 1.03056578, 2.6452111, 0.06308385, 1.7588321, 0.1856028, 0.43753742, -0.5073642, 0.1956563, 0.7894510, 1.616827, 1.2982599,
            -0.3952474, 0.4159269, 0.9302094, 0.4149974, 0.06320009, -0.2499560, 0.07699807, -0.3302055, 0.2251735, 1.05518358, 1.5570556, 1.56722524, 0.7746612, 0.6317269,
            0.2868127, -0.8427589, -0.5203087, 1.378371, 0.21877043, 1.3248861, 1.116289, -0.02483981, 0.9851038, 2.0124958, 0.8528731, 0.18058714, 0.3836349, 0.1543565,
            1.5386375, 0.5968523, -0.8675151, 0.6965364, -0.560134, 0.0468307, -0.2940597, -0.7977367, 1.5562892, -0.418663, 0.7090632, -1.914596, -0.6904434, 0.4202924,
            -0.4178498, 1.158380, 0.4407949, -0.6739869, -0.2547683, -2.548125, -0.01772158, 1.701047, -0.9074274, 0.5698553, -0.412260, 0.4802025, -1.010536, -1.004413,
            0.3153381, -0.06413118, 0.1867356, -0.5604133, -0.2584187, -2.617554, 0.1779533, 0.2919446, 0.8086064, -0.8539425, -1.065013, -0.4210202, -0.01716559, -0.6738706,
            -0.6118359, -0.6872731, 0.6965364, -0.864624, -1.045666, -0.385674, 0.8936929, -0.2681707, -1.007878, 0.02425043}}
    };

    DataFrame df = csvHandler::fromCSV("../../Data/Iris.txt");

    PCA pca;

    SECTION("SVD is false") {
        pca.fit(df, true, false);
        DataFrame proj = pca.transform(df);

        REQUIRE(proj.getDim().first == 150);
        REQUIRE(proj.getDim().second == 2);

        for(unsigned int i = 0; i < projectedData.size(); i++) {
            for(unsigned int j = 0; j < projectedData[i].second.size(); j++) {
                REQUIRE(proj.get(j, i) == Approx(projectedData[i].second[j]).epsilon(0.001));
            }
        }
    }

    SECTION("SVD is true") {
        pca.fit(df, true, true);
        DataFrame proj = pca.transform(df);

        REQUIRE(proj.getDim().first == 150);
        REQUIRE(proj.getDim().second == 2);

        for(unsigned int i = 0; i < projectedData.size(); i++) {
            for(unsigned int j = 0; j < projectedData[i].second.size(); j++) {
                // Note: The result of the SVD has the Eigenvectors times (-1) in comparison to the specttralDecomposition, 
                // which is allowed. since Eigenvectors are only unique up to a scale
                REQUIRE(std::abs(proj.get(j, i)) == Approx(std::abs(projectedData[i].second[j])).epsilon(0.001));
            }
        }
    }
}


TEST_CASE("PCA transform without fit", "[PCA]") {
    DataFrame df({
        {"A", {1.0, 2.0, 3.0}},
        {"B", {4.0, 5.0, 6.0}}
    });    
    PCA pca;
    REQUIRE_THROWS_AS(pca.transform(df), std::runtime_error);
}

