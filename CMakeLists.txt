# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.19)

# Set the project name and version
project(raycaster VERSION 0.1 LANGUAGES C)

# Generate a compile commands file for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define source files
file(GLOB SOURCES
  ${CMAKE_SOURCE_DIR}/src/*.c
  ${CMAKE_SOURCE_DIR}/src/actors/*.c
  ${CMAKE_SOURCE_DIR}/src/core/*.c
  ${CMAKE_SOURCE_DIR}/src/sim/*.c
  ${CMAKE_SOURCE_DIR}/src/util/*.c
  ${CMAKE_SOURCE_DIR}/src/interface/*.c
  ${CMAKE_SOURCE_DIR}/src/platform/sdl/src/*.c
)

# Include directories
include_directories(
  ${CMAKE_SOURCE_DIR}/include/actors
  ${CMAKE_SOURCE_DIR}/include/core
  ${CMAKE_SOURCE_DIR}/include/interface
  ${CMAKE_SOURCE_DIR}/include/sim
  ${CMAKE_SOURCE_DIR}/include/util
  ${CMAKE_SOURCE_DIR}/src/platform/sdl/include
)

# Add project executable
add_executable(main ${SOURCES})

if(WIN32)
  set(CMAKE_BUILD_TYPE Debug) # Ensure the Debug build type is used
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /DEBUG") # Enable debug symbols for MSVC

  set(SDL2IMAGE_VENDORED OFF)
  add_subdirectory(Packages/SDL EXCLUDE_FROM_ALL)
  add_subdirectory(Packages/SDL_image EXCLUDE_FROM_ALL)

  # SDL2 DLL paths
  set(SDL2_DLL "${CMAKE_BINARY_DIR}/Packages/SDL/Debug/SDL2d.dll")
  set(SDL2_IMAGE_DLL "${CMAKE_BINARY_DIR}/Packages/SDL_image/Debug/SDL2_imaged.dll")

  # Copy SDL2.dll to the build directory
  add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${SDL2_DLL} $<TARGET_FILE_DIR:main>
  )

  # Copy SDL2_image.dll to the build directory
  add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${SDL2_IMAGE_DLL} $<TARGET_FILE_DIR:main>
  )

  # Link libraries
  target_link_libraries(main PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image)
  target_include_directories(main PRIVATE
    SDL2::SDL2 SDL2_image::SDL2_image)

elseif(APPLE)
  # Setup debug
  # set(CMAKE_BUILD_TYPE Debug)
  # set(CMAKE_C_FLAGS_DEBUG "-g")
  # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/lib")
  # find_package(PkgConfig REQUIRED)
  # pkg_check_modules(SDL2 REQUIRED sdl2)
  # pkg_check_modules(SDL2_IMAGE REQUIRED sdl2_image)
  # set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS})
  # set(SDL2_LIBRARIES ${SDL2_LIBRARIES})
  # set(SDL2_IMAGE_LIBRARIES ${SDL2_IMAGE_LIBRARIES})
  # set(SDL2_IMAGE_INCLUDE_DIRS ${SDL2_IMAGE_INCLUDE_DIRS})
endif()
