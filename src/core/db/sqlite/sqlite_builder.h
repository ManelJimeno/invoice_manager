/**
 * @file sqlite_meta_info.h
 * @brief Contains the declaration of the SQLiteMetaInfo class.
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

class SQLiteBuilder final : public SQLBuilder
{
  public:
    /**
     * @brief Constructor
     */
    SQLiteBuilder() : SQLBuilder(DBManager::QSQLITE) {};

    /**
     * @brief Finds a column by name within a vector of Column shared pointers.
     *
     * This function searches through a vector of Column pointers and returns
     * a shared pointer to the column with the specified name, if it exists.
     *
     * @param columnName The name of the column to search for.
     * @return An optional containing the shared pointer to the Column if found; otherwise, std::nullopt.
     */
    [[nodiscard]] std::optional<std::shared_ptr<core::db::Column>> column(const QString& columnName) override;
    /**
     * @brief Generates a CREATE TABLE statement for the given table.
     * @return The CREATE TABLE statement as a QString.
     */
    [[nodiscard]] QString createTable() const override;
    /**
     * @brief Generates SQL statements to create indices for the table, if any.
     * @return A vector of SQL statements, each creating an index.
     */
    [[nodiscard]] QVector<QString> createIndexes() const override;
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
};

} // namespace core::db
