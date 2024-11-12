/**
 * @file sql_builder.h
 * @brief Contains the declaration of the SQLBuilder class template.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "column.h"
#include <QJsonObject>
#include <QVector>
#include <memory>

namespace core::db
{

/**
 * @class SQLBuilder
 * @brief Generates SQL statements based on the provided table structure and database type.
 *
 * This class is responsible for creating SQL commands, such as table creation,
 * indices, and insert statements. It supports different database types
 * by generating syntax-specific SQL for each type.
 */
class CORE_API SQLBuilder
{
  public:
    /**
     * @brief Destructor for SQLBuilder.
     */
    virtual ~SQLBuilder() = default;

    /**
     * @brief Adds a column to the table definition.
     * @param column A shared pointer to a Column object.
     */
    void addColumn(std::shared_ptr<Column> column);

    /**
     * @brief Sets the name of the table.
     * @param tableName The name of the table to be used in SQL generation.
     */
    void setTableName(QString tableName);

    /**
     * @brief Retrieves the name of the table.
     * @return The table name as a QString.
     */
    [[nodiscard]] QString name() const;

    /**
     * @brief Retrieves the list of columns in the table.
     * @return A const reference to a QVector of shared pointers to Column objects.
     */
    [[nodiscard]] const QVector<std::shared_ptr<Column>>& columns() const;
    /**
     * @brief Finds a column by name within a vector of Column shared pointers.
     *
     * This function searches through a vector of Column pointers and returns
     * a shared pointer to the column with the specified name, if it exists.
     *
     * @param columnName The name of the column to search for.
     * @return An optional containing the shared pointer to the Column if found; otherwise, std::nullopt.
     */
    [[nodiscard]] virtual std::optional<std::shared_ptr<core::db::Column>> column(const QString& columnName) = 0;
    /**
     * @brief Generates a CREATE TABLE statement for the given table.
     * @return The CREATE TABLE statement as a QString.
     */
    [[nodiscard]] virtual QString createTable() const = 0;
    /**
     * @brief Generates SQL statements to create indices for the table, if any.
     * @return A QVector of QStrings, each representing an SQL statement to create an index.
     */
    [[nodiscard]] virtual QVector<QString> createIndexes() const = 0;
    /**
     * @brief Creates the SQL INSERT statement for the table.
     * @return The INSERT statement as a QString.
     */
    [[nodiscard]] virtual QString createInsert() const = 0;
    /**
     * @brief Creates the SQL UPDATE statement for the table.
     * @return The UPDATE statement as a QString.
     */
    [[nodiscard]] virtual QString createUpdate() const = 0;
    /**
     * @brief Creates the SQL SELECT statement for the table.
     * @return The SELECT statement as a QString.
     */
    [[nodiscard]] virtual QString createSelect() const = 0;
    /**
     * @brief Creates the SQL SELECT statement using the primary key condition.
     * @return The SELECT statement as a QString.
     */
    [[nodiscard]] virtual QString createSelectPk() const = 0;
    /**
     * @brief Creates the SQL DELETE statement for the table.
     * @return The DELETE statement as a QString.
     */
    [[nodiscard]] virtual QString createDelete() const = 0;
    /**
     * @brief Creates the SQL WHERE clause for the table.
     * @return The WHERE clause as a QString.
     */
    [[nodiscard]] virtual QString whereClause() const = 0;

  protected:
    /**
     * @brief Constructs an SQLBuilder for a specific database type.
     * @param dbType The database type name as a QString.
     */
    explicit SQLBuilder(QString dbType);

    QString m_dbTypeName;                       ///< Name of the database type.
    QString m_tableName;                        ///< Name of the table.
    QVector<std::shared_ptr<Column>> m_columns; ///< List of columns in the table.
};

} // namespace core::db
