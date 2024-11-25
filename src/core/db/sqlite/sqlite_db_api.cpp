/**
 * @file sqlite_db_api.cpp
 * @brief Contains the implementation of the TableMetaInfo class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "sqlite_db_api.h"

#include <QSqlError>
#include <QSqlQuery>

#include "db/db_exception.h"


namespace core::db
{
    SQLiteDbApi::SQLiteDbApi(const QSqlDatabase &db) : m_database(db)
    {
    }

    long long SQLiteDbApi::getLastInsertRowId() const
    {
        QSqlQuery query(m_database);
        if (query.exec("SELECT last_insert_rowid();"))
        {
            if (query.next())
            {
                return query.value(0).toLongLong();
            }
            return 0;
        }
        throw core::db::SQLError(query.lastError().text());
    }


} // namespace core::db
