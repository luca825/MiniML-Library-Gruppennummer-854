#include "DataFrame.hpp"


int main() {

    // --- Initialisierungsbeispiele: Verschiedene Arten einen Dataframe zu erstellen ---
    
    // Initialisierung über initializer list
    DataFrame df {
        {"A", {1, 2, 3}},
        {"B", {4, 5, 6}},
        {"C", {7, 8, 9}}
    };

    // Initialisierung über vector mit colname, column combination und costum row names
    /**
    std::vector<std::pair<std::string, std::vector<double>>> cols = {
        {"A", {1, 2, 3}},
        {"B", {4, 5, 6}},
        {"C", {7, 8, 9}}
    };
    std::vector<std::string> rowNames = {"row1", "row2", "row3"};

    DataFrame df(cols, rowNames,);
    */

    // Initialisierung über vector mit colname, column combination, sowie costum row names und verschiedene column types
    /** 
    std::vector<std::pair<std::string, std::vector<double>>> cols = {
        {"A", {1, 2, 3}},
        {"B", {4.3, 5.2, 6.1}},
        {"C", {7.1, 8.2, 9.5}}
    };
    std::vector<std::string> rowNames = {"row1", "row2", "row3"};
    std::vector<ColumnType> types = {ColumnType::Int, ColumnType::Float, ColumnType::Double};

    DataFrame df(cols, rowNames, types);
    */

    // Weitere Möglichkeiten einen Dataframe zu initialisieren ist eine default Initialisierung des Dataframes: Dataframe df; um ihn dann mit Spalten zu befüllen oder einen
    // Dataframe als CSV einzulesen (siehe z.B. Beispiel StandardScaler)


    // Print Dataframe in Konsole
    std::cout << "Initialisierter Dataframe:" << std::endl;
    df.print();
    std::cout << "\n" << std::endl;


    // --- Beispiel für Hinzufügen von Rows und Columns ---
    std::vector<double> row = {1.1,1.2,1.3};
    df.addRow(row);
    // df.addRow(row, "NewRow");

    std::vector<double> col = {10,11,12,13};
    df.addColumn(col, "D", ColumnType::Int);
    // Oder default als mit Datatyp double
    // df.addColumn(col, "D");

    // Add row mit costum rowname:
    row = {2.1,2.2,2.3,2.4};
    df.addRow(row, "MyRowName");

    // Fehlerabfang:
    //row = {2.1,2.2,2.3,2.4,2.5};
    //df.addRow(row, "MyRowName");
    // würde z.B. eine Exception werfen, da die row mehr Einträge hat als der Dataframe colums

    // Print Dataframe in Konsole
    std::cout << "Dataframe nach hinzufügen einer Reihe (\"R3\"), einer Spalte (\"D\") und noch einer Reihe (\"MyRowName\"):" << std::endl;
    df.print();
    std::cout << "\n" << std::endl;


    // Print Column um zusätzliche Information, wie z.B. den Datatyp zu bekommen, in welchem die Daten intern gespeichert sind
    std::cout << "Zusätzliche Informationen über eine spezifische Spalte (hier \"D\"):" << std::endl;
    df.getColumn("D").print();
    std::cout << "\n" << std::endl;


    // --- Slicing Beispiele, um Subsets des Dataframes als Dataframes zu bekommen ---
    std::vector<int> row_indices = {0, 1, 4};
    std::vector<int> col_indices = {1, 3};

    // Erhalte einen Dataframe welcher die zweite und vierte Spalte, mit der ersten, zweiten und fünften Reihe 
    DataFrame dfSliced = df.get(row_indices, col_indices);

    std::cout << "Erstelle Subdataframe mit Spalten 2, 4 und Reihen 1, 2, 5: " << std::endl;
    std::cout << "Dimension des Sub-Dataframe: " << dfSliced.getDim().first << " " << dfSliced.getDim().second << std::endl;

    std::cout << "Sub-Dataframe:" << std::endl;
    dfSliced.print();
    std::cout << "\n" << std::endl;


    // --- Concatenate Dataframes Beispiel ---

    DataFrame dfOther {
        {"X", {10, 20, 30}},
        {"Y", {40, 50, 60}},
    };
    
    // Füge dem vorigen Dataframe die zwei neuen Spalten "X" und "Y" des Dataframes dfOther an:
    dfSliced.concatenate(dfOther);
    std::cout << "Print concatenated Dataframes" << std::endl;
    dfSliced.print();
    std::cout << "\n" << std::endl;

    // --- Löschen von Spalten und Reihen ---

    std::cout << "Lösche Spalte \"X\", sowie die Reihe \"R1\" des vorigen Dataframes:" << std::endl;
    std::string colnameToDelete = "X";
    dfSliced.dropColumn(colnameToDelete);
    // oder auch dfSliced.dropColumn(2); mit Integer-Indizierung
    std::string rownameToDelete = "R1";
    dfSliced.dropRow(rownameToDelete);
    // oder auch dfSliced.dropRow(1); mit Integer-Indizierung

    std::cout << "Neuer Dataframe:" << std::endl;
    dfSliced.print();
    std::cout << "\n" << std::endl;


    // --- Änderungen von Spalten und Reihennamen ---
    std::vector<std::string> colNames = dfSliced.getColNames();
    colNames[1] = "NewColname";
    colNames[2] = "AnotherColname";
    dfSliced.setColNames(colNames);

    std::vector<std::string> rowNames = dfSliced.getRowNames();
    rowNames[0] = "NewRowname";
    dfSliced.setRowNames(rowNames);

    std::cout << "Voriger Dataframe, indem die Spaltennamen \"D\" und \"Y\", sowie Reihenname \"R0\" geändert wurden" << std::endl;
    dfSliced.print();
    std::cout << "\n" << std::endl;

}
