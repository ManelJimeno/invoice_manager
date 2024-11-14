/**
 * @file dllexports.h
 * @brief Defines the macro for handling DLL import/export for the Core library.
 *
 * This header defines the `CORE_API` macro used for managing the export and import
 * of symbols when building and using the Core library. It ensures that symbols are
 * correctly exported when building the DLL and imported when linking against the DLL.
 *
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// Check if the platform is Windows (defined by _WIN32)
#ifdef _WIN32

// If building the Core library, define CORE_API for symbol export
#ifdef CORE_EXPORTS
/**
 * @def CORE_API
 * @brief Defines the export symbol for Windows.
 *
 * This macro is used to export symbols from the Core library when it is being
 * compiled as a DLL on Windows. The `__declspec(dllexport)` keyword is used to
 * mark the functions or variables for export.
 */
#define CORE_API __declspec(dllexport)

// If linking against the Core library, define CORE_API for symbol import
#else
/**
 * @def CORE_API
 * @brief Defines the import symbol for Windows.
 *
 * This macro is used to import symbols from the Core library when it is being
 * linked in another application on Windows. The `__declspec(dllimport)` keyword
 * is used to mark the functions or variables for import.
 */
#define CORE_API __declspec(dllimport)

#endif

// If not on Windows, the macro is not needed
#else
/**
 * @def CORE_API
 * @brief Empty definition for non-Windows platforms.
 *
 * On non-Windows platforms (such as Linux or macOS), no special symbol
 * import/export mechanism is needed. Therefore, the macro is defined as empty.
 */
#define CORE_API

#endif
