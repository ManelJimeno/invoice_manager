/**
 * @file sql_builder.cpp
 * @brief Implementation file for the SQLBuilder class in the database core module.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "sql_builder.h"

#include <utility>

namespace core::db
{

void SQLBuilder::addColumn(std::shared_ptr<Column> column)
{
    m_columns.push_back(std::move(column));
}

void SQLBuilder::setTableName(QString tableName)
{
    m_tableName = std::move(tableName);
}

QString SQLBuilder::name() const
{
    return m_tableName;
}

const QVector<std::shared_ptr<Column>>& SQLBuilder::columns() const
{
    return m_columns;
}

SQLBuilder::SQLBuilder(QString dbType) : m_dbTypeName(std::move(dbType))
{
}

} // namespace core::db
