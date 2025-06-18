#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <span>

#include "Column.hpp"


class DataFrame {
    private:
        std::unordered_map<std::string, Column> columns;
        std::unordered_map<int, std::string> colIndexToName;
        std::unordered_map<std::string, int> rowNameToIndex;
        std::vector<std::string> columnNames;
        std::vector<std::string> rowNames;

        int nrCols;
        int nrRows;

        /**
         * @brief Checks if any out of a collection of indices is out of range for rows
         * @param indices A collection of indices for rows
         * @return true if the DataFrame has no rows or no columns, false otherwise.
         */
        void checkIndexOutOfRange(const std::span<int>& indices, bool row);
        void checkIndexOutOfRange(int index, bool row);

    public:

        DataFrame();
        DataFrame(std::vector<std::pair<std::string, std::vector<double>>> columns, std::vector<std::string> rowNames = {}, std::vector<ColumnType> types = {});
        DataFrame(std::vector<std::pair<std::string, Column>> columns, std::vector<std::string> rowNames = {});
        DataFrame(std::initializer_list<std::pair<std::string, std::vector<double>>> columns);
        /**
         * @brief Checks if the DataFrame is empty.
         *
         * A DataFrame is considered empty if it has either zero rows or zero columns.
         *
         * @return true if the DataFrame has no rows or no columns, false otherwise.
         */
        bool empty();
        /**
         * @brief Returns a row as double vector, given a numeric row index
         * @param row Index of the row
         * @return double vector of row values
         */
        std::vector<double> getRow(int row);
        /**
         * @brief Returns a row as double vector, given a specific rowname
         * @param row String of the row name
         * @return double vector of row values
         */
        std::vector<double> getRow(std::string row);
        /**
         * @brief Returns a Column, given a numeric column index
         * @param col Index of the column
         * @return Column object
         */
        Column getColumn(int col);
        /**
         * @brief Returns a Column, given a column name
         * @param col String of the column name
         * @return Column object
         */
        Column getColumn(std::string col);
        /**
         * @brief Returns the type in which the column is saved in internally, given a numeric index
         * @param col Index of the column
         * @return ColumnType Enum value
         */
        ColumnType getColumnType(int col);
        /**
         * @brief Returns the type in which the column is saved in internally, given a column name
         * @param col String of the column name
         * @return ColumnType Enum value
         */
        ColumnType getColumnType(std::string col);
        /**
         * @brief Returns a single value from the data as a double value, given two numeric row and column indices
         * @param row Index of row
         * @param col Index of column
         * @return double value
         */
        double get(int row, int col); 
        /**
         * @brief Returns a single value from the data as a double value, given a numeric row index and a column name
         * @param row Index of row
         * @param col String of column name
         * @return double value
         */
        double get(int row, std::string col);
        /**
         * @brief Returns a single value from the data as a double value, given a row and a column name
         * @param row String of row name
         * @param col String of column name
         * @return double value
         */
        double get(std::string row, std::string col);
        /**
         * @brief Returns a single value from the data as a double value, given a row name and a column index
         * @param row String of row name
         * @param col Index of column
         * @return double value
         */
        double get(std::string row, int col);
        /**
         * @brief Returns a subset DataFrame of the internally hold DataFrame, depending on the rows and col indices/names
         * @param rows A span object holding ints (e.g. vector) of row indices
         * @param cols A span object holding strings (e.g. vector) of column names
         * @return A DataFrame
         * @throws std::invalid_argument If the indices/names are invalid
         */
        DataFrame get(std::span<int> rows, std::span<std::string> cols);
        /**
         * @brief Returns a subset DataFrame of the internally hold DataFrame, depending on the rows and col indices/names
         * @param rows A span object holding ints (e.g. vector) of row indices
         * @param cols A span object holding ints (e.g. vector) of column indices
         * @return A DataFrame
         * @throws std::invalid_argument If the indices/names are invalid
         */
        DataFrame get(std::span<int> rows, std::span<int> cols);
        /**
         * @brief Returns a subset DataFrame of the internally hold DataFrame, depending on the rows and col indices/names
         * @param rows A span object holding strings (e.g. vector) of row names
         * @param rows A span object holding ints (e.g. vector) of column indices
         * @return A DataFrame
         * @throws std::invalid_argument If the indices/names are invalid
         */
        DataFrame get(std::span<std::string> rows, std::span<int> cols);
        /**
         * @brief Returns a subset DataFrame of the internally hold DataFrame, depending on the rows and col indices/names
         * @param rows A span object holding strings (e.g. vector) of row names
         * @param rows A span object holding strings (e.g. vector) of column names
         * @return A DataFrame
         * @throws std::invalid_argument If the indices/names are invalid
         */
        DataFrame get(std::span<std::string> rows, std::span<std::string> cols); 
        /**
         * @brief Sets a single value on a specific in the dataframe
         * @param val The value
         * @param row Integer index of the row
         * @param col Column name
         */
        void set(double val, int row, std::string col);
        /**
         * @brief Sets a single value on a specific in the dataframe
         * @param val The value
         * @param row Integer index of the row
         * @param col Integer index of the column
         */
        void set(double val, int row, int col);
        /**
         * @brief Sets a single value on a specific in the dataframe
         * @param val The value
         * @param row Row name
         * @param col Column name
         */
        void set(double val, std::string row, std::string col);
        /**
         * @brief Sets a single value on a specific in the dataframe
         * @param val The value
         * @param row Row name
         * @param col Integer index of the column
         */
        void set(double val, std::string row, int col);
        /**
         * @brief Adds a Column from a double vector
         * 
         *  Adds the Column under the column name and stores it internally under the given data type
         * 
         * @param newCol Double vector holding the column values
         * @param colName String of the column name
         * @param ColumnType Data type in which the column should be stored
         * @return A DataFrame
         * @throws std::invalid_argument If the new row has not the same number of entries as already existing rows, or no column exist yet
         */
        void addColumn(std::vector<double>& newCol, const std::string& colName, ColumnType type = ColumnType::Double);      
        /**
         * @brief Adds a Row from a double vector
         * 
         *  Adds the Row under the row name. If no row name is given, it is choosen as "R" + (number of rows + 1)
         * 
         * @param newRow Double vector holding the row values
         * @param rowName String of the row name
         * @return A DataFrame
         * @throws std::invalid_argument If the new row has not the same number of entries as already existing rows, or no column exist yet
         */
        void addRow(std::vector<double>& newRow, const std::string& rowName = "");
        /**
         * @brief Cocnatenates two DataFrames together
         * 
         *  Concatenates two DataFrames together. If keepFirstOnly is true, only the column of the internal DataFrame is kept, if two columns have the same name
         * 
         * @param df The DataFrame to concatenate to the current DataFrame
         * @param keepFirstOnly Boolean, if true and two columns have the same name, then only the column of the current DataFrame is kept
         * @return A DataFrame
         * @throws std::invalid_argument If both DataFrames don't have the same number of rows
         */
        void concatenate(DataFrame df, bool keepFirstOnly = false);
        /**
         * @brief Drops a column from the DataFrame
         * @param col String of the column name
         * @throws std::invalid_argument If Column does not exist
         */
        void dropColumn(std::string& col);
        /**
         * @brief Drops a column from the DataFrame
         * @param col Index of the column
         * @throws std::invalid_argument If Column does not exist
         */
        void dropColumn(int col);
        
