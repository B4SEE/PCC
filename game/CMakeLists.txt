# CMakeLists.txt

# Set the minimum version of CMake that can be used
cmake_minimum_required(VERSION 3.10)

# Enable testing
enable_testing()

# Add the test to CTest
add_test(NAME ConfigTest COMMAND ConfigTest)

# Set the project name
project(PCCGame)

# Find all source files
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Add the executable
add_executable(PCCGame ${SRC_FILES})

# Include directories
target_include_directories(PCCGame PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add Catch2 using FetchContent
include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v2.13.7 # You can specify the version you need
)
FetchContent_MakeAvailable(Catch2)

# Find all test files
file(GLOB_RECURSE TEST_FILES test/*.cpp)

# Remove the main.cpp file from the test files
list(REMOVE_ITEM SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

# Add the test executable
add_executable(Tests ${TEST_FILES} ${SRC_FILES})

# Include directories
target_include_directories(Tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link the test executable with the necessary libraries
target_link_libraries(Tests PRIVATE Catch2::Catch2)