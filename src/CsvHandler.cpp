#include "CsvHandler.hpp"

namespace csvHandler {

    DataFrame fromCSV(const std::string& filename, bool hasRowNames, char delimiter) {
        if (!std::filesystem::exists(filename)) {
            throw std::runtime_error(filename + " does not exist");
        }
    
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        }
    
        std::string line;
        std::vector<std::string> colNames;
        std::vector<std::pair<std::string, Column>> colData;
        std::vector<std::string> rowNames;
    
        if (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string token;
            bool isFirst = true;
    
            while (std::getline(ss, token, delimiter)) {
                auto start = token.find_first_not_of(" \t\r\n");
                auto end = token.find_last_not_of(" \t\r\n");
                token = (start == std::string::npos) ? "" : token.substr(start, end - start + 1);
    
                if (hasRowNames && isFirst) {
                    isFirst = false;
                    continue;
                }
    
                colNames.push_back(token);
                colData.emplace_back(token, Column(ColumnType::Double));
            }
        }
    
        int rowIdx = 0;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string value;
            int colIdx = 0;
            bool isFirst = true;
            std::string rowName;
    
            while (std::getline(ss, value, delimiter)) {
                if (hasRowNames && isFirst) {
                    rowName = value;
                    isFirst = false;
                    continue;
                }
    
                if (colIdx >= static_cast<int>(colNames.size())) {
                    throw std::runtime_error("Too many values in row " + std::to_string(rowIdx));
                }
    
                std::stringstream val_stream(value);
                double val;
                val_stream >> val;
    
                if (val_stream.fail()) {
                    throw std::runtime_error("Failed to convert value: '" + value + "' at row " + std::to_string(rowIdx));
                }
    
                colData.at(colIdx).second.addValueFromDouble(val);
                ++colIdx;
            }
    
            if (colIdx != static_cast<int>(colNames.size())) {
                throw std::runtime_error("Too few values in row " + std::to_string(rowIdx));
            }
    
            if (hasRowNames) {
                rowNames.push_back(rowName);
            } else {
                rowNames.push_back("R" + std::to_string(rowIdx));
            }
    
            ++rowIdx;
        }
    
        file.close();
    
        return DataFrame(colData, rowNames);
    }
    

    void toCSV(DataFrame& df, const std::string& filename, bool includeRowNames, char delimiter) {
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            throw std::runtime_error("Unable to open file: " + filename);
        }

        auto colNames = df.getColNames();
        auto rowNames = df.getRowNames();
        int numRows = df.getDim().first;
        int numCols = df.getDim().second;

        if (includeRowNames) outFile << "RowNames,";
        for (unsigned int i = 0; i < colNames.size(); ++i) {
            outFile << colNames[i];
            if (i != colNames.size() - 1) outFile << delimiter;
        }
        outFile << "\n";

        for (int row = 0; row < numRows; ++row) {
            if (includeRowNames) {
                outFile << (row < rowNames.size() ? rowNames[row] : "") << delimiter;
            }
            for (int col = 0; col < numCols; ++col) {
                outFile << df.getColumn(col).getDataAtAsDouble(row);
                if (col != numCols - 1) outFile << delimiter;
            }
            outFile << "\n";
        }

        outFile.close();
    }

}