/**
 * @file db_manager.h
 * @brief Declares the DBManager class for managing database connections.
 *
 * Provides a singleton manager for establishing, managing, and retrieving
 * multiple named database connections and specifying allowed database types.
 *
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "dllexports.h"

#include <QMap>
#include <QSet>
#include <QSqlDatabase>
#include <exception.h>

namespace core::db
{

    /**
     * @class DBManagerException
     * @brief Custom exception class for `DBManager`.
     *
     * Extends the general `Exception` class to handle errors that
     * specifically occur in the context of `DBManager`.
     */
    class DBManagerException final : public Exception
    {
        using Exception::Exception; ///< Inherits constructors from the `Exception` base class.
    };

    /**
     * @class DBManager
     * @brief Singleton class for managing multiple database connections.
     *
     * `DBManager` provides a centralized interface for creating and managing
     * multiple database connections, each identified by a unique name.
     */
    class CORE_API DBManager
    {
    public:
        /**
         * @brief Default connection name for unnamed connections.
         *
         * This name is used when a connection name is not provided.
         */
        static constexpr auto DEFAULT_CONNECTION = "DEFAULT_CONNECTION";

        /**
         * @brief Identifier for the SQLite database type.
         *
         * Constant representing the SQLite database type for use in
         * connection operations.
         */
        static constexpr auto QSQLITE = "QSQLITE";

        /**
         * @brief Provides access to the singleton instance of `DBManager`.
         *
         * Allows access to the single `DBManager` instance in the system.
         *
         * @return The singleton instance of `DBManager`.
         */
        static DBManager &manager();

        /**
         * @brief Establishes a database connection.
         *
         * Creates a connection to the specified database type using the provided
         * connection information and name.
         *
         * @param dbType The type of database to connect to.
         * @param connectionInfo The connection string or information.
         * @param connectionName The name of the database connection.
         * @return A `QSqlDatabase` object representing the connection.
         */
        [[nodiscard]] QSqlDatabase connect(const QString &dbType, const QString &connectionInfo = "",
                                           const QString &connectionName = DEFAULT_CONNECTION);

        /**
         * @brief Retrieves an existing database connection.
         *
         * Allows access to a database connection by name. If no name is provided,
         * returns the default connection.
         *
         * @param connectionName The name of the desired connection.
         * @return A reference to the `QSqlDatabase` object for the connection.
         */
        [[nodiscard]] QSqlDatabase &connection(const QString &connectionName = DEFAULT_CONNECTION);

        /**
         * @brief Gets the set of allowed database types.
         *
         * Retrieves a set of database types allowed by this manager for connections.
         *
         * @return A constant reference to a set of allowed database types.
         */
        [[nodiscard]] static const QSet<QString> &allowTypes();

        /**
         * @brief Provides access to the main database connection.
         *
         * @return A `QSqlDatabase` object representing the main connection.
         */
        [[nodiscard]] QSqlDatabase main() const
        {
            return m_main;
        }

    private:
        /**
         * @brief Private constructor for the singleton pattern.
         *
         * Initializes the manager and defines the set of allowed database types.
         */
        DBManager();

        QSqlDatabase                m_main; ///< Main database connection.
        QMap<QString, QSqlDatabase> m_connections; ///< Map of connection names to `QSqlDatabase` objects.
        static const QSet<QString>  m_allowedDBTypes; ///< Allowed database types for connections.
    };

} // namespace core::db
