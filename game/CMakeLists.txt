cmake_minimum_required(VERSION 3.10)

# Set the project name
project(PCCGame)

# Find all source files
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Add the executable
add_executable(PCCGame ${SRC_FILES})

# Include directories
target_include_directories(PCCGame PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)