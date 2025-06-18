#ifndef STANDARDSCALER_H
#define STANDARDSCALER_H

#include <vector>
#include <string>
#include "DataFrame.hpp"
#include "DescriptiveStatistics.hpp"

class StandardScaler {
private:
    std::vector<double> means;
    std::vector<double> stdev;

public:
    StandardScaler();

    /**
     * @brief Calculates means and standard deviations of the input data
     * @param df DataFrame
     */
    void fit(DataFrame& df);
    /**
     * @brief Normalizes the input data, given the means and standard deviations in fit
     * @param df DataFrame
     * @return DataFrame
     */
    DataFrame transform(DataFrame& df);
    DataFrame fitTransform(DataFrame& df);
    std::vector<double> getMeans();
    std::vector<double> getStdevs();
    bool hasFitted;
};

#endif // STANDARDSCALER_H
