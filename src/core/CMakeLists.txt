#
# Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: http://www.opensource.org/licenses/mit-license.php MIT
#

# Option to enable or disable building Unit Tests (UT) for Core module
option(INVOICE_CORE_WITH_UT "Build UT for Core" ON)

# Option to enable or disable building Unit Tests (UT) for Core module
option(INVOICE_CORE_TOOLS "Build Core tools" ON)

# Targets definition Define the core components and libraries
set(CMAKE_COMPONENT_CORE core) # Core component
set(CMAKE_COMPONENT_CORE_UT core_ut) # Core unit test component
set(INVOICE_CORE_LIBRARY invoice_core) # Core shared library
set(INVOICE_CORE_OBJ_LIBRARY ${INVOICE_CORE_LIBRARY}_obj) # Core object library

# Define the source files for the core object library
set(INVOICE_CORE_SOURCES
    exception.cpp
    db/db_exception.h
    tools/tools.cpp
    tools/tools.h
    settings/settings.cpp
    settings/settings.h
    settings/sql_settings.cpp
    settings/sql_settings.h
    settings/sqlite_settings.cpp
    settings/sqlite_settings.h
    include/dllexports.h
    include/version.h
    db/dynamic_table.cpp
    db/dynamic_table.h
    db/column.cpp
    db/column.h
    db/sqlite/sqlite_column.h
    db/sqlite/sqlite_column.cpp
    db/factory.cpp
    db/factory.h
    db/sqlite/sqlite_builder.cpp
    db/sqlite/sqlite_builder.h
    db/sql_builder.cpp
    db/sql_builder.h
    db/db_manager.cpp
    db/db_manager.h
    db/sqlite/sqlite_db_api.cpp
    db/sqlite/sqlite_db_api.h)

# Create the core object library target
add_library(${INVOICE_CORE_OBJ_LIBRARY} OBJECT ${INVOICE_CORE_SOURCES})

# Include the source directories for the object library
target_include_directories(${INVOICE_CORE_OBJ_LIBRARY} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${INVOICE_CORE_OBJ_LIBRARY} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Define compilation flags for exporting symbols (on Windows)
target_compile_definitions(${INVOICE_CORE_OBJ_LIBRARY} PRIVATE CORE_EXPORTS)

# Find and link Qt6 libraries (Core and SQL components are required)
find_package(Qt6 REQUIRED COMPONENTS Core Sql)
target_link_libraries(${INVOICE_CORE_OBJ_LIBRARY} PUBLIC Qt6::Core Qt6::Sql)

# Definition Handle configuration of version files
set(SHARED_SOURCES "") # Placeholder for shared sources
set(CONFIGURE_SOURCES version.cpp.in) # Configuration input file

# Process the version configuration file
foreach(source_in IN LISTS CONFIGURE_SOURCES)
    string(REGEX REPLACE "\\.[^.]*$" "" source ${source_in}) # Remove file extension
    configure_file(${source_in} ${source} @ONLY) # Generate the file
    list(APPEND SHARED_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/${source}) # Add to shared sources list
endforeach()

# Create the shared library target for the core
add_library(${INVOICE_CORE_LIBRARY} SHARED ${SHARED_SOURCES})

# Link the object library to the shared library
target_link_libraries(${INVOICE_CORE_LIBRARY} PRIVATE ${INVOICE_CORE_OBJ_LIBRARY})

# Define export symbol compilation flags for the shared library
target_compile_definitions(${INVOICE_CORE_LIBRARY} PRIVATE CORE_EXPORTS)

# Include directories for the shared library (for both build and install interfaces)
target_include_directories(
    ${INVOICE_CORE_LIBRARY} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:include>)

# Install Install the include directory (headers) to the destination
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION .
    COMPONENT ${CMAKE_COMPONENT_CORE})

# Install the core library and its components (library, archive, runtime)
install(
    TARGETS ${INVOICE_CORE_LIBRARY}
    EXPORT ${INVOICE_CORE_LIBRARY}Targets
    LIBRARY DESTINATION lib COMPONENT ${CMAKE_COMPONENT_CORE}
    ARCHIVE DESTINATION lib COMPONENT ${CMAKE_COMPONENT_CORE}
    RUNTIME DESTINATION bin COMPONENT ${CMAKE_COMPONENT_CORE}
    INCLUDES
    DESTINATION include)

# Install the target export configuration file (for cmake find_package)
install(
    EXPORT ${INVOICE_CORE_LIBRARY}Targets
    FILE ${INVOICE_CORE_LIBRARY}Targets.cmake
    NAMESPACE invoice::
    DESTINATION lib/cmake/
    COMPONENT ${CMAKE_COMPONENT_CORE})

# Create an alias for easier referencing of the core library
add_library(invoice::${INVOICE_CORE_LIBRARY} ALIAS ${INVOICE_CORE_LIBRARY})

# Add tools build
if(INVOICE_CORE_TOOLS)
    add_subdirectory(tools/db_api_generator)
endif()

# Unit test If unit tests are enabled, include the test configuration and build them
if(INVOICE_CORE_WITH_UT)
    enable_testing()
    include(test_config) # Include the test configuration file
    message(STATUS "Build core-ut") # Output a message indicating UT build
    add_subdirectory(ut) # Add the unit test subdirectory
else()
    message(STATUS "Skip building core-ut") # Output a message indicating UT is skipped
endif()

# Setup doc
if(INVOICE_BUILD_DOC)
    add_input_folder_to_doc(${CMAKE_CURRENT_SOURCE_DIR})
endif()
