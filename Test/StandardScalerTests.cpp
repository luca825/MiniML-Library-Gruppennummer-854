
#include "catch2/catch.hpp"

#include "DataFrame.hpp"
#include "Column.hpp"
#include "DescriptiveStatistics.hpp"
#include "StandardScaler.hpp"


TEST_CASE("StandardScaler fit, transform and fitTransform", "[StandardScaler]") {
    DataFrame df({
        {"A", {1.0, 2.0, 3.0}},
        {"B", {4.0, 5.0, 6.0}}
    });

    SECTION("Fit computes correct means and std devs") {
        StandardScaler scaler;
        scaler.fit(df);

        // Expected mean and std dev
        std::vector<double> expected_means = {2.0, 5.0};
        std::vector<double> expected_stdev = {1.0, 1.0};

        REQUIRE(scaler.getMeans() == expected_means);
        REQUIRE(scaler.getStdevs() == expected_stdev);
    }

    SECTION("Transform scales the DataFrame correctly") {
        StandardScaler scaler;
        scaler.fit(df);
        DataFrame scaled = scaler.transform(df);

        REQUIRE(scaled.getDim() == std::make_pair(3, 2));
        std::vector<double> expectedA = {-1.0, 0.0, 1.0};
        std::vector<double> expectedB = {-1.0, 0.0, 1.0};

        for (int i = 0; i < 3; ++i) {
            REQUIRE(scaled.get(i, 0) == Approx(expectedA[i]));
            REQUIRE(scaled.get(i, 1) == Approx(expectedB[i]));
        }
    }

    SECTION("fitTransform gives same result as fit + transform") {
        StandardScaler scaler1, scaler2;
        scaler1.fit(df);
        DataFrame transformed1 = scaler1.transform(df);

        DataFrame transformed2 = scaler2.fitTransform(df);

        REQUIRE(transformed2.getDim() == transformed1.getDim());

        for (int i = 0; i < 3; ++i) {
            REQUIRE(transformed2.get(i, 0) == Approx(transformed1.get(i, 0)));
            REQUIRE(transformed2.get(i, 1) == Approx(transformed1.get(i, 1)));
        }
    }

}


TEST_CASE("StandardScaler on empty dataframe", "[StandardScaler]") {
    DataFrame empty_df;
    StandardScaler scaler;
    REQUIRE_THROWS_AS(scaler.fitTransform(empty_df), std::invalid_argument);
}

TEST_CASE("StandardScaler transform without fit", "[StandardScaler]") {
    DataFrame df({
        {"A", {1.0, 2.0, 3.0}},
        {"B", {4.0, 5.0, 6.0}}
    });    
    StandardScaler scaler;
    REQUIRE_THROWS_AS(scaler.transform(df), std::runtime_error);
}
