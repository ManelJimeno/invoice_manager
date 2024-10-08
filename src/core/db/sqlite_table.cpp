/**
 * @file sqlite_table.cpp
 * @brief Implementation file for the SQLiteTable class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "sqlite_table.h"

namespace core::db
{
/**
 * @brief Generates an SQL CREATE TABLE statement for the current SQLite table.
 *
 * The query includes column definitions, their types, NULL constraints, and PRIMARY KEY
 * constraints for indexed columns.
 *
 * @return A string containing the SQL query to create the table.
 */
QString SQLiteTable::createTable() const
{
    QString query = "CREATE TABLE IF NOT EXISTS " + this->name() + " (";
    auto cols = columns();
    QString primaryKeyClause;
    QString foreignKeyClause;

    for (auto it = cols.begin(); it != cols.end(); ++it)
    {
        // Nombre de la columna
        query += it.key() + " " + toSQLiteType(it.value().type());

        // NOT NULL
        if (!it.value().nullAllowed())
        {
            query += " NOT NULL";
        }

        // PRIMARY KEY y AUTOINCREMENT
        if (it.value().isPrimaryKey())
        {
            query += " PRIMARY KEY";
            if (it.value().isAutoIncrement())
            {
                query += " AUTOINCREMENT";
            }
        }

        // UNIQUE
        if (it.value().isUnique())
        {
            query += " UNIQUE";
        }

        // CHECK condition
        if (!it.value().checkCondition().isEmpty())
        {
            query += " CHECK(" + it.value().checkCondition() + ")";
        }

        // DEFAULT value
        if (!it.value().defaultValue().isEmpty())
        {
            query += " DEFAULT " + it.value().defaultValue();
        }

        // COLLATE
        if (!it.value().collate().isEmpty())
        {
            query += " COLLATE " + it.value().collate();
        }

        // FOREIGN KEY (se define después de la lista de columnas)
        if (!it.value().foreignKey().isEmpty())
        {
            if (!foreignKeyClause.isEmpty())
            {
                foreignKeyClause += ", ";
            }
            foreignKeyClause += "FOREIGN KEY (" + it.key() + ") REFERENCES " + it.value().foreignKey();
        }

        // Añade una coma si no es la última columna
        if (std::next(it) != cols.end())
        {
            query += ", ";
        }
    }

    // Si hay cláusulas de clave foránea, añádelas al final
    if (!foreignKeyClause.isEmpty())
    {
        query += ", " + foreignKeyClause;
    }

    query += ");";
    return query;
}

/**
 * @brief Generates an SQL INSERT statement for the current SQLite table.
 *
 * The query is generated using the table's column names as the fields to insert values into.
 * The values are represented as placeholders (e.g., `:column_name`).
 *
 * @return A string containing the SQL query to insert values into the table.
 */
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

/**
 * @brief Generates an SQL UPDATE statement for the current SQLite table.
 *
 * The query sets the values of columns using placeholders, and includes a WHERE clause
 * for indexed columns to ensure only relevant rows are updated.
 *
 * @return A string containing the SQL query to update rows in the table.
 */
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

/**
 * @brief Generates an SQL DELETE statement for the current SQLite table.
 *
 * The query deletes rows based on conditions specified in the WHERE clause.
 *
 * @return A string containing the SQL query to delete rows from the table.
 */
QString SQLiteTable::createDelete() const
{
    return "DELETE FROM " + name() + whereClause() + ";";
}

/**
 * @brief Generates an SQL SELECT statement that selects rows by primary key.
 *
 * This is essentially a SELECT statement with a WHERE clause targeting the primary key(s).
 *
 * @return A string containing the SQL query to select rows by primary key.
 */
QString SQLiteTable::createSelectPk() const
{
    return createSelect() + whereClause() + ";";
}

/**
 * @brief Generates an SQL SELECT statement that selects all rows from the table.
 *
 * The query fetches all the columns and rows from the table without any condition.
 *
 * @return A string containing the SQL query to select all rows from the table.
 */
QString SQLiteTable::createSelect() const
{
    return "SELECT * FROM " + name();
}

/**
 * @brief Converts an SQLiteDataType enum value to its corresponding SQLite type as a string.
 *
 * This method handles the mapping between the `SQLiteDataType` enum values and
 * their corresponding SQL data types (e.g., INTEGER, TEXT).
 *
 * @param type The `SQLiteDataType` to convert.
 * @return A string representing the SQL data type.
 */
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
    return "TEXT"; // Default type
}

/**
 * @brief Generates a WHERE clause for indexed columns in the table.
 *
 * The WHERE clause uses placeholders for the column values, targeting only columns
 * that are indexed (typically primary key columns).
 *
 * @return A string containing the WHERE clause, or an empty string if no indexed columns exist.
 */
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
