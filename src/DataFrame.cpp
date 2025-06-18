#include "DataFrame.hpp"

DataFrame::DataFrame() : nrCols(0), nrRows(0) {}

// Standard way of creating a DataFrame: Create pairs of columns and column names and additional if wanted add row names
DataFrame::DataFrame(std::vector<std::pair<std::string, Column>> columns, std::vector<std::string> rowNames) {
    if(columns.empty()) throw std::invalid_argument("For Dataframe initialization some data is needed");

    int i = 0;
    for (const auto& [name, values] : columns) {
        if(i == 0) {
            nrRows = values.size();
        } else {
            if (nrRows != values.size()) {
                throw std::invalid_argument("All columns must have the same number of elements.");
            }
        }
        columnNames.push_back(name);
        colIndexToName[i] = name;
        this->columns.emplace(name, values);
        ++i;
    }

    if(!rowNames.empty()) {
        if(nrRows != rowNames.size()) throw std::invalid_argument("Number of rows does not match number of row names");
        this->rowNames = rowNames;
        for (int j = 0; j < nrRows; j++) {
            rowNameToIndex[rowNames[j]] = j;
        }
    } else {
        for (int j = 0; j < nrRows; j++) {
            rowNames.push_back("R" + std::to_string(j));
            rowNameToIndex[rowNames[j]] = j;
        }
        this->rowNames = rowNames;
    }
    nrCols = this->columns.size();
}


// Delegating constructor to the column version per initiliazer list and lambda, because the classic Java way of doing it does not work
DataFrame::DataFrame(std::vector<std::pair<std::string, std::vector<double>>> columns, std::vector<std::string> rowNames, std::vector<ColumnType> types)
    : DataFrame(
        [&]() {
        if(columns.empty()) throw std::invalid_argument("For Dataframe initialization some data is needed");
        bool typesGiven = !types.empty();
        if(typesGiven && columns.size() != types.size()) throw std::invalid_argument("Number of types does not match number of columns");

        std::vector<std::pair<std::string, Column>> colObjects;
        for (unsigned int i = 0; i < columns.size(); ++i) {
            const auto& [name, values] = columns[i];
            Column col(typesGiven ? types[i] : ColumnType::Double);
            col.fillFromDouble(values);
            colObjects.emplace_back(name, col);
        }
        return(colObjects);
        }(),
        rowNames)
    {}

// Delegates to the std::vector<std::pair<std::string, std::vector<double>>> version, which in turn delegates to the column version
DataFrame::DataFrame(std::initializer_list<std::pair<std::string, std::vector<double>>> columns) : DataFrame(
    [&]() {
        std::vector<std::pair<std::string, std::vector<double>>> columnsVec(columns);
        return(columnsVec);
    }())
    {}

void DataFrame::checkIndexOutOfRange(const std::span<int>& indices, bool row) {
    for (int val : indices) {
        int limit = 0;
        if(row) limit = nrRows;
        else limit = nrCols;
        if (val < 0 || val >= limit) {
            if(row) {
                throw std::invalid_argument("Could not find a row with index: " + val);
            } else {
                throw std::invalid_argument("Could not find a column with index: " + val);
            }
        }
    }
}

void DataFrame::checkIndexOutOfRange(int index, bool row) {
    std::array<int, 1> tmp = {index};
    checkIndexOutOfRange(std::span(tmp), row);
}

bool DataFrame::empty() {
    if(nrRows == 0 || nrCols == 0) return(true);
    else return(false);
}

std::vector<double> DataFrame::getRow(int row) {
    if(row < 0 || row >= rowNames.size()) throw std::invalid_argument("Row index: " + std::to_string(row) + " does not exist");
    std::vector<double> rowVec;
    for(const auto& [name, col] : columns) {
        rowVec.push_back(col.getDataAtAsDouble(row));
    }
    return rowVec;
}

std::vector<double> DataFrame::getRow(std::string row) {
    return getRow(rowNameToIndex[row]);
}

Column DataFrame::getColumn(int col) {
    checkIndexOutOfRange(col, false);
    return(columns.at(colIndexToName[col]));

}

Column DataFrame::getColumn(std::string col) {
    if (!(columns.find(col) != columns.end())) throw std::invalid_argument("Could not find a column with name: " + col);
    return columns.at(col);
}

ColumnType DataFrame::getColumnType(int col) {
    checkIndexOutOfRange(col, false);
    return columns.at(colIndexToName[col]).getType();
}

ColumnType DataFrame::getColumnType(std::string col) {
    if (!(columns.find(col) != columns.end())) throw std::invalid_argument("Could not find a column with name: " + col);
    return columns.at(col).getType();
}

