/**
 * @file dynamic_table.h
 * @brief Header file for the StatementType and Table classes.
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
 * @brief Represents a database table, containing columns and SQL statements.
 *
 * This class holds information about a table's name, its columns, and associated SQL statements.
 */
class CORE_API DynamicTable
{
  public:
    static constexpr auto CREATE = "create";       ///< Represents the CREATE statement type.
    static constexpr auto INSERT = "insert";       ///< Represents the INSERT statement type.
    static constexpr auto DELETE = "delete";       ///< Represents the DELETE statement type.
    static constexpr auto UPDATE = "update";       ///< Represents the UPDATE statement type.
    static constexpr auto SELECT = "select";       ///< Represents the SELECT statement type.
    static constexpr auto SELECT_PK = "select_pk"; ///< Represents the SELECT_PK statement type.

    /**
     * @brief Constructs a Table with a given name.
     * @param database The connection to the database.
     * @param name The name of the table.
     * @param columns A list of columns definition
     */
    explicit DynamicTable(const QSqlDatabase& database, QString name,
                          std::initializer_list<std::shared_ptr<db::Column>> columns);
    /**
     * @brief Retrieves the list of columns in the table.
     * @return Vector of Column objects.
     */
    [[nodiscard]] const QVector<std::shared_ptr<Column>>& columns() const;
    /**
     * @brief Creates the database table.
     * Executes the SQL statement to create the table based on the defined columns.
     */
    void create();
    /**
     * @brief Inserts values into the table.
     * @param columns A map containing the column names and their respective values to insert.
     */
    void insert(const QMap<QString, QVariant>& columns);

    /**
     * @brief Updates rows in the table.
     * @param columns A map containing the column names and their respective values to update.
     */
    void update(const QMap<QString, QVariant>& columns);

    /**
     * @brief Deletes rows from the table.
     * @param columns A map containing the conditions for deleting rows.
     */
    void deleteRows(const QMap<QString, QVariant>& columns);

    /**
     * @brief Selects rows using the primary key.
     * @param columns A map containing the primary key values.
     * @return A list of records selected from the table.
     */
    QList<QSqlRecord> selectPk(const QMap<QString, QVariant>& columns);

    /**
     * @brief Selects all rows from the table.
     * @return A list of records selected from the table.
     */
    QList<QSqlRecord> select();

  private:
    /**
     * @brief Ensures a prepared statement exists for the given SQL operation.
     * @param name The type of SQL statement (e.g., INSERT, UPDATE).
     * @param statement The SQL query to prepare (optional).
     * @return A shared pointer to the prepared SQL query.
     */
    std::shared_ptr<QSqlQuery> ensureStatementExists(const QString& name, const QString& statement = "");
    /**
     * @brief Executes the given SQL statement with bound values.
     * @param statement A shared pointer to the SQL query to execute.
     * @param columns A map of column names and their respective values to bind.
     */
    static void exec(const std::shared_ptr<QSqlQuery>& statement, const QMap<QString, QVariant>& columns);

    const QSqlDatabase& m_database;        ///< The database connection used by the table.
    QString m_name;                        ///< Name of the table.
    std::shared_ptr<SQLBuilder> m_builder; ///< The SQL builder adapted to the QSqlDatabase::driverName().
    QMap<QString, std::shared_ptr<QSqlQuery>> m_statements; ///< List of SQL statements for the table.
    QMap<QString, QString> m_sentences;                     ///< List of SQL sentences for the table.
};

} // namespace core::db
