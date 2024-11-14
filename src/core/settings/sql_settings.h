/**
 * @file settings.h
 * @brief Header file for the SQLSettings class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// Include necessary headers
#include "db/dynamic_table.h"
#include "settings.h"
#include <QSqlDatabase>

namespace core::settings
{

/**
 * @class SQLSettings
 * @brief Derived class for managing SQL-based settings.
 *
 * `SQLSettings` extends the `Settings` class to provide an implementation for settings
 * management that involves storing and retrieving settings from a SQL database.
 */
class CORE_API SQLSettings : public Settings
{
  protected:
    /**
     * @brief Constructor for SQLSettings.
     *
     * The constructor initializes an instance of `DynamicTable` to interact with the
     * database table for storing settings. This is a protected constructor, meaning it can
     * only be invoked by derived classes.
     *
     * @param database The `QSqlDatabase` object used for database interaction.
     * @param name The name of the settings table in the database.
     * @param columns A list of columns used to define the table's structure.
     */
    explicit SQLSettings(const QSqlDatabase& database, QString name,
                         const std::initializer_list<std::shared_ptr<db::Column>> columns)
        : m_table(database, std::move(name), columns) // Initialize the m_table with database, name, and columns
    {
    }

  public:
    /**
     * @brief Accessor for the `DynamicTable` instance.
     *
     * Provides access to the underlying `DynamicTable` used for interacting with the SQL database.
     * This function is useful for operations like querying the database for settings.
     *
     * @return The `DynamicTable` instance for the settings table.
     */
    auto table()
    {
        return m_table;
    };

    /**
     * @brief Virtual function to write settings to the database.
     *
     * This function must be implemented by derived classes to define the mechanism
     * for saving settings to the database. Returns `true` if the operation was successful,
     * `false` otherwise.
     *
     * @return A boolean indicating whether the write operation was successful.
     */
    bool write() override = 0;

    /**
     * @brief Virtual function to read settings from the database.
     *
     * This function must be implemented by derived classes to define the mechanism
     * for reading settings from the database. Returns `true` if the operation was successful,
     * `false` otherwise.
     *
     * @return A boolean indicating whether the read operation was successful.
     */
    bool read() override = 0;

    /**
     * @brief Destructor for SQLSettings.
     *
     * Ensures that any resources are cleaned up properly when the object is destroyed.
     */
    ~SQLSettings() override = default;

  protected:
    /**
     * @brief The `DynamicTable` instance for managing the settings table in the database.
     *
     * The `DynamicTable` object `m_table` provides methods for interacting with the SQL
     * table where settings are stored. It is used for operations like creating, updating,
     * and deleting rows in the table.
     */
    db::DynamicTable m_table;
};

} // namespace core::settings
