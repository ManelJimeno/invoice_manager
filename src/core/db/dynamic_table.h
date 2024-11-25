/**
 * @file dynamic_table.h
 * @brief Header file for the StatementType and DynamicTable classes.
 *
 * This file declares the DynamicTable class, responsible for managing
 * database tables, their columns, and SQL statements for data manipulation.
 *
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "column.h"
#include "db_exception.h"
#include "sql_builder.h"

#include <QMap>
#include <QSqlQuery>
#include <QSqlRecord>

namespace core::db
{

    /**
     * @class DynamicTable
     * @brief Manages a database table's structure and SQL operations.
     *
     * The DynamicTable class provides functionality for creating and managing
     * SQL statements associated with a specific table, including create, insert,
     * delete, update, and select operations. It holds the table's columns and
     * connection to the database, facilitating interaction with stored data.
     */
    class CORE_API DynamicTable
    {
    public:
        static constexpr auto CREATE    = "create"; ///< Represents the CREATE statement type.
        static constexpr auto INSERT    = "insert"; ///< Represents the INSERT statement type.
        static constexpr auto DELETE    = "delete"; ///< Represents the DELETE statement type.
        static constexpr auto UPDATE    = "update"; ///< Represents the UPDATE statement type.
        static constexpr auto SELECT    = "select"; ///< Represents the SELECT statement type.
        static constexpr auto SELECT_PK = "select_pk"; ///< Represents the SELECT_PK statement type.

        /**
         * @brief Constructs a DynamicTable with a specified name and columns.
         * @param database A reference to the active database connection.
         * @param name The name of the table.
         * @param columns An initializer list of column definitions.
         */
        explicit DynamicTable(const QSqlDatabase &database, QString name,
                              std::initializer_list<std::shared_ptr<db::Column>> columns);

        /**
         * @brief Retrieves the list of columns associated with the table.
         * @return A constant vector of shared pointers to Column objects.
         */
        [[nodiscard]] const QVector<std::shared_ptr<Column>> &columns() const;

        /**
         * @brief Executes SQL to create the table based on its defined columns.
         *
         * Generates and executes a SQL CREATE statement to initialize the table
         * structure in the database.
         */
        void create();

        /**
         * @brief Inserts values into the table.
         *
         * Constructs and executes an SQL INSERT statement using the provided
         * column-value mappings.
         *
         * @param columns A map of column names to values for insertion.
         */
        void insert(const QMap<QString, QVariant> &columns);

        /**
         * @brief Updates rows in the table with specified values.
         *
         * Constructs and executes an SQL UPDATE statement to modify table rows,
         * using the provided column-value mappings.
         *
         * @param columns A map of column names to values for updating rows.
         */
        void update(const QMap<QString, QVariant> &columns);

        /**
         * @brief Deletes rows from the table based on specified conditions.
         *
         * Constructs and executes an SQL DELETE statement to remove rows matching
         * the conditions provided in the column-value mappings.
         *
         * @param columns A map of conditions for deleting specific rows.
         */
        void deleteRows(const QMap<QString, QVariant> &columns);

        /**
         * @brief Selects rows from the table using the primary key values.
         *
         * Constructs and executes a SELECT query to retrieve rows that match
         * the specified primary key values.
         *
         * @param columns A map of primary key column names to their values.
         * @return A list of QSqlRecord objects containing the selected rows.
         */
        QList<QSqlRecord> selectPk(const QMap<QString, QVariant> &columns);

        /**
         * @brief Selects all rows from the table.
         *
         * Executes a SELECT query to retrieve all records in the table.
         *
         * @return A list of QSqlRecord objects containing all rows in the table.
         */
        QList<QSqlRecord> select();

    private:
        /**
         * @brief Ensures that a prepared statement exists for the given SQL operation.
         *
         * Checks for an existing prepared statement for the specified SQL statement type,
         * creating and storing it if it does not already exist.
         *
         * @param name The name of the SQL statement type (e.g., INSERT, UPDATE).
         * @param statement The SQL query to prepare (optional).
         * @return A shared pointer to the prepared QSqlQuery object.
         */
        std::shared_ptr<QSqlQuery> ensureStatementExists(const QString &name, const QString &statement = "");

        /**
         * @brief Executes a prepared SQL statement with the specified bound values.
         *
         * Binds each column name to its corresponding value in the provided map,
         * then executes the SQL statement.
         *
         * @param statement A shared pointer to the QSqlQuery object to execute.
         * @param columns A map of column names and their values to bind to the statement.
         */
        static void exec(const std::shared_ptr<QSqlQuery> &statement, const QMap<QString, QVariant> &columns);

        const QSqlDatabase         &m_database; ///< Reference to the database connection used by the table.
        QString                     m_name; ///< Name of the table.
        std::shared_ptr<SQLBuilder> m_builder; ///< SQLBuilder instance adapted to the QSqlDatabase::driverName().
        QMap<QString, std::shared_ptr<QSqlQuery>> m_statements; ///< Map of SQL statements prepared for this table.
        QMap<QString, QString>                    m_sentences; ///< Map of raw SQL sentence strings for the table.
    };

} // namespace core::db
