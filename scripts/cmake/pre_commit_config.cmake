#
# Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: https://www.opensource.org/licenses/mit-license.php MIT
#

# Define options for configuring pre-commit and Python pip installation
option(PRE_COMMIT_TOOL "Enable or disable pre-commit tool installation" ON)
option(PYTHON_INSTALL_ON_USER "Enable --user option for pip install if Python >= 3.12" ON)

# Function to configure and install the pre-commit tool
function(configure_pre_commit)
    # Check if the pre-commit hook is already installed (inside .git/hooks directory)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git/hooks/pre-commit") # Ensure correct path

        # Try to find the pre-commit tool already installed in the system
        find_program(
            PRE_COMMIT_TOOL
            NAMES pre-commit
            DOC "Pre-commit tool")

        if(PRE_COMMIT_TOOL)
            # If pre-commit is found, notify the user
            message(STATUS "Pre-commit is already installed at: ${PRE_COMMIT_TOOL}")
        else()
            # Pre-commit is not found, proceed with the installation

            # Check the Python version installed on the system
            execute_process(
                COMMAND "${Python3_EXECUTABLE}" --version
                OUTPUT_VARIABLE PYTHON_VERSION_OUTPUT
                OUTPUT_STRIP_TRAILING_WHITESPACE)

            # Extract the major and minor version numbers of Python
            string(REGEX MATCH "([0-9]+)\\.([0-9]+)" PYTHON_VERSION "${PYTHON_VERSION_OUTPUT}")
            string(REGEX REPLACE "([0-9]+)\\.([0-9]+)" "\\1" PYTHON_VERSION_MAJOR "${PYTHON_VERSION}")
            string(REGEX REPLACE "([0-9]+)\\.([0-9]+)" "\\2" PYTHON_VERSION_MINOR "${PYTHON_VERSION}")

            # By default, no --user option is used for pip install
            set(pip_user_option "")

            # If Python version is >= 3.12 and PYTHON_INSTALL_ON_USER is enabled, use the --user option
            if(PYTHON_INSTALL_ON_USER AND ((PYTHON_VERSION_MAJOR GREATER 3)
                                           OR (PYTHON_VERSION_MAJOR EQUAL 3 AND PYTHON_VERSION_MINOR GREATER_EQUAL 12)))
                set(pip_user_option "--user")
            endif()

            # Install necessary tools using pip (editorconfig-checker, black, cmakelang, and pre-commit)
            execute_process(
                COMMAND "${Python3_EXECUTABLE}" -m pip install --upgrade editorconfig-checker black cmakelang pre-commit
                        ${PIP_USER_OPTION}
                WORKING_DIRECTORY "." # Set current working directory
                RESULT_VARIABLE COMMAND_RESULT
                OUTPUT_QUIET ERROR_QUIET)

            # If the installation fails, issue a warning message
            if(NOT COMMAND_RESULT EQUAL "0")
                message(WARNING "Cannot install editorconfig-checker, black, pre-commit, or cmakelang")
            endif()
        endif()

        # Check if the .pre-commit-config.yaml file exists (required for pre-commit setup)
        if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.pre-commit-config.yaml")
            message(FATAL_ERROR "The file .pre-commit-config.yaml is mandatory to configure the pre-commit tool")
        endif()

        # Install the pre-commit hook by executing the pre-commit install command
        execute_process(
            COMMAND pre-commit install
            WORKING_DIRECTORY "." # Set current working directory
            RESULT_VARIABLE COMMAND_RESULT
            OUTPUT_QUIET ERROR_QUIET)

        # If the installation fails, issue a warning message
        if(NOT COMMAND_RESULT EQUAL "0")
            message(WARNING "Cannot install pre-commit")
        else()
            # Success message if pre-commit is installed
            message(STATUS "Pre-commit tools installed successfully")
        endif()
    endif()
endfunction()

# Check if the PRE_COMMIT_TOOL option is enabled, then configure pre-commit
if(PRE_COMMIT_TOOL)
    configure_pre_commit()
endif()
