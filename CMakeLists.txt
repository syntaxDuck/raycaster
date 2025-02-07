# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.19)

# Set the project name and version
project(raycaster VERSION 0.1 LANGUAGES C)

# Specify the C compiler
set(CMAKE_C_COMPILER clang)

# Generate a compile commands file for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Setup debug
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/lib")

# Define source files
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED sdl2_image)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add project executable
add_executable(main ${SOURCES})

# Link libraries
# SDL2 is linked with SDL2_IMAGE so we don't need to
# explcitly link SDL@_LIBRARIES
target_link_libraries(main PRIVATE ${SDL2_IMAGE_LIBRARIES})
target_include_directories(main PRIVATE
  ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

