#
# Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: https://www.opensource.org/licenses/mit-license.php MIT
#

# Detect the operating system and set the Conan profile accordingly
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Set Conan profile for Linux
    set(CONAN_PROFILE_NAME "linux")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # For macOS, check if the processor is Apple Silicon (arm) or Intel (x86)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
        # Set profile for macOS on Apple Silicon (M1/M2)
        set(CONAN_PROFILE_NAME "macos-silicon")
    else()
        # Set profile for macOS on Intel
        set(CONAN_PROFILE_NAME "macos")
    endif()

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # For Windows, print the MSVC version and set the profile for 64-bit architecture
    message(STATUS "MSVC version found ${MSVC_VERSION}")
    set(CONAN_PROFILE_NAME "windows-x86_64")
endif()

# the build tools profile is always release
set(CONAN_BUILD_PROFILE_NAME "${CONAN_PROFILE_NAME}")

# Check if the build type is Debug and modify the profile name to include "debug"
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(CONAN_PROFILE_NAME "${CONAN_PROFILE_NAME}-debug")
endif()

# Set the Conan host and build profile paths using the detected profile name
set(CONAN_HOST_PROFILE "${CMAKE_SOURCE_DIR}/scripts/profiles/${CONAN_PROFILE_NAME}")
set(CONAN_BUILD_PROFILE "${CMAKE_SOURCE_DIR}/scripts/profiles/${CONAN_BUILD_PROFILE_NAME}")

# Macro to create a profile for Conan v2 This macro checks if the default profile for Conan v2 exists and creates it if
# not found. Usage: CREATE_PROFILE()
macro(CREATE_PROFILE)
    if(NOT EXISTS "$ENV{HOMEPATH}/.conan2/profiles/default")
        # Display message indicating the creation of the default profile
        message(STATUS "Creating default profile on $ENV{HOME}/.conan2/profiles/default")
        # Execute the command to detect the default profile
        execute_process(
            COMMAND conan profile detect -f
            RESULT_VARIABLE COMMAND_RESULT
            ERROR_VARIABLE COMMAND_ERROR
            OUTPUT_QUIET)
        if(NOT COMMAND_RESULT EQUAL "0")
            # Display a fatal error message if the default profile creation fails
            message(FATAL_ERROR "Cannot create the default profile\n${COMMAND_ERROR}")
        else()
            # Display message indicating successful installation of Conan dependencies
            message(STATUS "Conan dependencies installed successfully")
        endif()
    endif()
endmacro()

# Macro to add Conan remotes with dynamic name and URL Usage: ADD_REMOTES(artifactory_name artifactory_url)
function(add_remotes artifactory_name artifactory_url)
    # Check if the remote named as content of 'artifactory_name' exists
    execute_process(
        COMMAND conan remote list
        RESULT_VARIABLE CONAN_REMOTE_LIST_RESULT
        OUTPUT_VARIABLE CONAN_REMOTE_LIST_OUTPUT
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    # Search for the remote with the specified name in the output of 'conan remote list'
    string(FIND "${CONAN_REMOTE_LIST_OUTPUT}" "${artifactory_name}" CONAN_REMOTE_LIST_FIND)

    # Check if the remote was not found
    if(${CONAN_REMOTE_LIST_FIND} EQUAL -1)
        # Display a status message indicating the addition of the remote server
        message(STATUS "Adding remote ${artifactory_name} server")
        set(conan_command conan)
        set(add_args
            remote
            add
            --insecure
            --index
            0
            ${artifactory_name}
            ${artifactory_url})
        set(login_args remote login -p $ENV{CONAN_PASSWORD} ${artifactory_name} $ENV{CONAN_USER})
        list(APPEND ADD_COMMAND_LIST ${conan_command} ${add_args})
        list(APPEND LOGIN_COMMAND_LIST ${conan_command} ${login_args})

        # Add the remote to Conan
        execute_process(
            COMMAND ${ADD_COMMAND_LIST}
            RESULT_VARIABLE CONAN_ADD_REMOTE_RESULT
            OUTPUT_VARIABLE CONAN_ADD_REMOTE_OUTPUT
            ERROR_VARIABLE CONAN_ADD_REMOTE_ERROR
            OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE)

        # Check for errors in adding remote
        if(NOT ${CONAN_ADD_REMOTE_RESULT} EQUAL 0)
            message(FATAL_ERROR "Failed to add remote ${artifactory_name}."
                                "Error: ${CONAN_ADD_REMOTE_ERROR} ${CONAN_ADD_REMOTE_OUTPUT}")
        endif()

        # Log in to the added remote
        execute_process(
            COMMAND ${LOGIN_COMMAND_LIST}
            RESULT_VARIABLE CONAN_LOGIN_RESULT
            OUTPUT_VARIABLE CONAN_LOGIN_OUTPUT
            ERROR_VARIABLE CONAN_LOGIN_ERROR
            OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE)

        # Check for errors in logging in
        if(NOT ${CONAN_LOGIN_RESULT} EQUAL 0)
            message(FATAL_ERROR "Failed to log in to remote ${artifactory_name}. Error: ${CONAN_LOGIN_ERROR}")
        endif()

    else()
        # Inform that the remote already exists
        message(STATUS "Remote ${artifactory_name} already exists")
    endif()
endfunction()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CONAN_INSTALL_FLAG "${CMAKE_BINARY_DIR}/conan/build/${CMAKE_BUILD_TYPE}/generators")
else()
    set(CONAN_INSTALL_FLAG "${CMAKE_BINARY_DIR}/conan/build/generators")
endif()

if(NOT EXISTS "${CONAN_INSTALL_FLAG}")
    create_profile()
    add_remotes(${CONAN_REPOSITORY} ${CONAN_URL})
endif()
