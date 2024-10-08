/**
 * @file table.h
 * @brief Header file for the StatementType and Table classes.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "column.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <utility>

namespace core::db
{
/**
 * @class StatementType
 * @brief Defines different SQL statement types (e.g., CREATE, INSERT, DELETE).
 *
 * Provides various SQL statement types, each associated with a string value.
 * This class allows comparing statement types and converting them to strings.
 */
class StatementType
{
  public:
    CORE_API static const StatementType CREATE;    ///< Represents the CREATE statement type.
    CORE_API static const StatementType INSERT;    ///< Represents the INSERT statement type.
    CORE_API static const StatementType DELETE;    ///< Represents the DELETE statement type.
    CORE_API static const StatementType UPDATE;    ///< Represents the UPDATE statement type.
    CORE_API static const StatementType SELECT;    ///< Represents the SELECT statement type.
    CORE_API static const StatementType SELECT_PK; ///< Represents the SELECT_PK statement type.

    /**
     * @brief Converts the StatementType to its string representation.
     * @return A constant reference to the string representing the SQL statement.
     */
    [[nodiscard]] const QString& toString() const
    {
        return value;
    }

    /**
     * @brief Compares this StatementType with another.
     * @param other Another StatementType to compare with.
     * @return True if this StatementType is lexicographically less than the other.
     */
    bool operator<(const StatementType& other) const
    {
        return value < other.value;
    }

  private:
    QString value; ///< @brief Stores the string value of the SQL statement type.

    /**
     * @brief Constructs a StatementType with a specified string value.
     * @param value The string representation of the SQL statement.
     */
    explicit StatementType(QString value) : value(std::move(value))
    {
    }
};

/**
 * @class Table
 * @brief A template class for managing database tables with different SQL operations.
 *
 * This class provides methods for executing SQL statements like CREATE, INSERT, UPDATE,
 * DELETE, and SELECT on database tables. It uses a templated enum for columns.
 *
 * @tparam type_column The table's  columns.
 */
template <typename type_column> class Table
{
  public:
    /**
     * @brief Virtual destructor for Table.
     */
    virtual ~Table() = default;

    /**
     * @brief Retrieves the name of the table.
     * @return The name of the table.
     */
    auto name() const;

    /**
     * @brief Retrieves the columns of the table.
     * @return A map containing the table columns.
     */
    auto columns() const;

    /**
     * @brief Creates the database table.
     * Executes the SQL statement to create the table based on the defined columns.
     */
    void create()
    {
        const auto statement = ensureStatementExists(StatementType::CREATE);
        statement->exec(createTable());
    }

    /**
     * @brief Inserts values into the table.
     * @param columns A map containing the column names and their respective values to insert.
     */
    void insert(const QMap<QString, QVariant>& columns)
    {
        const auto statement = ensureStatementExists(StatementType::INSERT, createInsert());
        exec(statement, columns);
    }

    /**
     * @brief Updates rows in the table.
     * @param columns A map containing the column names and their respective values to update.
     */
    void update(const QMap<QString, QVariant>& columns)
    {
        const auto statement = ensureStatementExists(StatementType::UPDATE, createUpdate());
        exec(statement, columns);
    }

    /**
     * @brief Deletes rows from the table.
     * @param columns A map containing the conditions for deleting rows.
     */
    void deleteRows(const QMap<QString, QVariant>& columns)
    {
        const auto statement = ensureStatementExists(StatementType::DELETE, createDelete());
        exec(statement, columns);
    }

    /**
     * @brief Selects rows using the primary key.
     * @param columns A map containing the primary key values.
     * @return A list of records selected from the table.
     */
    QList<QSqlRecord> selectPk(const QMap<QString, QVariant>& columns)
    {
        const auto statement = ensureStatementExists(StatementType::SELECT_PK, createSelectPk());
        exec(statement, columns);
        QList<QSqlRecord> records;
        while (statement->next())
        {
            records.append(statement->record());
        }
        return records;
    }

    /**
     * @brief Selects all rows from the table.
     * @return A list of records selected from the table.
     */
    QList<QSqlRecord> select()
    {
        const auto statement = ensureStatementExists(StatementType::SELECT, createSelect());
        statement->exec();
        QList<QSqlRecord> records;
        while (statement->next())
        {
            records.append(statement->record());
        }
        return records;
    }

  protected:
    /**
     * @brief Constructs a Table with a given database, name, and columns.
     * @param database The database connection to use.
     * @param name The name of the table.
     * @param columns A map of column names to their respective column objects.
     */
    Table(const QSqlDatabase& database, QString name, QMap<QString, type_column> columns)
        : m_name(std::move(name)), m_columns(std::move(columns)), m_database(database)
    {
    }

    /**
     * @brief Constructs a Table with an initializer list of columns.
     * @param database The database connection to use.
     * @param name The name of the table.
     * @param columns A list of pairs of column names and column objects.
     */
    Table(const QSqlDatabase& database, QString name, std::initializer_list<type_column> columns)
        : m_name(std::move(name)), m_database(database)
    {
        for (const auto& column : columns)
        {
            m_columns.insert(column.name(), column);
        }
    }

    /**
     * @brief Ensures a prepared statement exists for the given SQL operation.
     * @param type The type of SQL statement (e.g., INSERT, UPDATE).
     * @param statement The SQL query to prepare (optional).
     * @return A shared pointer to the prepared SQL query.
     */
    std::shared_ptr<QSqlQuery> ensureStatementExists(const StatementType& type, const QString& statement = "")
    {
        if (m_statements[type] == nullptr)
        {
            m_statements[type] = std::make_shared<QSqlQuery>(m_database);
            if (!statement.isEmpty())
            {
                m_statements[type]->prepare(statement);
            }
        }
        return m_statements[type];
    }

    /**
     * @brief Executes the given SQL statement with bound values.
     * @param statement A shared pointer to the SQL query to execute.
     * @param columns A map of column names and their respective values to bind.
     */
    static void exec(const std::shared_ptr<QSqlQuery>& statement, const QMap<QString, QVariant>& columns)
    {
        for (auto it = columns.cbegin(); it != columns.cend(); ++it)
        {
            statement->bindValue(":" + it.key(), it.value());
        }
        statement->exec();
    }

    [[nodiscard]] virtual QString createTable() const = 0;  ///< @brief Creates the SQL CREATE statement for the table.
    [[nodiscard]] virtual QString createInsert() const = 0; ///< @brief Creates the SQL INSERT statement for the table.
    [[nodiscard]] virtual QString createUpdate() const = 0; ///< @brief Creates the SQL UPDATE statement for the table.
    [[nodiscard]] virtual QString createSelect() const = 0; ///< @brief Creates the SQL SELECT statement for the table.
    [[nodiscard]] virtual QString createSelectPk() const = 0; ///< @brief Creates the SQL SELECT_PK statement.
    [[nodiscard]] virtual QString createDelete() const = 0; ///< @brief Creates the SQL DELETE statement for the table.
    [[nodiscard]] virtual QString whereClause() const = 0;  ///< @brief Creates the SQL WHERE clause for the table.

    QString m_name;                                               ///< @brief The name of the table.
    QMap<QString, type_column> m_columns;                         ///< @brief A map of columns for the table.
    const QSqlDatabase& m_database;                               ///< @brief The database connection used by the table.
    QMap<StatementType, std::shared_ptr<QSqlQuery>> m_statements; ///< @brief Cached SQL statements for the table.
};

template <typename type_column> auto Table<type_column>::name() const
{
    return m_name;
}

template <typename type_column> auto Table<type_column>::columns() const
{
    return m_columns;
}
} // namespace core::db
