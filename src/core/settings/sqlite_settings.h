/**
 * @file sqlite_settings.h
 * @brief Header file for the SQLiteSettings class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// Include necessary headers
#include "db/dynamic_table.h"
#include "sql_settings.h"

namespace core::settings
{

    /**
     * @class SQLiteSettings
     * @brief Concrete class for handling settings stored in an SQLite database.
     *
     * `SQLiteSettings` is a subclass of `SQLSettings` and provides an implementation
     * for reading and writing settings specifically for SQLite databases. It overrides
     * the pure virtual functions from `SQLSettings` to implement the database-specific logic.
     */
    class CORE_API SQLiteSettings final : public SQLSettings
    {
    public:
        /**
         * @brief Constructor for SQLiteSettings.
         *
         * Initializes an `SQLiteSettings` object by associating it with a specific SQLite database
         * and a table name. This constructor uses the parent class `SQLSettings` constructor
         * to set up the database and table structure.
         *
         * @param database The `QSqlDatabase` object that represents the SQLite database connection.
         * @param name The name of the settings table in the SQLite database.
         */
        SQLiteSettings(const QSqlDatabase &database, QString name);

        /**
         * @brief Destructor for SQLiteSettings.
         *
         * The destructor is automatically called when the object goes out of scope, ensuring proper
         * cleanup of resources. The `override` keyword ensures the destructor is correctly overridden
         * from the base class `SQLSettings`.
         */
        ~SQLiteSettings() override = default;

        /**
         * @brief Implements the write function to store settings in the SQLite database.
         *
         * This function writes the current settings to the SQLite database. It must be implemented
         * in this class, as it is a pure virtual function in the base class `SQLSettings`.
         *
         * @return A boolean value indicating whether the write operation was successful (`true`) or not (`false`).
         */
        bool write() override;

        /**
         * @brief Implements the read function to load settings from the SQLite database.
         *
         * This function reads the settings from the SQLite database. It must be implemented in this
         * class, as it is a pure virtual function in the base class `SQLSettings`.
         *
         * @return A boolean value indicating whether the read operation was successful (`true`) or not (`false`).
         */
        bool read() override;
    };

} // namespace core::settings
