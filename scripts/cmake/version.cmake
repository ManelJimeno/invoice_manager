#
# Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: https://www.opensource.org/licenses/mit-license.php MIT
#

# Function to populate the product version from a "version" file This function reads the version information from a file
# named "version" located in the current directory.
function(populate_product_version_from_file)
    # Check if the "version" file exists in the current source directory
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/version")

        # Extract the version string from the file, looking for a pattern like "X.Y.Z"
        file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/version" version_string REGEX "[0-9]+\\.[0-9]+\\.[0-9]+")

        # Split the version string (e.g., "X.Y.Z") into its components: major, minor, and revision
        string(REPLACE "." ";" VERSION_LIST ${version_string})
        list(GET VERSION_LIST 0 local_major)
        list(GET VERSION_LIST 1 local_minor)
        list(GET VERSION_LIST 2 local_revision)

        # Set major, minor, and revision as variables and pass them to the parent scope
        set(PRODUCT_VERSION_MAJOR
            ${local_major}
            PARENT_SCOPE)
        set(PRODUCT_VERSION_MINOR
            ${local_minor}
            PARENT_SCOPE)
        set(PRODUCT_VERSION_REVISION
            ${local_revision}
            PARENT_SCOPE)

        # Set the complete version string as PRODUCT_VERSION and PRODUCT_VERSION_LONG, passing them to the parent scope
        set(PRODUCT_VERSION
            "${version_string}"
            PARENT_SCOPE)
        set(PRODUCT_VERSION_LONG
            "${version_string}"
            PARENT_SCOPE)

        # If the file doesn't exist, show a fatal error
    else()
        message(FATAL_ERROR "It is mandatory to have a version file in the ${CMAKE_CURRENT_SOURCE_DIR} folder")
    endif()
endfunction()

# Function to populate the product version from Git information This function retrieves version information using Git
# commands (if the project is using Git).
function(populate_product_version_from_git)

    # Find the Git executable; if it's not found, quietly fail
    find_package(Git QUIET REQUIRED)

    # Check if the current directory is a Git repository (by checking for a ".git" folder)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")

        # Get the commit hash of the last change made to the "version" file
        execute_process(
            COMMAND "${GIT_EXECUTABLE}" log -n 1 --pretty=format:%H -- version
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            RESULT_VARIABLE COMMAND_RESULT
            OUTPUT_VARIABLE VERSION_GIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

        if(COMMAND_RESULT EQUAL "0")
            # Get the number of commits made since the last "version" commit
            execute_process(
                COMMAND "${GIT_EXECUTABLE}" rev-list ${VERSION_GIT_HASH}..HEAD
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                RESULT_VARIABLE COMMAND_RESULT
                OUTPUT_VARIABLE rev_list_number
                OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

            if(COMMAND_RESULT EQUAL "0")
                # Replace newlines with semicolons and count the number of commits
                string(REGEX REPLACE "\n" ";" rev_list_number "${rev_list_number}")
                list(LENGTH rev_list_number rev_list_number)

                # Get the short hash (7 characters) of the latest commit
                execute_process(
                    COMMAND "${GIT_EXECUTABLE}" rev-parse --short=7 HEAD
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                    RESULT_VARIABLE COMMAND_RESULT
                    OUTPUT_VARIABLE CURRENT_GIT_HASH
                    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

                if(COMMAND_RESULT EQUAL "0")
                    # Append the commit count and the current Git hash to the version (e.g., "1.0.0-10-abcd123")
                    set(PRODUCT_VERSION_LONG
                        "${PRODUCT_VERSION}.${rev_list_number}-${CURRENT_GIT_HASH}"
                        PARENT_SCOPE)
                    set(PRODUCT_REPOSITORY_COMMIT
                        "${CURRENT_GIT_HASH}"
                        PARENT_SCOPE)
                endif()
            endif()
        endif()

        # Get the URL of the Git repository (remote origin)
        execute_process(
            COMMAND "${GIT_EXECUTABLE}" config --get remote.origin.url
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            RESULT_VARIABLE COMMAND_RESULT
            OUTPUT_VARIABLE REPOSITORY_URL
            OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

        # Set the repository URL as a variable and pass it to the parent scope
        set(PRODUCT_REPOSITORY_URL
            ${REPOSITORY_URL}
            PARENT_SCOPE)
    endif()
endfunction()

# If PRODUCT_VERSION is not already defined, populate it from the "version" file and Git info
if(NOT PRODUCT_VERSION)
    populate_product_version_from_file() # Get version from the "version" file
    populate_product_version_from_git() # Get additional version info from Git (if available)
    message(STATUS "The current version of the product is ${PRODUCT_VERSION_LONG}")
endif()
