#
# Main cmake file.
#
# Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: http://www.opensource.org/licenses/mit-license.php MIT
#

# Set minimum CMake version
cmake_minimum_required(VERSION 3.28)

# Append custom CMake modules path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/scripts/cmake")

# Define configuration options
option(INVOICE_BUILD_DOC "Build documentation" OFF)
option(INVOICE_BUILD_INSTALLER "Build the installer for the current platform" OFF)

# Load version routines
include(version)

# Load pre-commit configuration
include(pre_commit_config)

# Load functions are used to generate table classes from JSON files
include(tables_config)

# Specify Conan routines for dependency management
set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES "${CMAKE_SOURCE_DIR}/scripts/cmake/conan_provider.cmake")

# Define the project
project(
    invoice_manager
    VERSION "${PRODUCT_VERSION}"
    DESCRIPTION "Invoice Manager"
    LANGUAGES C CXX)

# Configure output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>)

# Enforce C++ standard version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Load Conan configuration
include(conan_config)

# Load Qt configuration
include(qt_config)

# Configure installer generation
if(INVOICE_BUILD_INSTALLER)
    set(CMAKE_TOOLS_HOME "${CMAKE_SOURCE_DIR}/resources")
    set(CONFIG_AUTHOR_PROJECT "manel.jimeno@gmail.com")
    include(installer_config)
elseif(INVOICE_BUILD_DOC)
    set(INSTALL_DOCDIR "doc")
endif()

# Configure documentation generation with Doxygen
if(INVOICE_BUILD_DOC)
    add_subdirectory(doc)
endif()

# Add source subdirectory
add_subdirectory(src)

# Print configuration summary
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
