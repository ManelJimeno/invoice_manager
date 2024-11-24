/**
 * @file sql_builder.h
 * @brief Contains the declaration of the SQLBuilder class template.
 *
 * This file declares the SQLBuilder class, which is responsible for generating SQL
 * statements such as CREATE, INSERT, UPDATE, DELETE, SELECT, and indexes for a table.
 * The class is designed to be flexible and supports different database types by
 * adapting the SQL syntax according to the database system (e.g., SQLite, MySQL).
 *
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include <QJsonObject>
#include <QVector>
#include <memory>
#include "column.h"

namespace core::db
{

    /**
     * @class SQLBuilder
     * @brief Generates SQL statements based on the provided table structure and database type.
     *
     * This class is responsible for creating SQL commands for table creation, inserting data,
     * updating records, deleting records, and generating SQL queries such as SELECT and WHERE
     * clauses. It supports multiple database types by generating syntax-specific SQL for each
     * database type.
     */
    class CORE_API SQLBuilder
    {
    public:
        /**
         * @brief Destructor for SQLBuilder.
         *
         * Destroys the SQLBuilder instance, releasing any allocated resources.
         */
        virtual ~SQLBuilder() = default;

        /**
         * @brief Adds a column to the table definition.
         *
         * Adds a new column to the internal list of columns that will be included in the
         * generated SQL statements (e.g., CREATE TABLE, INSERT).
         *
         * @param column A shared pointer to a Column object representing the new column.
         */
        void addColumn(std::shared_ptr<Column> column);

        /**
         * @brief Sets the name of the table.
         *
         * Sets the name of the table to be used in the generated SQL statements (e.g., CREATE TABLE).
         *
         * @param tableName The name of the table to be used.
         */
        void setTableName(QString tableName);

        /**
         * @brief Retrieves the name of the table.
         *
         * Returns the name of the table as a QString.
         *
         * @return The table name.
         */
        [[nodiscard]] QString name() const;

        /**
         * @brief Retrieves the list of columns in the table.
         *
         * Returns a constant reference to the list of columns in the table, where each column
         * is represented as a shared pointer to a Column object.
         *
         * @return A const reference to a QVector of shared pointers to Column objects.
         */
        [[nodiscard]] const QVector<std::shared_ptr<Column>> &columns() const;

        /**
         * @brief Finds a column by name within a vector of Column shared pointers.
         *
         * Searches through the list of columns to find a column by its name and returns
         * a shared pointer to the matching column, if found.
         *
         * @param columnName The name of the column to search for.
         * @return An optional containing a shared pointer to the Column if found; otherwise, std::nullopt.
         */
        [[nodiscard]] virtual std::optional<std::shared_ptr<core::db::Column>> column(const QString &columnName) = 0;

        /**
         * @brief Generates a CREATE TABLE statement for the given table.
         *
         * Creates a valid SQL statement to create a table with the columns defined in the
         * SQLBuilder object.
         *
         * @return The generated CREATE TABLE statement as a QString.
         */
        [[nodiscard]] virtual QString createTable() const = 0;

        /**
         * @brief Generates SQL statements to create indices for the table, if any.
         *
         * Returns a list of SQL statements to create indices on the table columns if any
         * are defined.
         *
         * @return A QVector of SQL statements (each as a QString) to create indices.
         */
        [[nodiscard]] virtual QVector<QString> createIndexes() const = 0;

        /**
         * @brief Creates the SQL INSERT statement for the table.
         *
         * Generates an SQL INSERT statement based on the defined columns of the table.
         *
         * @return The generated INSERT statement as a QString.
         */
        [[nodiscard]] virtual QString createInsert() const = 0;

        /**
         * @brief Creates the SQL UPDATE statement for the table.
         *
         * Generates an SQL UPDATE statement to update rows in the table, including
         * the columns and their new values.
         *
         * @return The generated UPDATE statement as a QString.
         */
        [[nodiscard]] virtual QString createUpdate() const = 0;

        /**
         * @brief Creates the SQL SELECT statement for the table.
         *
         * Generates an SQL SELECT statement to query all rows from the table.
         *
         * @return The generated SELECT statement as a QString.
         */
        [[nodiscard]] virtual QString createSelect() const = 0;

        /**
         * @brief Creates the SQL SELECT statement using the primary key condition.
         *
         * Generates a SELECT statement specifically for querying rows using the primary key
         * as a condition.
         *
         * @return The generated SELECT statement for primary key queries as a QString.
         */
        [[nodiscard]] virtual QString createSelectPk() const = 0;

        /**
         * @brief Creates the SQL SELECT COUNT statement using the primary key condition.
         *
         * Generates a SELECT statement specifically for made a select count
         *
         * @return The generated SELECT statement for primary key queries as a QString.
         */
        [[nodiscard]] virtual QString createSelectCount() const = 0;

        /**
         * @brief Creates the SQL DELETE statement for the table.
         *
         * Generates an SQL DELETE statement to remove rows from the table, based on a condition.
         *
         * @return The generated DELETE statement as a QString.
         */
        [[nodiscard]] virtual QString createDelete() const = 0;

        /**
         * @brief Creates the SQL WHERE clause for the table.
         *
         * Generates an SQL WHERE clause based on the conditions specified for selecting,
         * updating, or deleting rows.
         *
         * @return The generated WHERE clause as a QString.
         */
        [[nodiscard]] virtual QString whereClause() const = 0;

    protected:
        /**
         * @brief Constructs an SQLBuilder for a specific database type.
         *
         * Protected constructor to initialize the SQLBuilder with the database type
         * for which SQL statements will be generated.
         *
         * @param dbType The name of the database type (e.g., "QSQLITE").
         */
        explicit SQLBuilder(QString dbType);

        QString                          m_dbTypeName; ///< Name of the database type (e.g., "QSQLITE").
        QString                          m_tableName; ///< Name of the table for which SQL will be generated.
        QVector<std::shared_ptr<Column>> m_columns; ///< List of columns in the table.
    };

} // namespace core::db
