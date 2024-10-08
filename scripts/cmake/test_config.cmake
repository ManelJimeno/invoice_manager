#
# Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: https://www.opensource.org/licenses/mit-license.php MIT
#

# Include the GoogleTest module and enable testing support in CMake
include(GoogleTest)

# Find and include the GoogleTest (gtest) library, which is required for unit tests
find_package(GTest REQUIRED)

# Include custom target configuration from target_config.cmake
include(target_config)

# Function to create a C++ unit test target Arguments: - name: The name of the test (without "ut_" prefix)
function(add_cpp_test name)
    # Set the test name with a "ut_" prefix
    set(test_name ut_${name})

    # Create an executable target for the unit test using the corresponding .cpp file
    add_executable(${test_name} EXCLUDE_FROM_ALL ${test_name}.cpp)

    # Apply custom target configuration using the target_configure function
    target_configure(${test_name})

    # Link the test executable to the GoogleTest library and any additional arguments (like other libraries)
    target_link_libraries(${test_name} PRIVATE gtest::gtest ${ARGN})

    # Set the test target under the "Tests" folder for better organization
    set_target_properties(${test_name} PROPERTIES FOLDER "Tests")

    # Special handling for Windows platform: - If on Windows, define GTEST_LINKED_AS_SHARED_LIBRARY=0 (to indicate
    # GoogleTest is linked statically) - Elsewhere (other platforms), define GTEST_LINKED_AS_SHARED_LIBRARY=1 (for
    # shared library linkage)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        target_compile_definitions(${test_name} PRIVATE GTEST_LINKED_AS_SHARED_LIBRARY=0)
    else()
        target_compile_definitions(${test_name} PRIVATE GTEST_LINKED_AS_SHARED_LIBRARY=1)
    endif()

    # Automatically discover tests in the executable - DISCOVERY_MODE PRE_TEST: Ensures test discovery happens before
    # running tests - DISCOVERY_TIMEOUT: Sets a timeout for test discovery (600 seconds)
    gtest_discover_tests(${test_name} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin DISCOVERY_MODE PRE_TEST
                                                                                DISCOVERY_TIMEOUT 600)

    # Include the current binary directory for access to generated files (like headers)
    target_include_directories(${test_name} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

    # Print a message indicating that the unit test was added
    message(STATUS "\t unit test ${name} added")
endfunction()
