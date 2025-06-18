cmake_minimum_required(VERSION 3.10)

project(MiniML)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_library(miniML STATIC
        src/DataFrame.cpp
        src/CsvHandler.cpp
        src/CastingHelper.cpp
        src/DescriptiveStatistics.cpp
        src/LinAlgOps.cpp
        src/StandardScaler.cpp
        src/PCA.cpp
)

target_include_directories(miniML PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(miniML PUBLIC Eigen3::Eigen)

# Kommentiere hier das jeweilige Beispielskript ein, welches ausgeführt werden soll (kann Löschung des cmake cache benötigen, wenn ein anderes Beispiel eingefügt wird)
add_executable(dataframe_app Examples/DataframeOperationsExample.cpp)
# add_executable(dataframe_app Examples/ScalingAndStatisticsExample.cpp)
# add_executable(dataframe_app Examples/PcaExample.cpp)

target_link_libraries(dataframe_app PRIVATE miniML)

# Der restliche Teil linked das catch2 test frame work
include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.10
)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

include(CTest)
enable_testing()

add_subdirectory(Test)