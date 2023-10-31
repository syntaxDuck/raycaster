# Specify the minimum Cmake version required
cmake_minimum_required(VERSION 3.0)

# Set the project name and version
project(raycaster VERSION 0.1)


# Specify the C compiler
set(CMAKE_C_COMPILER clang)

# Generate a compile comands file for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define source files
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

include_directories(${CMAKE_SOURCE_DIR}/include)

# Add project executable
add_executable(raycaster ${SOURCES})

target_link_libraries(raycaster SDL2)
target_link_libraries(raycaster m)
