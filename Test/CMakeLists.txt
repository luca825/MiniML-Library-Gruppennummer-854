
# Define test executable
add_executable(
    AllTests
    ../src/DataFrame.cpp
    DataFrameTests.cpp
    ../src/DescriptiveStatistics.cpp
    ../src/CsvHandler.cpp
    DescriptiveStatisticsTests.cpp
    ../src/StandardScaler.cpp
    StandardScalerTests.cpp
    ../src/PCA.cpp
    ../src/CastingHelper.cpp
    ../src/LinAlgOps.cpp 
    CastingHelperTests.cpp
    PCATests.cpp
)

# Include headers
target_include_directories(
    AllTests
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(AllTests PRIVATE Eigen3::Eigen Catch2::Catch2)
add_test(
    NAME    AllTests
    COMMAND AllTests
)
set_tests_properties(AllTests PROPERTIES TIMEOUT 100)