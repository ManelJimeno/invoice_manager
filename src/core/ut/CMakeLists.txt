#
# Configure project and target default parameters.
#
# Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: https://www.opensource.org/licenses/mit-license.php MIT
#

# Function to add a core test Arguments: - name: The name of the test - ARGN: Additional arguments (like libraries to
# link)
function(add_core_test name)
    # Set the test name by prepending "ut_" to the provided name
    set(test_name ut_${name})
    # Add the C++ test target using the provided name and arguments
    add_cpp_test(${name} ${ARGN})
endfunction()

# Define the libraries to be linked with the core tests
set(TEST_LIBRARIES ${INVOICE_CORE_LIBRARY} Qt6::Core Qt6::Sql)

# Add the unit tests, specifying their names and the libraries to link wit
set(_unit_tests core_package core_db core_sqlite_configure core_db_api_generator)
set(_unit_test_dependencies)

foreach(unit_test ${_unit_tests})
    add_core_test(${unit_test} ${TEST_LIBRARIES})
    list(APPEND unit_test_dependencies "ut_${unit_test}")
endforeach()

# Add additional settings
target_link_libraries(ut_core_db_api_generator PUBLIC ${DB_API_GENERATOR_OBJ_LIBRARY})

# Native path to data
set(ORIGINAL_PATH "${CMAKE_SOURCE_DIR}/src/core/ut/data")
file(TO_NATIVE_PATH "${ORIGINAL_PATH}" NATIVE_PATH)

# Define custom target to run all core tests directly, without using ctest
add_custom_target(
    run_core_tests
    COMMAND ${CMAKE_COMMAND} -E echo "Running all core tests..."
    COMMAND ${CMAKE_COMMAND} -E echo "--------------------------"
    # Execute each test binary one by one
    COMMAND $<TARGET_FILE:ut_core_package>
    COMMAND $<TARGET_FILE:ut_core_db>
    COMMAND $<TARGET_FILE:ut_core_sqlite_configure>
    COMMAND $<TARGET_FILE:ut_core_db_api_generator> --source-folder ${NATIVE_PATH}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Executing all core unit tests"
    DEPENDS ${_unit_test_dependencies})
