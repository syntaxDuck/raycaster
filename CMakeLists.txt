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

# Find SDL2_image package
find_package(SDL2_IMAGE REQUIRED)

if(NOT SDL2_IMAGE_FOUND)
    message(FATAL_ERROR "SDL2_image not found! Please install SDL2_image.")
endif()

# Define source files
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(/opt/homebrew/include/SDL2)
link_directories(/opt/homebrew/lib)

# Add project executable
add_executable(raycaster ${SOURCES})

# Link libraries
target_link_libraries(raycaster ${SDL2_LIBRARIES} /opt/homebrew/Cellar/sdl2_image/2.8.2_2/lib/libSDL2_image.dylib m)
