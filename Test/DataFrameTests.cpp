#define CATCH_CONFIG_CONSOLE_WIDTH 400
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "DataFrame.hpp"
#include "Column.hpp"



TEST_CASE("DataFrame insert and recall operations", "[DataFrame]") {
    DataFrame df;
    REQUIRE(df.getDim().first == 0);
    REQUIRE(df.getDim().second == 0);

    SECTION("Add one column") {
        std::vector<double> col = {1.0, 2.0, 3.0};
        df.addColumn(col, "col1");
        REQUIRE(df.getDim().second == 1);
        REQUIRE(df.getDim().first == 3);

        SECTION("Check if added Column can be recalled") {
            std::vector<double> col1 = df.getColumn("col1").getDataAsDouble();
            REQUIRE(col == col1);
        }

        SECTION("Check if single value can be recalled") {
            double val = df.get(1, "col1");
            REQUIRE(col[1] == val);
            val = df.get(1, 0);
            REQUIRE(col[1] == val);
        }

        SECTION("Check Col type") {
            REQUIRE(df.getColumnType(0) == ColumnType::Double);
            REQUIRE(df.getColumnType("col1") == ColumnType::Double);
        }
    }

    SECTION("Add one row") {
        std::vector<double> row = {42.0};
        df.addColumn(row, "col1");
        df.addRow(row);
        REQUIRE(df.getDim().first == 2);
    }
}

TEST_CASE("DataFrame dropping operations for rows", "[DataFrame][dropRow]") {
    std::vector<std::pair<std::string, std::vector<double>>> cols = {
        {"A", {1.0, 2.0, 3.0}},
        {"B", {4.0, 5.0, 6.0}}
    };
    std::vector<std::string> rowNames = {"row1", "row2", "row3"};

    DataFrame df(cols, rowNames);

    REQUIRE(df.getDim().first == 3);
    REQUIRE(df.getDim().second == 2);

    SECTION("Drop by index") {
        df.dropRow(1);
        REQUIRE(df.getDim().first == 2);
        std::vector<std::string> names = df.getRowNames();
        REQUIRE(names == std::vector<std::string>{"row1", "row3"});
    }

    SECTION("Drop by name") {
        std::string rowToDrop = "row1";
        df.dropRow(rowToDrop);
        REQUIRE(df.getDim().first == 2);
        std::vector<std::string> names = df.getRowNames();
        REQUIRE(names == std::vector<std::string>{"row2", "row3"});
    }
}

TEST_CASE("Row and Column naming", "[DataFrame][getRowNames]") {
    std::vector<std::pair<std::string, std::vector<double>>> cols = {
        {"Col1", {1.0, 2.0, 3.0}},
        {"Col2", {4.0, 5.0, 6.0}}
    };
    std::vector<std::string> rowNames = {"RowA", "RowB", "RowC"};

    DataFrame df(cols, rowNames);

    std::vector<std::string> expected = {"RowA", "RowB", "RowC"};
    REQUIRE(df.getRowNames() == expected);

    expected = {"Col1", "Col2"};
    REQUIRE(df.getColNames() == expected);
}

TEST_CASE("DataFrame dropping operations for columns", "[DataFrame][dropCol]") {
    std::vector<std::pair<std::string, std::vector<double>>> cols = {
        {"A", {1.0, 2.0, 3.0}},
        {"B", {4.0, 5.0, 6.0}},
        {"C", {7.0, 8.0, 9.0}}
    };
    std::vector<std::string> rowNames = {"row1", "row2", "row3"};

    DataFrame df(cols, rowNames);

    REQUIRE(df.getDim().first == 3);
    REQUIRE(df.getDim().second == 3);

    SECTION("Drop by index") {
        df.dropColumn(1);
        REQUIRE(df.getDim().second == 2);
        std::vector<std::string> names = df.getColNames();
        REQUIRE(names == std::vector<std::string>{"A", "C"});
    }

    SECTION("Drop by name") {
        std::string colToDrop = "B";
        df.dropColumn(colToDrop);
        REQUIRE(df.getDim().second == 2);
        std::vector<std::string> names = df.getColNames();
        REQUIRE(names == std::vector<std::string>{"A", "C"});
    }
}

TEST_CASE("Concatenate: distinct column names", "[DataFrame][concatenate]") {
    DataFrame df1({{"A", {1.0, 2.0}}});
    DataFrame df2({{"B", {3.0, 4.0}}});

    df1.concatenate(df2);

    REQUIRE(df1.getDim() == std::pair<int, int>{2, 2});
    REQUIRE(df1.getColNames() == std::vector<std::string>{"A", "B"});
}


TEST_CASE("Concatenate: duplicate column names, keepFirstOnly = true", "[DataFrame][concatenate]") {
    DataFrame df1({{"X", {10.0, 20.0}}});
    DataFrame df2({{"X", {30.0, 40.0}}});

    df1.concatenate(df2, true);

    REQUIRE(df1.getDim() == std::pair<int, int>{2, 1});
    REQUIRE(df1.getColNames() == std::vector<std::string>{"X"});
    REQUIRE(df1.getColumn("X").getDataAsDouble() == std::vector<double>{10.0, 20.0});
}

TEST_CASE("Concatenate: duplicate column names, keepFirstOnly = false", "[DataFrame][concatenate]") {
    DataFrame df1({{"X", {1.0, 2.0}}});
    DataFrame df2({{"X", {3.0, 4.0}}});

    df1.concatenate(df2, false);

    REQUIRE(df1.getDim() == std::pair<int, int>{2, 2});
    REQUIRE(df1.getColNames() == std::vector<std::string>{"X", "X_2"});
    REQUIRE(df1.getColumn("X_2").getDataAsDouble() == std::vector<double>{3.0, 4.0});
}

TEST_CASE("Concatenate throws when row counts differ", "[DataFrame][concatenate][throws]") {
    DataFrame df1({{"A", {1.0, 2.0}}});
    DataFrame df2({{"B", {3.0, 4.0, 5.0}}});

    REQUIRE_THROWS_AS(df1.concatenate(df2), std::invalid_argument);
}




