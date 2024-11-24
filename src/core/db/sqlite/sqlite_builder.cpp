/**
 * @file sqlite_builder.cpp
 * @brief Contains the implementation of the TableMetaInfo class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "sqlite_builder.h"
#include "db/factory.h"

#include <QtCore/QJsonArray>

namespace core::db
{

    std::optional<std::shared_ptr<core::db::Column>> SQLiteBuilder::column(const QString &columnName)
    {
        for (const auto &column: m_columns)
        {
            if (column && column->columnName() == columnName)
            {
                return column;
            }
        }
        return std::nullopt;
    }

    QString SQLiteBuilder::createTable() const
    {
        QString foreignKeyClause;
        QString query = "CREATE TABLE IF NOT EXISTS " + name() + " ( ";
        for (auto it = m_columns.begin(); it != m_columns.end(); ++it)
        {
            query += (*it)->columnDefinition();
            if (std::next(it) != m_columns.end())
            {
                query += ", ";
            }
            if ((*it)->foreignKey().has_value())
            {
                if (!foreignKeyClause.isEmpty())
                {
                    foreignKeyClause += ", ";
                }
                foreignKeyClause += QString("FOREIGN KEY (%1) REFERENCES").arg((*it)->foreignKey().value());
            }
        }
        if (!foreignKeyClause.isEmpty())
        {
            query += ", " + foreignKeyClause;
        }
        query += " );";
        return query;
    }

    QVector<QString> SQLiteBuilder::createIndexes() const
    {
        QVector<QString>                    queries;
        std::map<QString, QVector<QString>> indexes;
        for (const auto &column: m_columns)
        {
            if (column->indexName().has_value())
            {
                indexes[column->indexName().value()].push_back(column->columnName());
            }
        }

        for (const auto &[indexName, fields]: indexes)
        {
            QString query = "CREATE INDEX " + indexName + " ON " + m_tableName.toLower() + "(";
            for (auto it = fields.begin(); it != fields.end(); ++it)
            {
                query += (*it);
                if (std::next(it) != fields.end())
                {
                    query += ", ";
                }
            }
            query += ");";
            queries.append(query);
        }
        return queries;
    }

    QString SQLiteBuilder::createInsert() const
    {
        QString     query = "INSERT INTO " + m_tableName + " (";
        QStringList columnsList;
        QStringList valuesList;
        for (const auto &column: m_columns)
        {
            columnsList << column->columnName();
            valuesList << ":" + column->columnName();
        }
        query += columnsList.join(", ") + ") VALUES (" + valuesList.join(", ") + ");";
        return query;
    }

    QString SQLiteBuilder::createUpdate() const
    {
        QString     query = "UPDATE " + m_tableName + " SET ";
        QStringList setList;
        QStringList whereList;
        for (const auto &column: m_columns)
        {
            setList << column->columnName() + "=:" + column->columnName();
            if (column->indexName() != std::nullopt)
            {
                whereList << column->columnName() + "=:" + column->columnName();
            }
        }
        query += setList.join(", ");
        query += whereClause() + ";";

        return query;
    }

    QString SQLiteBuilder::createDelete() const
    {
        return "DELETE FROM " + m_tableName + whereClause() + ";";
    }

    QString SQLiteBuilder::createSelectPk() const
    {
        return createSelect() + whereClause() + ";";
    }

    QString SQLiteBuilder::createSelect() const
    {
        return "SELECT * FROM " + m_tableName;
    }

    QString SQLiteBuilder::whereClause() const
    {
        QStringList whereList;

        for (const auto &item: m_columns)
        {
            auto column = std::dynamic_pointer_cast<SQLiteColumn>(item);
            if (column->hasModifier(SQLiteModifier::isPrimaryKey))
            {
                whereList << column->columnName() + "=:" + column->columnName();
            }
        }
        if (whereList.empty())
        {
            for (const auto &column: m_columns)
            {
                if (column->indexName().has_value())
                {
                    whereList << column->columnName() + "=:" + column->columnName();
                }
            }
        }

        if (!whereList.empty())
        {
            return " WHERE " + whereList.join(" and ");
        }

        return {};
    }

} // namespace core::db
