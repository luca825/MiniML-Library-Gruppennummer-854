
#include "catch2/catch.hpp"

#include "DataFrame.hpp"
#include "Column.hpp"
#include "DescriptiveStatistics.hpp"
#include "CsvHandler.hpp"


TEST_CASE("DescriptiveStatistics calculates correct means, variances, and standard deviations", "[descriptiveStatistics]") {

    DataFrame df({
        {"A", {1.0, 2.0, 3.0}},
        {"B", {4.0, 5.0, 6.0}},
        {"C", {7.0, 8.0, 9.0}}
    });

    SECTION("Mean calculation is correct") {
        std::vector<double> means = descriptiveStatistics::means(df);
        REQUIRE(means.size() == 3);
        REQUIRE(means[0] == Approx(2.0));
        REQUIRE(means[1] == Approx(5.0));
        REQUIRE(means[2] == Approx(8.0));
    }

    SECTION("Variance calculation is correct") {
        std::vector<double> variances = descriptiveStatistics::variances(df);
        REQUIRE(variances.size() == 3);
        REQUIRE(variances[0] == Approx(1.0));
        REQUIRE(variances[1] == Approx(1.0));
        REQUIRE(variances[2] == Approx(1.0));
    }

    SECTION("Standard deviation calculation is correct") {
        std::vector<double> stdevs = descriptiveStatistics::standardDeviations(df);
        REQUIRE(stdevs.size() == 3);
        REQUIRE(stdevs[0] == Approx(1.0));
        REQUIRE(stdevs[1] == Approx(1.0));
        REQUIRE(stdevs[2] == Approx(1.0));
    }

    SECTION("All stats work on constant column") {
        DataFrame constDf({
            {"X", {5.0, 5.0, 5.0}}
        });

        auto means = descriptiveStatistics::means(constDf);
        auto variances = descriptiveStatistics::variances(constDf);
        auto stdevs = descriptiveStatistics::standardDeviations(constDf);

        REQUIRE(means[0] == Approx(5.0));
        REQUIRE(variances[0] == Approx(0.0));
        REQUIRE(stdevs[0] == Approx(0.0));
    }
}

TEST_CASE("DescriptiveStatistics calculates correct means, variances, and standard deviations on Iris", "[descriptiveStatistics]") {

    DataFrame df = csvHandler::fromCSV("../../Data/Iris.txt");

    // Calculated in R
    std::vector<double> meansExpected = {5.843333, 3.057333, 3.758000, 1.199333};
    std::vector<double> variancesExpected = {0.6856935, 0.1899794, 3.1162779, 0.5810063};
    std::vector<double> stdevsExpected = {0.8280661, 0.4358663, 1.7652982, 0.7622377};



    SECTION("Mean calculation is correct") {
        std::vector<double> means = descriptiveStatistics::means(df);
        REQUIRE(means.size() == 4);
        for (size_t i = 0; i < means.size(); ++i) {
            REQUIRE(means[i] == Approx(meansExpected[i]).epsilon(0.0001));
        }
    }

    SECTION("Variance calculation is correct") {
        std::vector<double> variances = descriptiveStatistics::variances(df);
        REQUIRE(variances.size() == 4);
        for (size_t i = 0; i < variances.size(); ++i) {
            REQUIRE(variances[i] == Approx(variancesExpected[i]).epsilon(0.0001));
        }
    }

    SECTION("Standard deviation calculation is correct") {
        std::vector<double> stdevs = descriptiveStatistics::standardDeviations(df);
        REQUIRE(stdevs.size() == 4);
        for (size_t i = 0; i < stdevs.size(); ++i) {
            REQUIRE(stdevs[i] == Approx(stdevsExpected[i]).epsilon(0.0001));
        }
    }

    SECTION("All stats work on constant column") {
        DataFrame constDf({
            {"X", {5.0, 5.0, 5.0}}
        });

        auto means = descriptiveStatistics::means(constDf);
        auto variances = descriptiveStatistics::variances(constDf);
        auto stdevs = descriptiveStatistics::standardDeviations(constDf);

        REQUIRE(means[0] == Approx(5.0));
        REQUIRE(variances[0] == Approx(0.0));
        REQUIRE(stdevs[0] == Approx(0.0));
    }
}


TEST_CASE("Covariance matrix calculation", "[descriptiveStatistics][covariances]") {
    // Covariance of the Iris data set calculated in R
    std::vector<std::pair<std::string, std::vector<double>>> cols = {
        {"sepal.length", {0.685693512304251, -0.0424340044742729, 1.27431543624161, 0.51627069351230}},
        {"sepal.width", {-0.0424340044742729, 0.189979418344519, -0.329656375838926, -0.12163937360179}},
        {"petal.length", {1.27431543624161, -0.329656375838926, 3.11627785234899, 1.29560939597315}},
        {"petal.width", {0.516270693512304, -0.12163937360179, 1.29560939597315, 0.581006263982103}}
    };
    std::vector<std::string> rowNames = {"sepal.length", "sepal.width", "petal.length", "petal.width"};

    DataFrame dfIrisCovR(cols, rowNames);

    DataFrame iris = csvHandler::fromCSV("../../Data/Iris.txt");

    DataFrame dfIrisCov = descriptiveStatistics::covariances(iris);

    for(std::string& var1 : rowNames) {
        for(std::string& var2 : rowNames) {
            REQUIRE(dfIrisCov.get(var1, var2) == Approx(dfIrisCovR.get(var1, var2)).epsilon(0.0001));
        }
    }
}

TEST_CASE("Empty Input", "[descriptiveStatistics]") {

    DataFrame df;
    REQUIRE_THROWS_AS(descriptiveStatistics::means(df), std::invalid_argument);
    REQUIRE_THROWS_AS(descriptiveStatistics::variances(df), std::invalid_argument);
    REQUIRE_THROWS_AS(descriptiveStatistics::standardDeviations(df), std::invalid_argument);
    REQUIRE_THROWS_AS(descriptiveStatistics::covariances(df), std::invalid_argument);
}


