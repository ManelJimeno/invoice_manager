#
# Main cmake file.
#
# Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: http://www.opensource.org/licenses/mit-license.php MIT
#

cmake_minimum_required(VERSION 3.28)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/scripts/cmake")

# --- Load version routines
include(version)

# --- Load tools
include(pre_commit_config)

# ---- Load Conan routines
set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES "${CMAKE_SOURCE_DIR}/scripts/cmake/conan_provider.cmake")

# ---- Project definition
project(
    invoice_manager
    VERSION "${PRODUCT_VERSION}"
    DESCRIPTION "Invoice Manager"
    LANGUAGES C CXX)

# ---- Setting output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>)

# ---- Set CXX version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Load Conan config
include(conan_config)

# ---- Add subprojects
add_subdirectory(src)

# ---- Summary
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
