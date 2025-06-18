#include "DataFrame.hpp"
#include "DescriptiveStatistics.hpp"
#include "PCA.hpp"
#include "CsvHandler.hpp"
#include "CastingHelper.hpp"


int main() {

    /// --- PCA-Berechnung Beispiel auf dem Iris Dataset ---

    // Lade das Iris Datenset als CSV
    DataFrame df = csvHandler::fromCSV("../Data/Iris.txt");

    df.print();
    std::cout << "\n" << std::endl;

    // PCA fit Beispiel
    // Initialisiere die PCA
    PCA pca;
    // Fitte die PCA an den Iris-Daten
    pca.fit(df);
    
    // Alternativ können die Daten auch Normalisiert werden, bevor der Berechnung der Kovarianzmatrix:
    // pca.fit(df, true);
    // Oder es kann eine schon berechnete Kovarianzmatrix benutzt werden
    // DataFrame dfCov = descriptiveStatistics::covariances(df);
    // pca.fit(dfCov, false, false, true);
    // Die PCA kann statt mit der Spektralzerlegung auch mit der SVD berechnet werden, für die Originalen Daten df
    // pca.fit(dfCov, true, true);


    // Bekomme und printe die Informationen der Eigenzerlegung
    DataFrame eigenInformation = pca.getEigenInformation();
    // Die Spalten sind die Eigenvektoren, die Spaltennamen zeigen die Eigenwerte an. Die Spalten des Dataframes sind nach Varianz (Eigenwerten) sortiert
    std::cout << "Eigenvektoren der Kovarianzmatrix des Iris Datensets (mit Eigenwerten in den Spaltennamen)" << std::endl;
    eigenInformation.print();


    // Transformiere den Iris Dataframe durch StandardScaling
    DataFrame dfProjected = pca.transform(df);
    // Alternativ um mehere Dimensionen zu benutzen, benutze das dim Arugment
    // DataFrame dfProjected = pca.transform(df, dim = 3);

    std::cout << "Projiezierte Daten der beiden Dimensionen mit der größten Varianz" << std::endl;
    dfProjected.print();
    std::cout << "\n" << std::endl;

    // Alternativ can fit und transform auch in einem Schritt gemacht werden:
    dfProjected = pca.fitTransform(df);

    dfProjected.print();

    // Write the projection out to a CSV
    csvHandler::toCSV(dfProjected, "../Data/IrisProjection.txt");

    // it is also possible to write the file out with rownames:
    // csvHandler::toCSV(dfProjected, "../Data/IrisProjection.txt", true);
    // and with different delimeters:
    // csvHandler::toCSV(dfProjected, "../Data/IrisProjection.txt", true, ";");


}