double DataFrame::get(int row, int col) {
    checkIndexOutOfRange(col, false);
    checkIndexOutOfRange(row, true);
    return columns.at(colIndexToName[col]).getDataAtAsDouble(row);
}

double DataFrame::get(int row, std::string col) {
    if (!(columns.find(col) != columns.end())) throw std::invalid_argument("Could not find a column with name: " + col);
    checkIndexOutOfRange(row, true);
    return columns.at(col).getDataAtAsDouble(row);
}

double DataFrame::get(std::string row, std::string col) {
    if (!(columns.find(col) != columns.end())) throw std::invalid_argument("Could not find a column with name: " + col);
    return columns.at(col).getDataAtAsDouble(rowNameToIndex[row]);
}

double DataFrame::get(std::string row, int col) {
    checkIndexOutOfRange(col, false);
    return columns.at(colIndexToName[col]).getDataAtAsDouble(rowNameToIndex[row]);
}

DataFrame DataFrame::get(std::span<int> rows, std::span<std::string> cols) {
    checkIndexOutOfRange(rows, true);

    DataFrame dfNew;
    for (std::string col : cols) {
        if (columns.find(col) == columns.end()) throw std::invalid_argument("Could not find a column with name: " + col);
        std::vector<double> new_col(rows.size());
        for(unsigned int i = 0; i < rows.size(); i++) {
            new_col[i] = columns.at(col).getDataAtAsDouble(rows[i]);
        }
        dfNew.addColumn(new_col, col, columns.at(col).getType());
    }
    
    std::vector<std::string> newRowNames(rows.size());
    std::vector<std::string> oldRowNames = getRowNames();

    for(unsigned int i = 0; i < rows.size(); i++) {
        newRowNames[i] = oldRowNames[rows[i]];
    }
    dfNew.setRowNames(newRowNames);

    return(dfNew);
}

DataFrame DataFrame::get(std::span<int> rows, std::span<int> cols) {
    checkIndexOutOfRange(rows, true);
    checkIndexOutOfRange(cols, false);

    std::vector<std::string> newColNames;
    for(int col : cols) {
        newColNames.push_back(colIndexToName[col]);
    }
    return(get(rows, newColNames));
}

DataFrame DataFrame::get(std::span<std::string> rows, std::span<int> cols) {
    checkIndexOutOfRange(cols, false);
    std::vector<std::string> newColnames;
    for(int col : cols) {
        newColnames.push_back(colIndexToName[col]);
    }
    std::vector<int> newRowIndices;
    for(std::string row : rows) {
        newRowIndices.push_back(rowNameToIndex[row]);
    }
    return(get(newRowIndices, newColnames));
}

DataFrame DataFrame::get(std::span<std::string> rows, std::span<std::string> cols) {
    std::vector<int> newRowIndices;
    for(std::string row : rows) {
        newRowIndices.push_back(rowNameToIndex[row]);
    }
    return(get(newRowIndices, cols));
}

void DataFrame::set(double val, int row, std::string col) {
    checkIndexOutOfRange(row, true);
    columns.at(col).setAt(val, row); 
}

void DataFrame::set(double val, int row, int col) {
    checkIndexOutOfRange(row, true);
    checkIndexOutOfRange(col, false);
    set(val, row, colIndexToName[col]);
}

void DataFrame::set(double val, std::string row, int col) {
    checkIndexOutOfRange(col, false);
    set(val, rowNameToIndex[row], colIndexToName[col]);
}

void DataFrame::set(double val, std::string row, std::string col) {
    set(val, rowNameToIndex[row], col);
}

void DataFrame::addColumn(std::vector<double>& newCol, const std::string& colName, ColumnType type) {
    if(nrRows == 0) nrRows = newCol.size();
    else if(newCol.size() != nrRows) throw std::invalid_argument("New column has to have the same number of rows, as the data frame");
    if (std::find(columnNames.begin(), columnNames.end(), colName) != columnNames.end()) {
        std::cerr << "Warning: Column with name " + colName + " already exists. If you want to overwrite it, please use the set function" << std::endl;
        return;
    }
    columns.emplace(colName, Column(type));
    columns.at(colName).fillFromDouble(newCol);
    columnNames.push_back(colName);
    colIndexToName[nrCols] = colName;
    nrCols++;
    if(rowNames.empty()) {
        for(unsigned int i = 0; i < nrRows; i++)
        rowNames.push_back("R" + std::to_string(i));
    }
}

