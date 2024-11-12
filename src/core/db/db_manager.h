/**
 * @file db_manager.h
 * @brief Contains the declaration of the DBManager class for managing database connections.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @date 2024
 * @author Manel Jimeno
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "dllexports.h"

#include <QMap>
#include <QSet>
#include <QSqlDatabase>

namespace core::db
{

/**
 * @class DBManager
 * @brief Singleton class for managing multiple database connections.
 *
 * The DBManager class provides an interface for connecting to databases, managing
 * multiple named connections, and accessing allowed database types.
 */
class CORE_API DBManager
{
  public:
    /**
     * @brief Default connection name used for unnamed connections.
     */
    static constexpr auto DEFAULT_CONNECTION = "DEFAULT_CONNECTION";

    /**
     * @brief Identifier for the SQLite database type.
     */
    static constexpr auto QSQLITE = "QSQLITE";

    /**
     * @brief Accessor for the singleton instance of DBManager.
     * @return The singleton instance of DBManager.
     */
    static DBManager& manager();

    /**
     * @brief Establishes a database connection.
     *
     * Creates a connection to the specified database type using the given
     * connection information and name. If the connection name is not provided,
     * the default connection name is used.
     *
     * @param dbType The type of database to connect to.
     * @param connectionInfo The connection string or information.
     * @param connectionName The name for the database connection.
     * @return A QSqlDatabase object representing the connection.
     */
    [[nodiscard]] QSqlDatabase connect(const QString& dbType, const QString& connectionInfo = "",
                                       const QString& connectionName = DEFAULT_CONNECTION) const;

    /**
     * @brief Retrieves an existing database connection.
     *
     * Provides access to a database connection by name. If no name is specified,
     * the default connection is returned.
     *
     * @param connectionName The name of the desired connection.
     * @return A reference to the QSqlDatabase object for the connection.
     */
    [[nodiscard]] QSqlDatabase& connection(const QString& connectionName = DEFAULT_CONNECTION);

    /**
     * @brief Gets the set of allowed database types.
     *
     * Retrieves a set of database types permitted by this manager.
     *
     * @return A constant reference to a set of allowed database types.
     */
    [[nodiscard]] static const QSet<QString>& allowTypes();

    [[nodiscard]] QSqlDatabase main() const
    {
        return m_main;
    }

  private:
    /**
     * @brief Private constructor for the singleton pattern.
     *
     * Initializes the manager with a set of allowed database types.
     */
    DBManager();

    QSqlDatabase m_main;
    QMap<QString, QSqlDatabase> m_connections;   ///< Map of connection names to QSqlDatabase objects.
    static const QSet<QString> m_allowedDBTypes; ///< Allowed database types for connection.
};

} // namespace core::db
