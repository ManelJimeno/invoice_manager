/**
 * @file dynamic_table.cpp
 * @brief Defines the SQL statement types for the core::db module.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "dynamic_table.h"

#include "factory.h"

#include <QSqlError>
#include <QSqlQuery>

namespace core::db
{

    DynamicTable::DynamicTable(const QSqlDatabase &database, QString name,
                               const std::initializer_list<std::shared_ptr<db::Column>> columns) :
        m_database(database), m_name(std::move(name)), m_builder(Factory::builder(m_database.driverName()))
    {
        m_builder->setTableName(m_name);
        for (const auto &column: columns)
        {
            m_builder->addColumn(column);
        }
        m_sentences[DynamicTable::CREATE]    = m_builder->createTable();
        m_sentences[DynamicTable::INSERT]    = m_builder->createInsert();
        m_sentences[DynamicTable::UPDATE]    = m_builder->createUpdate();
        m_sentences[DynamicTable::DELETE]    = m_builder->createDelete();
        m_sentences[DynamicTable::SELECT]    = m_builder->createSelect();
        m_sentences[DynamicTable::SELECT_PK] = m_builder->createSelectPk();
    }

    std::shared_ptr<QSqlQuery> DynamicTable::ensureStatementExists(const QString &name, const QString &statement)
    {
        if (m_statements[name] == nullptr)
        {
            m_statements[name] = std::make_shared<QSqlQuery>(m_database);
            if (!statement.isEmpty())
            {
                m_statements[name]->prepare(statement);
            }
        }
        return m_statements[name];
    }

    void DynamicTable::create()
    {
        const auto statement = ensureStatementExists(DynamicTable::CREATE);
        if (!statement->exec(m_sentences[DynamicTable::CREATE]))
        {
            throw SQLError(statement->lastError().text());
        }
    }

    void DynamicTable::insert(const QMap<QString, QVariant> &columns)
    {
        const auto statement = ensureStatementExists(DynamicTable::INSERT, m_sentences[DynamicTable::INSERT]);
        exec(statement, columns);
    }

    void DynamicTable::update(const QMap<QString, QVariant> &columns)
    {
        const auto statement = ensureStatementExists(DynamicTable::UPDATE, m_sentences[DynamicTable::UPDATE]);
        exec(statement, columns);
    }

    void DynamicTable::deleteRows(const QMap<QString, QVariant> &columns)
    {
        const auto statement = ensureStatementExists(DynamicTable::DELETE, m_sentences[DynamicTable::DELETE]);
        exec(statement, columns);
    }

    QList<QSqlRecord> DynamicTable::select()
    {
        const auto statement = ensureStatementExists(DynamicTable::SELECT, m_sentences[DynamicTable::SELECT]);
        if (!statement->exec())
        {
            throw SQLError(statement->lastError().text());
        }
        QList<QSqlRecord> records;
        while (statement->next())
        {
            records.append(statement->record());
        }
        return records;
    }

    QList<QSqlRecord> DynamicTable::selectPk(const QMap<QString, QVariant> &columns)
    {
        const auto statement = ensureStatementExists(DynamicTable::SELECT_PK, m_sentences[DynamicTable::SELECT_PK]);
        exec(statement, columns);
        QList<QSqlRecord> records;
        while (statement->next())
        {
            records.append(statement->record());
        }
        return records;
    }

    void DynamicTable::exec(const std::shared_ptr<QSqlQuery> &statement, const QMap<QString, QVariant> &columns)
    {
        for (auto it = columns.cbegin(); it != columns.cend(); ++it)
        {
            statement->bindValue(":" + it.key(), it.value());
        }
        if (!statement->exec())
        {
            throw SQLError(statement->lastError().text());
        }
    }

} // namespace core::db
