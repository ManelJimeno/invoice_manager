#
# Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: https://www.opensource.org/licenses/mit-license.php MIT
#

option(USE_CODE_WARNINGS_AS_ERRORS "Use code warnings as errors" ON)

# Function to configure a target with specific settings Argument: target_name - the name of the target to configure
function(target_configure target_name)

    # Set the target's properties to use C++17 and disable C++ extensions
    set_target_properties(${target_name} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)

    # Check if the compiler is MSVC (Microsoft Visual C++)
    if(MSVC)
        # If the option to treat warnings as errors is enabled
        if(USE_CODE_WARNINGS_AS_ERRORS)
            # Set the compiler options: - /W4: Enable a high level of warnings - /WX: Treat warnings as errors -
            # /wd4996: Disable warning C4996 (deprecated function use) - /wd4805: Disable warning C4805 (unsafe mix of
            # type comparisons)
            target_compile_options(${target_name} PRIVATE /W4 /WX /wd4996 /wd4805)
        endif()

        # If the compiler is not MSVC (likely GCC or Clang)
    else()
        # Define a preprocessor macro to signal C++17 support
        target_compile_definitions(${target_name} PRIVATE _HAS_CXX17=1)

        # If the option to treat warnings as errors is enabled
        if(USE_CODE_WARNINGS_AS_ERRORS)
            # Set the compiler options: -Wall: Enable all common warnings -Wextra: Enable extra warnings -Werror: Treat
            # warnings as errors
            target_compile_options(${target_name} PRIVATE -Wall -Wextra -Werror)
        endif()
    endif()

    # If the CLANG_TIDY_CLI variable is set, apply Clang-Tidy checks to the target
    if(CLANG_TIDY_CLI)
        set_target_properties(${target_name} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_CLI}")
    endif()

endfunction()