void DataFrame::addRow(std::vector<double>& newRow, const std::string& rowName) {
    if(nrCols == 0) throw std::invalid_argument("A new row can only be added, if columns do already exist");
    if(newRow.size() != nrCols) throw std::invalid_argument("New row has to have the same number of cols, as the data frame");

    for(int i = 0; i < nrCols; i++) {
        columns.at(colIndexToName[i]).addValueFromDouble(newRow[i]);
    }

    if(rowName.empty()) {
        std::string defaultName = "R" + std::to_string(nrRows);
        rowNames.push_back(defaultName);
        rowNameToIndex[defaultName] = nrRows;
    } else {
        rowNames.push_back(rowName);
        rowNameToIndex[rowName] = nrRows;
    }
    nrRows++;
}

void DataFrame::concatenate(DataFrame df, bool keepFirstOnly) {
    if(df.getDim().first != nrRows) throw std::invalid_argument("Both dataframes need to have the same number of rows");

    std::vector<std::string> colnamesOther = df.getColNames();
    std::string newColname;
    for (const auto& colnameOther : colnamesOther) {
        if (!(columns.find(colnameOther) != columns.end())) {
            newColname = colnameOther;
        } else {
            if(!keepFirstOnly) {
                newColname = colnameOther + "_2";
            }
        }
        if(!newColname.empty()) {
            std::vector<double> colData = df.getColumn(colnameOther).getDataAsDouble();
            addColumn(colData, newColname, df.getColumn(colnameOther).getType());
        }
    }
}

void DataFrame::dropColumn(std::string& col) {
    auto it = columns.find(col);
    if (it != columns.end()) {
        columns.erase(it);
    } else {
        throw std::invalid_argument("Column '" + col + "' does not exist.");
    }

    auto name_it = std::find(columnNames.begin(), columnNames.end(), col);
    if (name_it != columnNames.end()) {
        columnNames.erase(name_it);
    }

    colIndexToName.clear();
    for (size_t i = 0; i < columnNames.size(); ++i) {
        colIndexToName[i] = columnNames[i];
    }

    nrCols = static_cast<int>(columnNames.size());
}

void DataFrame::dropColumn(int col) {
    checkIndexOutOfRange(col, false);
    dropColumn(colIndexToName[col]);
}

void DataFrame::dropRow(int row) {
    checkIndexOutOfRange(row, true);
    for (auto& [key, col] : columns) {
        col.deleteAt(row);
    }
    rowNames.erase(rowNames.begin() + row);

    std::string foundKey = "";
    for (const auto& [key, value] : rowNameToIndex) {
        if (value == row) {
            foundKey = key;
            break;
        }
    }
    if(!foundKey.empty()) rowNameToIndex.erase(foundKey);
    nrRows--;
}

void DataFrame::dropRow(std::string& row) {
    dropRow(rowNameToIndex[row]);
}

std::vector<std::string> DataFrame::getRowNames() {
    return rowNames;
}

std::vector<std::string> DataFrame::getColNames() {
    return columnNames;
}

std::pair<int, int> DataFrame::getDim() {
    return std::make_pair(nrRows, nrCols);
}

void DataFrame::setRowNames(std::vector<std::string> rowNames) {
    if(nrRows != rowNames.size()) throw std::invalid_argument("New row names has to have the same number of rows, as the data frame");
    this->rowNames = rowNames;
    rowNameToIndex.clear();
    int i = 0;
    for(std::string rowName : rowNames) {
        rowNameToIndex[rowName] = i;
        ++i;
    }
}

void DataFrame::setColNames(std::vector<std::string> columnNames) {
    if(this->columnNames.size() != columnNames.size()) throw std::invalid_argument("New column names has to have the same number of rows, as the data frame");
    this->columnNames = columnNames;

    std::unordered_map<std::string, Column> newColMapping;
    std::unordered_map<int, std::string> newColIndexToName;

    for(int i = 0; i < columnNames.size(); i++) {
        newColIndexToName[i] = columnNames[i];
        newColMapping.emplace(columnNames[i], Column(columns.at(colIndexToName[i]).getType()));
        newColMapping.at(columnNames[i]).fillFromDouble(columns.at(colIndexToName[i]).getDataAsDouble());
    }
    columns = std::move(newColMapping);
    colIndexToName = std::move(newColIndexToName);
}

void DataFrame::print() {
    std::cout << std::setw(10) << " " << " | ";
    for (const auto& col_name : columnNames) {
        std::cout << std::setw(10) << col_name << " ";
    }
    std::cout << "\n";
    std::cout << std::string(12 + columnNames.size() * 11, '-') << "\n";

    for (int row = 0; row < nrRows; ++row) {
        std::cout << std::setw(10) << rowNames[row] << " | ";
        for (int col = 0; col < static_cast<int>(columnNames.size()); ++col) {
            std::cout << std::setw(10) << columns.at(columnNames[col]).getDataAtAsDouble(row) << " ";
        }
        std::cout << "\n";
    }
}



