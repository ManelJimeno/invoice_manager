/**
 * @file sqlite_db_api.h
 * @brief Contains the declaration of the SQLiteDbApi class for managing SQLite-specific database operations.
 *
 * This file defines the SQLiteDbApi class, which encapsulates operations specific to SQLite databases,
 * such as retrieving the last inserted row ID and managing database connections.
 *
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once
#include <QSqlDatabase>
#include "dllexports.h"

namespace core::db
{
    /**
     * @class SQLiteDbApi
     * @brief Encapsulates SQLite-specific database operations.
     *
     * The SQLiteDbApi class provides an abstraction for interacting with SQLite databases.
     * It supports operations such as retrieving the last inserted row ID and managing connections.
     */
    class CORE_API SQLiteDbApi
    {
    public:
        /**
         * @brief Constructs a new SQLiteDbApi object with a specified database connection.
         *
         * @param db The QSqlDatabase object representing the SQLite database connection.
         */
        explicit SQLiteDbApi(const QSqlDatabase &db);

        /**
         * @brief Retrieves the row ID of the last inserted row.
         *
         * This method provides a SQLite-specific operation to fetch the `rowid` of the last
         * successfully inserted row in the connected database.
         *
         * @return The ID of the last inserted row as a long long integer.
         */
        long long getLastInsertRowId() const;

    protected:
        QSqlDatabase m_database; ///< The QSqlDatabase object representing the SQLite connection.
    };
} // namespace core::db
