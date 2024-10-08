/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#pragma once

#include "column.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <utility>

namespace core::db
{
class StatementType
{
  public:
    CORE_API static const StatementType CREATE;
    CORE_API static const StatementType INSERT;
    CORE_API static const StatementType DELETE;
    CORE_API static const StatementType UPDATE;
    CORE_API static const StatementType SELECT;
    CORE_API static const StatementType SELECT_PK;

    [[nodiscard]] const QString& toString() const
    {
        return value;
    }

    bool operator<(const StatementType& other) const
    {
        return value < other.value;
    }

  private:
    QString value;

    explicit StatementType(QString value) : value(std::move(value))
    {
    }
};

template <typename enum_name> class Table
{
  public:
    virtual ~Table() = default;

    auto name() const;

    auto columns() const;

    void create()
    {
        const auto statement = ensureStatementExists(StatementType::CREATE);
        statement->exec(createTable());
    }

    void insert(const QMap<QString, QVariant>& columns)
    {
        const auto statement = ensureStatementExists(StatementType::INSERT, createInsert());
        exec(statement, columns);
    }

    void update(const QMap<QString, QVariant>& columns)
    {
        const auto statement = ensureStatementExists(StatementType::UPDATE, createUpdate());
        exec(statement, columns);
    }

    void deleteRows(const QMap<QString, QVariant>& columns)
    {
        const auto statement = ensureStatementExists(StatementType::DELETE, createDelete());
        exec(statement, columns);
    }

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
    Table(const QSqlDatabase& database, QString name, QMap<QString, Column<enum_name>> columns)
        : m_name(std::move(name)), m_columns(std::move(columns)), m_database(database)
    {
    }

    Table(const QSqlDatabase& database, QString name,
          std::initializer_list<std::pair<QString, Column<enum_name>>> columns)
        : m_name(std::move(name)), m_database(database)
    {
        for (const auto& column : columns)
        {
            m_columns.insert(column.first, column.second);
        }
    }

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

    static void exec(const std::shared_ptr<QSqlQuery>& statement, const QMap<QString, QVariant>& columns)
    {
        for (auto it = columns.cbegin(); it != columns.cend(); ++it)
        {
            statement->bindValue(":" + it.key(), it.value());
        }
        statement->exec();
    }

    [[nodiscard]] virtual QString createTable() const = 0;

    [[nodiscard]] virtual QString createInsert() const = 0;

    [[nodiscard]] virtual QString createUpdate() const = 0;

    [[nodiscard]] virtual QString createSelect() const = 0;

    [[nodiscard]] virtual QString createSelectPk() const = 0;

    [[nodiscard]] virtual QString createDelete() const = 0;

    [[nodiscard]] virtual QString whereClause() const = 0;

    QString m_name;
    QMap<QString, Column<enum_name>> m_columns;
    const QSqlDatabase& m_database;
    QMap<StatementType, std::shared_ptr<QSqlQuery>> m_statements;
};

template <typename enum_name> auto Table<enum_name>::name() const
{
    return m_name;
}

template <typename enum_name> auto Table<enum_name>::columns() const
{
    return m_columns;
}
} // namespace core::db