        /**
         * @brief Drops a row from the DataFrame
         * @param row Index of the row
         * @throws std::invalid_argument If row does not exist
         */
        void dropRow(int row);
        /**
         * @brief Drops a row from the DataFrame
         * @param row String of the row name
         * @throws std::invalid_argument If row does not exist
         */
        void dropRow(std::string& row);
        /**
         * @brief Returns vector of column names
         * @return String vector
         */
        std::vector<std::string> getRowNames();
        /**
         * @brief Returns vector of row names
         * @return String vector
         */
        std::vector<std::string> getColNames();
        /**
         * @brief Returns the dimensions of the DataFrame (nr. rows, nr. columns)
         * @return A pair of ints
         */
        std::pair<int, int> getDim();
        /**
         * @brief Sets new rownames
         * @param rowNames String vector of the new row names
         * @throws std::invalid_argument If the number of new row names is not equal to the number of rows in the DataFrame
         */
        void setRowNames(std::vector<std::string> rowNames);
        /**
         * @brief Sets new column names
         * @param columnNames String vector of the new column names
         * @throws std::invalid_argument If the number of new column names is not equal to the number of columns in the DataFrame
         */
        void setColNames(std::vector<std::string> columnNames);
        /**
         * @brief Makes a pretty print of the DataFrame in the console
         */
        void print();

};

#endif // DATAFRAME_H






