#include "DataFrame.hpp"
#include "DescriptiveStatistics.hpp"
#include "StandardScaler.hpp"
#include "CsvHandler.hpp"


int main() {


    // --- StandardScaler Beispiel auf dem Iris Dataset ---

    // Lade das Iris Datenset als CSV
    DataFrame df = csvHandler::fromCSV("../Iris.txt");

    df.print();
    std::cout << "\n" << std::endl;

    // StandardScaler fit Beispiel
    // Initialisiere den StandardScaler
    StandardScaler stdScal;
    // Fitte den StandardScaler mit dem Iris Dataset
    stdScal.fit(df);

    std::cout << "Spalten Mittelwerte, welche der StandardScaler berechnet hat" << std::endl;
    std::vector<double> means = stdScal.getMeans();
    for (int i = 0; i < means.size(); ++i) {
        std::cout << means[i];
        if (i < means.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "\n";

    std::cout << "Spalten Standardabweichungen, welche der StandardScaler berechnet hat" << std::endl;
    std::vector<double> stdevs = stdScal.getStdevs();
    for (int i = 0; i < stdevs.size(); ++i) {
        std::cout << stdevs[i];
        if (i < stdevs.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "\n\n";

    // Transformiere den Iris Dataframe durch StandardScaling
    DataFrame dfScaled = stdScal.transform(df);

    std::cout << "Normalisierter Dataframe mit Mittelwert 0 und Standardabweichung 1 in den Spalten:" << std::endl;
    dfScaled.print();
    std::cout << "\n" << std::endl;

    // Alternativ can fit und transform auch in einem Schritt gemacht werden:
    dfScaled = stdScal.fitTransform(df);


    // --- Empirische Statistiken berechnen ---
    // Statistiken können auch durch die Funktionen im Namespace descriptiveStatistics berechnet werden, wie (empirische) Mittelwerte, Varianzen, Standardabweichungen 
    // und die Kovarianzmatrix. Hier ein Beispiel zur Berechnung der Kovarianzmatrix:

    DataFrame irisCov = descriptiveStatistics::covariances(df);
    std::cout << "Kovarianzmatrix des Iris-Datensets" << std::endl;
    irisCov.print();

}
