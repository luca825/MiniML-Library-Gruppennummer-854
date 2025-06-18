
#include "StandardScaler.hpp"

StandardScaler::StandardScaler() = default;

void StandardScaler::fit(DataFrame& df) {
    means = descriptiveStatistics::means(df);
    stdev = descriptiveStatistics::standardDeviations(df);
    hasFitted = true;
}

DataFrame StandardScaler::transform(DataFrame& df) {

    if(!hasFitted) throw std::runtime_error("StandardScaler has to be fitted first, before transforming data!");

    if(means.size() != df.getDim().second) {
        throw std::runtime_error("Input data needs to have the same number of columns, then the data, which was used in fit!");
    }

    DataFrame scaledDf;
    if(df.empty()) {
        std::cerr << "Warning: Input DataFrame was empty!" << std::endl;
        return(scaledDf);
    }

    int n = df.getDim().first;
    std::vector<std::string> colnames = df.getColNames();    

    int i = 0;
    for(std::string colname : colnames) {
        std::vector<double> scaledColumn(n); 
        std::vector<double> column = df.getColumn(colname).getDataAsDouble();
        if(stdev[i] == 0) {
            scaledColumn = column;
            std::cerr << "[Warning] Cannot scale column: " + colname + ", because the standard deviation is 0 (please check if the column is constant)" << std::endl;
        } else {
            for(int j = 0; j < n; j++) {
                scaledColumn[j] = (column[j] - means[i]) / stdev[i];
            }
        }
        scaledDf.addColumn(scaledColumn, colname);
        i++;
    }

    scaledDf.setRowNames(df.getRowNames());

    return(scaledDf);
}

DataFrame StandardScaler::fitTransform(DataFrame& df) {
    fit(df);
    return(transform(df));
}

std::vector<double> StandardScaler::getMeans() {
    return this->means;
}

std::vector<double> StandardScaler::getStdevs() {
    return this->stdev;
}


