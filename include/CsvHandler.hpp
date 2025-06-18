#ifndef CSVREADER_H
#define CSVREADER_H


#include <filesystem>
#include <fstream>
#include <sstream>


#include "DataFrame.hpp"

namespace csvHandler {
        /**
         * @brief Reads a Csv file in as DataFrame
         * 
         * Csv is allowed to have headers, but only numeric values. All Columns will have type double
         * 
         * @param filename The name (and path) of the file
         * @param delimiter The delimeter for ending a entry, e.g. "," or ";"
         * @return A DataFrame
         * @throws exception If the file cannot be found/opened or values cannot be parsed into double values
         */
        DataFrame fromCSV(const std::string& filename, bool hasRowNames = false, char delimiter = ',');
        /**
         * @brief Writes out DataFrame to an Csv
         * 
         * Column names will be used as headers. Delimeter will be ","
         * 
         * @param filename The name (and path) of the file
         * @param includeRowNames Boolean, if row names should be included
         * @return A DataFrame
         * @throws exception If the path cannot be found or no writting permitted
         */
        void toCSV(DataFrame& df, const std::string& filename, bool includeRowNames = false, char delimiter = ',');

}


#endif // CSVREADER_H