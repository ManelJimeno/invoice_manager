/**
 * @file sqlite_column.cpp
 * @brief Implementation file for the ColumnSQLite
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "db/sqlite/sqlite_column.h"

#include "db/sql_builder.h"

#include <QMap>

namespace core::db
{
    QString SQLiteColumn::toSQLiteType(const SQLiteDataType type)
    {
        switch (type)
        {
            case SQLiteDataType::INTEGER:
                return "INTEGER";
            case SQLiteDataType::REAL:
                return "REAL";
            case SQLiteDataType::TEXT:
                return "TEXT";
            case SQLiteDataType::BLOB:
                return "BLOB";
            case SQLiteDataType::DATETIME:
                return "DATETIME";
            default:
                return "TEXT";
        }
    }

    SQLiteColumn::SQLiteDataType SQLiteColumn::fromSQLiteType(const QString &typeStr)
    {
        static const QMap<QString, SQLiteDataType> typeMap = {
                {"INTEGER", SQLiteDataType::INTEGER},   {"REAL", SQLiteDataType::REAL},
                {"TEXT", SQLiteDataType::TEXT},         {"BLOB", SQLiteDataType::BLOB},
                {"DATETIME", SQLiteDataType::DATETIME},
        };

        // Attempt to find the typeStr in the map and return the corresponding enum
        auto it = typeMap.find(typeStr.toUpper().trimmed());
        if (it != typeMap.end())
        {
            return it.value();
        }

        // Default to TEXT if the type is not found
        return SQLiteDataType::TEXT;
    }

    QString SQLiteColumn::dataTypeToCppType(const SQLiteDataType type)
    {
        switch (type)
        {
            case SQLiteDataType::INTEGER:
                return "int"; ///< C++ type for INTEGER
            case SQLiteDataType::REAL:
                return "double"; ///< C++ type for REAL
            case SQLiteDataType::TEXT:
                return "QString"; ///< C++ type for TEXT
            case SQLiteDataType::BLOB:
                return "QByteArray"; ///< C++ type for BLOB
            case SQLiteDataType::NULL_TYPE:
                return "QVariant"; ///< C++ type for NULL
            case SQLiteDataType::BOOLEAN:
                return "bool"; ///< C++ type for BOOLEAN
            case SQLiteDataType::DATETIME:
                return "QDateTime"; ///< C++ type for DATETIME (can also use std::chrono if desired)
            default:
                return "void*"; ///< Fallback for unrecognized types
        }
    }

    SQLiteModifier SQLiteColumn::getModifierMask(const QStringList &modifiers)
    {
        auto mask = static_cast<unsigned int>(SQLiteModifier::None);

        // Map to associate strings with corresponding SQLiteModifier bits
        static const QMap<QString, SQLiteModifier> modifierMap = {
                {"is_primary_key", SQLiteModifier::isPrimaryKey},
                {"is_auto_increment", SQLiteModifier::isAutoIncrement},
                {"is_unique", SQLiteModifier::isUnique},
                {"is_not_null", SQLiteModifier::isNotNull}};

        // Iterate over each modifier string and update the mask
        for (const auto &modifier: modifiers)
        {
            auto it = modifierMap.find(modifier.toLower());
            if (it != modifierMap.end())
            {
                mask |= static_cast<unsigned int>(it.value());
            }
        }

        return static_cast<SQLiteModifier>(mask);
    }

    QString SQLiteColumn::columnDefinition()
    {
        QString definition;

        QVector<std::function<QString()>> modifiers = {
                {[modifier = this->hasModifier(SQLiteModifier::isNotNull)]() { return modifier ? " NOT NULL" : ""; }},
                {[modifier = this->hasModifier(SQLiteModifier::isPrimaryKey)]()
                 { return modifier ? " PRIMARY KEY" : ""; }},
                {[modifier = this->hasModifier(SQLiteModifier::isAutoIncrement)]()
                 { return modifier ? " AUTOINCREMENT" : ""; }},
                {[modifier = this->hasModifier(SQLiteModifier::isUnique)]() { return modifier ? " UNIQUE" : ""; }},
                {[modifier = m_customConstraint.has_value(), this]()
                 { return modifier ? QString(" CHECK(%1)").arg(m_customConstraint.value()) : ""; }},
                {[modifier = m_defaultValue.has_value(), this]()
                 { return modifier ? QString(" DEFAULT %1").arg(m_defaultValue.value()) : ""; }},
                {[modifier = m_customConstraint.has_value(), this]()
                 { return modifier ? QString(" COLLATE %1").arg(m_collate.value()) : ""; }},
        };

        definition += m_columnName + " " + m_dataType;

        for (const auto &modifier: modifiers)
        {
            definition += modifier();
        }

        return definition;
    }

    QString SQLiteColumn::columnToCppType()
    {
        return dataTypeToCppType(fromSQLiteType(m_dataType)) + " m_" + m_columnName + ";\n";
    }

    SQLiteColumn::SQLiteDataType SQLiteColumn::columnType() const
    {
        return m_columnType;
    }

    bool SQLiteColumn::hasModifier(const SQLiteModifiers modifier) const
    {
        return Column::hasModifier(static_cast<unsigned int>(modifier));
    }

} // namespace core::db
