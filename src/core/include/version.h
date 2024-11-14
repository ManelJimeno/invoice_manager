/**
 * @file version.h
 * @brief Header file for the Version class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// Import necessary libraries and definitions
#include "dllexports.h"
#include <QString>

namespace core::package
{

/**
 * @struct Version
 * @brief Struct that holds version information.
 *
 * The `Version` struct is used to store detailed information about a software version,
 * including the full version string, a triplet representing the version in a specific format,
 * and individual components such as the major, minor, and revision numbers.
 */
struct Version
{
    QString full;    ///< The full version string (e.g., "1.0.0").
    QString triplet; ///< The version triplet (e.g., "v1.0.0").
    int major;       ///< Major version number (e.g., 1 in "1.0.0").
    int minor;       ///< Minor version number (e.g., 0 in "1.0.0").
    int revision;    ///< Revision version number (e.g., 0 in "1.0.0").
};

/**
 * @brief Retrieves the current version of the software package.
 *
 * This function returns a `Version` struct containing information about the current
 * software version, including the full version string, triplet, and individual version
 * components such as major, minor, and revision numbers.
 *
 * @return A `Version` struct containing version information.
 *
 * @note This function is marked as noexcept, indicating it will not throw exceptions.
 */
CORE_API Version getVersion() noexcept;

} // namespace core::package
