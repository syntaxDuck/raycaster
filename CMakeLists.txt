# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.5)

# Set the project name and version
project(raycaster VERSION 0.1 LANGUAGES C)

# Specify the C compiler
set(CMAKE_C_COMPILER clang)

# Generate a compile commands file for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SDL2 package
find_package(SDL2 REQUIRED)

if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found! Please install SDL2.")
endif()

# Define source files
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include ${SDL2_INCLUDE_DIRS})

# Add project executable
add_executable(raycaster ${SOURCES})

# Link libraries
target_link_libraries(raycaster ${SDL2_LIBRARIES} m)
