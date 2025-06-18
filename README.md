# Mini-ML-Library

Die **Mini-ML-Library** ist eine kleine Machine-Learning-Library, welche einen Dataframe für numerische Datentypen als grundlegende Datenstruktur anbietet. Zusätzlich bietet sie Funktionalitäten wie die Berechnung der Principal Component Analysis (PCA), Normalisierungen, statistische Kennzahlenberechnung und das Handling von Dataframes zu und von CSV-Dateien.

## Projektstruktur

- `include/`  
  Header-Files der Library

- `src/`  
  C++-Files der Library

- `Test/`  
  Unit Tests für die vorhandenen Klassen und Namespaces

- `Data/`  
  Das Iris-Datenset als Beispieldatensatz

- `Examples/`  
  Drei ausführbare Beispielfiles für die Benutzung des Dataframes, des StandardScalers und der PCA

## Build

Das Projekt kann über CMake gebaut werden. Das Top-Level `CMakeLists.txt` erstellt eine statische Library und kompiliert eine ausführbare Datei für eines der Beispielfiles (siehe ab Zeile 23, wo das gewünschte Beispielfile einkommentiert werden kann). 

Ab Zeile 30 wird das Test-Verzeichnis und das Test-Framework Catch2 eingebunden, was auskommentiert werden kann, falls die Tests nicht gebaut werden sollen.

Das Projekt kann über das Terminal wie folgt gebaut werden:

```bash
cd <PATH_TO_MINI_ML_LIBRARY>
mkdir build
cd build
cmake ..
cmake --build .

```

Um das jeweils einkommentierte Beispielfile laufen zu lassen, kann man falls man im Terminal noch im build Ordner ist

```bash
./dataframe_app

```

benutzen.

Die statische Library für des Projekts befindet sich nach dem build im build Ordner unter dem Namen **libminiML.a**. 

Das Projekt wurde getestet mit gcc 13.2.0.


## Beispielfiles

- **DataframeOperationsExample**  
  Beispiele für die Initialisierung eines Dataframes sowie für einige der Operationen, die der Dataframe anbietet.

- **ScalingAndStatisticsExample**  
  Beispiel für die Benutzung des StandardScalers zur Normalisierung der Daten eines Dataframes. Zusätzlich ein Beispiel zur Berechnung statistischer Kennzahlen wie der Kovarianzmatrix.

- **PcaExample**  
  Beispiel zur Berechnung einer Principal Component Analysis (PCA) mit der Mini-ML-Library.

## Tests

Im Ordner `Test/` befindet sich ein weiteres `CMakeLists.txt`, um die Testfälle ausführbar zu machen.
