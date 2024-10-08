/**
 * @file sqlite_table.h
 * @brief Declaration of the SQLiteTable class and related types for SQLite database operations.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "sqlite_column.h"
#include "table.h"
#include <QSqlQuery>
#include <QVariant>

namespace core::db
{
/**
 * @brief A class representing an SQLite table for performing various database operations.
 *
 * The SQLiteTable class provides methods to generate SQL statements for operations such as
 * creating, inserting, updating, and deleting rows in a table. It also supports querying
 * the table using SELECT statements.
 */
class SQLiteTable final : public Table<SQLiteColumn>
{
  public:
    /**
     * @brief Constructs an SQLiteTable object with a given set of columns.
     *
     * This constructor takes a database connection, a table name, and a map of column definitions
     * for the table.
     *
     * @param database A reference to the QSqlDatabase object representing the database connection.
     * @param name The name of the table.
     * @param columns A map where the key is the column name and the value is a Column object
     *        representing the properties of the column.
     */
    CORE_API explicit SQLiteTable(const QSqlDatabase& database, QString name, QMap<QString, SQLiteColumn> columns)
        : Table(database, std::move(name), std::move(columns))
    {
    }

    /**
     * @brief Constructs an SQLiteTable object with a list of columns.
     *
     * This constructor takes a database connection, a table name, and an initializer list of
     * column definitions for the table.
     *
     * @param database A reference to the QSqlDatabase object representing the database connection.
     * @param name The name of the table.
     * @param columns An initializer list of pairs, where each pair consists of a column name
     *        and a Column object representing the properties of the column.
     */
    CORE_API explicit SQLiteTable(const QSqlDatabase& database, QString name,
                                  const std::initializer_list<SQLiteColumn> columns)
        : Table(database, std::move(name), columns)
    {
    }

  private:
    /**
     * @brief Generates the SQL statement to create the table.
     *
     * This method constructs a SQL CREATE TABLE statement that includes all columns,
     * their types, and any constraints (such as PRIMARY KEY or NOT NULL).
     *
     * @return A QString containing the SQL query to create the table.
     */
    [[nodiscard]] QString createTable() const override;

    /**
     * @brief Generates the SQL statement to insert a new row into the table.
     *
     * This method constructs an SQL INSERT statement using placeholders for the column values.
     *
     * @return A QString containing the SQL query to insert data into the table.
     */
    [[nodiscard]] QString createInsert() const override;

    /**
     * @brief Generates the SQL statement to update existing rows in the table.
     *
     * This method constructs an SQL UPDATE statement with placeholders for column values.
     * It includes a WHERE clause to target specific rows.
     *
     * @return A QString containing the SQL query to update rows in the table.
     */
    [[nodiscard]] QString createUpdate() const override;

    /**
     * @brief Generates the SQL statement to select all rows from the table.
     *
     * This method constructs a basic SQL SELECT statement that retrieves all columns and rows
     * from the table.
     *
     * @return A QString containing the SQL query to select all rows from the table.
     */
    [[nodiscard]] QString createSelect() const override;

    /**
     * @brief Generates the SQL statement to select rows by primary key.
     *
     * This method constructs a SQL SELECT statement with a WHERE clause to select rows based
     * on their primary key values.
     *
     * @return A QString containing the SQL query to select rows by primary key.
     */
    [[nodiscard]] QString createSelectPk() const override;

    /**
     * @brief Generates the SQL statement to delete rows from the table.
     *
     * This method constructs a SQL DELETE statement that includes a WHERE clause to
     * delete specific rows from the table.
     *
     * @return A QString containing the SQL query to delete rows from the table.
     */
    [[nodiscard]] QString createDelete() const override;

    /**
     * @brief Generates the WHERE clause for SQL queries based on indexed columns.
     *
     * This method constructs a WHERE clause targeting columns marked as primary keys
     * or indices, using placeholders for column values.
     *
     * @return A QString containing the WHERE clause for SQL queries.
     */
    [[nodiscard]] QString whereClause() const override;

    /**
     * @brief Converts a SQLiteDataType enum value to its corresponding SQLite type string.
     *
     * This static method translates a SQLiteDataType enum into a string representation
     * that can be used in SQL queries (e.g., INTEGER, REAL, TEXT).
     *
     * @param type The SQLiteDataType enum value.
     * @return A QString representing the SQL type.
     */
    [[nodiscard]] static QString toSQLiteType(SQLiteDataType type);
};
} // namespace core::db
