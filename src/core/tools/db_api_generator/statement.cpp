/**
 * @file statement.cpp
 * @brief Implementation file for the Statement class in the database core module.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "statement.h"
#include <ranges>
#include "tools/tools.h"

Statement::Statement(QString name, QString sql, const bool isUnique, const SQLTypes type,
                     QVector<QString> whereFields) :
    m_name(std::move(name)), m_type(type), m_whereFields(std::move(whereFields)), m_isUnique(isUnique)
{
    m_sqlVector.append({core::tools::upperSnake(m_name), std::move(sql)});
}

Statement::Statement(QString name, QVector<QString> sqlVector, const SQLTypes type) :
    m_name(std::move(name)), m_type(type), m_isUnique(true)
{
    if (type == SQLTypes::create)
    {
        m_sqlVector.append({core::tools::upperSnake(m_name), std::move(sqlVector.front())});
        for (int i = 1; i < sqlVector.size(); i++)
        {
            m_sqlVector.append({QString("CREATE_INDEX_%1").arg(i), std::move(sqlVector[i])});
        }
    }
    else
    {
        int index = 1;
        for (auto &sql: sqlVector)
        {
            m_sqlVector.append({core::tools::upperSnake(QString("SENTENCE_%1").arg(index++)), std::move(sql)});
        }
    }
}

QString Statement::name() const
{
    return m_name;
}

QString Statement::sql() const
{
    return m_sqlVector.at(0).second;
}

Statement::SQLTypes Statement::type() const
{
    return m_type;
}

QVector<QString> Statement::whereFields() const
{
    return m_whereFields;
}

QString Statement::signature() const
{
    if (m_type == SQLTypes::create)
    {
        return QString("void %1();\n").arg(m_name);
    }

    switch (m_type)
    {
        case SQLTypes::select:
        {
            QString signature = QString("bool %1(Record& record);\n").arg(m_name);
            if (!m_isUnique)
            {
                signature += QString("bool next%1(Record& record);\n").arg(core::tools::capitalizeFirstLetter(m_name));
            }
            return signature;
        }
        case SQLTypes::count:
            return QString("long long %1();\n").arg(m_name);
    }
    return QString("void %1(Record& record);\n").arg(m_name);
}

QString Statement::sentences() const
{
    QString sentences;
    for (const auto &[key, value]: m_sqlVector)
    {
        sentences += QString("const QString %1 = \"%2\";\n").arg(key, value);
    }
    return sentences;
}

QString Statement::sqlQuery() const
{
    return QString("QSqlQuery m_%1;\n").arg(m_name);
}

QString Statement::attributes() const
{
    QString attributes;
    const auto &[key, value] = m_sqlVector.at(0);
    attributes += QString("m_%1(m_database)").arg(m_name);
    return attributes;
}

QString Statement::prepare() const
{
    QString attributes;
    if (m_type != SQLTypes::create)
    {
        const auto &[key, value] = m_sqlVector.at(0);
        attributes += QString("m_%1.prepare(%2);\n").arg(m_name, key);
    }
    return attributes;
}

int Statement::sqlSize() const
{
    return m_sqlVector.size();
}

QString Statement::defines() const
{
    QStringList elements;
    for (const auto &first: m_sqlVector | std::views::keys)
    { // Structured binding to get 'first' directly
        elements << first;
    }
    return elements.join(',');
}

bool Statement::isUnique() const
{
    return m_isUnique;
}
