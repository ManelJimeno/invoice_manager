/**
 * @file db_manager.cpp
 * @brief Implementation file for the Column class in the database core module.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */
#include "db_manager.h"

namespace core::db
{

const QSet<QString> DBManager::m_allowedDBTypes = {DBManager::QSQLITE};

DBManager::DBManager() = default;

DBManager& DBManager::manager()
{
    static DBManager manager;
    return manager;
}

QSqlDatabase DBManager::connect(const QString& dbType, const QString& connectionInfo,
                                const QString& connectionName) const
{
    if (!m_allowedDBTypes.contains(dbType))
    {
        throw std::exception("This type of database is not registered.");
    }
    m_connections[connectionName] = QSqlDatabase::addDatabase(dbType);
    if (dbType.compare(DBManager::QSQLITE) == 0)
    {
        m_connections[connectionName].setDatabaseName(connectionInfo);
    }
    return m_connections[connectionName];
}

QSqlDatabase& DBManager::connection(const QString& connectionName)
{
    return m_connections[connectionName];
}

const QSet<QString>& DBManager::allowTypes()
{
    return m_allowedDBTypes;
}

} // namespace core::db
