# 3.13 for target_link_directories.
cmake_minimum_required(VERSION 3.13)

# Define project.
project(
  miniMaya
  VERSION 0.1.0
  DESCRIPTION
    ""
  LANGUAGES CXX)

include_directories(./include)

# Discover cmake modules.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/
                      ${CMAKE_CURRENT_SOURCE_DIR}/cmake/packages)

# Exposes build options (with default values) for this project.
include(Options)

# Set project defaults.
include(Defaults)

# Import the package dependencies (USD, TBB, ...).
include(Packages)

# Include USD plugin building CMake utilities.
include(Utils)

# Recurse into source tree.
add_subdirectory(src)
