/**
 * @file column.cpp
 * @brief Implementation file for the Column class in the database core module.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "column.h"

namespace core::db
{

bool Column::hasModifier(const unsigned int modifier) const
{
    return (m_modifiers & modifier) != 0;
}

std::optional<QString> Column::indexName() const
{
    return m_indexName;
}

std::optional<QString> Column::foreignKey() const
{
    return m_foreignKey;
}

std::optional<QString> Column::defaultValue() const
{
    return m_defaultValue;
}

std::optional<QString> Column::customConstraint() const
{
    return m_customConstraint;
}

void Column::setIndexName(const QString& index)
{
    m_indexName = index;
}

void Column::setForeignKey(const QString& foreignKey)
{
    m_foreignKey = foreignKey;
}

void Column::setDefaultValue(const QString& value)
{
    m_defaultValue = value;
}

void Column::setCustomConstraint(const QString& constraint)
{
    m_customConstraint = constraint;
}

} // namespace core::db
