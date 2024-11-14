/**
 * @file sqlite_builder.h
 * @brief Contains the declaration of the SQLiteBuilder class.
 *
 * This file defines the SQLiteBuilder class, which provides methods for generating
 * SQL statements such as CREATE TABLE, INSERT, UPDATE, DELETE, and SELECT specifically
 * tailored for SQLite databases.
 *
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "db/db_manager.h"
#include "db/sql_builder.h"
#include "sqlite_column.h"

namespace core::db
{

/**
 * @class SQLiteBuilder
 * @brief A builder class for constructing SQL statements for SQLite tables.
 *
 * The SQLiteBuilder class is responsible for generating various SQL queries used
 * to interact with SQLite tables, including table creation, data insertion, data
 * updates, deletion, and selection.
 */
class SQLiteBuilder final : public SQLBuilder
{
  public:
    /**
     * @brief Constructs a SQLiteBuilder instance.
     *
     * Initializes the builder with the SQLite database type defined in DBManager.
     */
    SQLiteBuilder() : SQLBuilder(DBManager::QSQLITE) {};

    /**
     * @brief Finds a column by name within a vector of Column shared pointers.
     *
     * Searches through the vector of columns and returns a shared pointer to
     * the column with the specified name, if it exists.
     *
     * @param columnName The name of the column to search for.
     * @return std::optional<std::shared_ptr<core::db::Column>> The shared pointer to the Column if found; otherwise,
     * std::nullopt.
     */
    [[nodiscard]] std::optional<std::shared_ptr<core::db::Column>> column(const QString& columnName) override;

    /**
     * @brief Generates a SQL CREATE TABLE statement.
     *
     * Constructs a SQL CREATE TABLE statement based on the columns provided, including
     * foreign key constraints if defined in the columns.
     *
     * @return QString The CREATE TABLE statement as a QString.
     */
    [[nodiscard]] QString createTable() const override;

    /**
     * @brief Generates SQL statements to create indices for columns.
     *
     * Creates a set of SQL statements to create indexes for columns that have index names defined.
     * These statements are returned as a vector of individual SQL commands.
     *
     * @return QVector<QString> A vector of SQL statements, each creating an index.
     */
    [[nodiscard]] QVector<QString> createIndexes() const override;

    /**
     * @brief Generates the SQL INSERT statement for inserting a new row.
     *
     * Constructs an SQL INSERT statement with placeholders for each column value. This
     * is used to add a new row to the table with values bound to the placeholders.
     *
     * @return QString A SQL query to insert data into the table.
     */
    [[nodiscard]] QString createInsert() const override;

    /**
     * @brief Generates the SQL UPDATE statement to modify existing rows.
     *
     * Constructs an SQL UPDATE statement with placeholders for column values.
     * Includes a WHERE clause targeting specific rows based on their values.
     *
     * @return QString A SQL query to update rows in the table.
     */
    [[nodiscard]] QString createUpdate() const override;

    /**
     * @brief Generates the SQL SELECT statement to retrieve all rows.
     *
     * Constructs a simple SQL SELECT statement that retrieves all columns
     * and rows from the table.
     *
     * @return QString A SQL query to select all rows from the table.
     */
    [[nodiscard]] QString createSelect() const override;

    /**
     * @brief Generates the SQL SELECT statement to select rows by primary key.
     *
     * Creates an SQL SELECT statement with a WHERE clause that filters rows
     * based on their primary key values.
     *
     * @return QString A SQL query to select rows by primary key.
     */
    [[nodiscard]] QString createSelectPk() const override;

    /**
     * @brief Generates the SQL DELETE statement to remove rows from the table.
     *
     * Constructs an SQL DELETE statement that includes a WHERE clause to delete
     * specific rows from the table based on their column values.
     *
     * @return QString A SQL query to delete rows from the table.
     */
    [[nodiscard]] QString createDelete() const override;

    /**
     * @brief Generates the WHERE clause for SQL queries.
     *
     * Constructs a WHERE clause targeting columns that are marked as primary keys
     * or indexed. Placeholder values are used for each column in the clause.
     *
     * @return QString The WHERE clause for SQL queries.
     */
    [[nodiscard]] QString whereClause() const override;
};

} // namespace core::db
