/**
 * @file settings.h
 * @brief Header file for the Settings class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// Include necessary headers
#include "dllexports.h"
#include <QVariant>

namespace core::settings
{

/**
 * @class Settings
 * @brief Abstract base class for settings management.
 *
 * The `Settings` class provides a common interface for handling application settings.
 * It defines the basic operations of reading and writing settings, as well as a way
 * to access individual settings by key. It uses a `QMap` to store settings values.
 */
class CORE_API Settings
{
  public:
    /**
     * @brief Virtual function to write settings to persistent storage.
     *
     * This function should be implemented by derived classes to define the mechanism
     * for saving the settings. Returns `true` if the operation was successful, `false` otherwise.
     *
     * @return A boolean indicating whether the write operation was successful.
     */
    virtual bool write() = 0;

    /**
     * @brief Virtual function to read settings from persistent storage.
     *
     * This function should be implemented by derived classes to define the mechanism
     * for loading the settings. Returns `true` if the operation was successful, `false` otherwise.
     *
     * @return A boolean indicating whether the read operation was successful.
     */
    virtual bool read() = 0;

    /**
     * @brief Virtual destructor for the Settings class.
     *
     * Ensures that any resources allocated by derived classes are cleaned up properly.
     */
    virtual ~Settings() = default;

    /**
     * @brief Overloaded `operator[]` to access settings by key.
     *
     * This operator allows accessing settings using the `[]` syntax, enabling users
     * to directly retrieve or modify values stored in the settings map.
     *
     * @param key The key used to access the setting.
     * @return A reference to the setting value associated with the provided key.
     */
    QVariant& operator[](const QString& key);

  protected:
    QMap<QString, QVariant> m_values; ///< A map storing the settings with their respective keys.
};

} // namespace core::settings
