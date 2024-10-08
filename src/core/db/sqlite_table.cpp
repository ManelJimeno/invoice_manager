/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#include "sqlite_table.h"

namespace core::db
{

QString SQLiteTable::createTable() const
{
    QString query = "CREATE TABLE IF NOT EXISTS " + this->name() + " (";
    auto cols = columns();
    for (auto it = cols.begin(); it != cols.end(); ++it)
    {
        query += it.key() + " " + toSQLiteType(it.value().type());
        if (!it.value().nullAllowed())
        {
            query += " NOT NULL ";
        }
        if (it.value().index())
        {
            query += " PRIMARY KEY";
        }
        if (std::next(it) != cols.end())
        {
            query += ", ";
        }
    }
    query += ");";
    return query;
}

QString SQLiteTable::createInsert() const
{
    QString query = "INSERT INTO " + name() + " (";
    QStringList columnsList;
    QStringList valuesList;
    for (auto column : this->columns())
    {
        columnsList << column.name();
        valuesList << ":" + column.name();
    }
    query += columnsList.join(", ") + ") VALUES (" + valuesList.join(", ") + ");";
    return query;
}

QString SQLiteTable::createUpdate() const
{
    QString query = "UPDATE " + name() + " SET ";
    QStringList setList;
    QStringList whereList;
    for (auto column : this->columns())
    {
        setList << column.name() + "=:" + column.name();
        if (column.index() != nullptr)
        {
            whereList << column.name() + "=:" + column.name();
        }
    }
    query += setList.join(", ");
    query += whereClause() + ";";

    return query;
}

QString SQLiteTable::createDelete() const
{
    return "DELETE FROM " + name() + whereClause() + ";";
}

QString SQLiteTable::createSelectPk() const
{
    return createSelect() + whereClause() + ";";
}

QString SQLiteTable::createSelect() const
{
    return "SELECT * FROM " + name();
}

QString SQLiteTable::toSQLiteType(SQLiteDataType type)
{
    switch (type)
    {
    case SQLiteDataType::INTEGER:
        return "INTEGER";
    case SQLiteDataType::REAL:
        return "REAL";
    case SQLiteDataType::TEXT:
        return "TEXT";
    case SQLiteDataType::BLOB:
        return "BLOB";
    case SQLiteDataType::NULL_TYPE:
        return "NULL";
    }
    return "TEXT";
}

QString SQLiteTable::whereClause() const
{
    QStringList whereList;

    for (auto column : this->columns())
    {
        if (column.index() != nullptr)
        {
            whereList << column.name() + "=:" + column.name();
        }
    }

    if (!whereList.empty())
    {
        return " WHERE " + whereList.join(" and ");
    }

    return {};
}

} // namespace core::db
