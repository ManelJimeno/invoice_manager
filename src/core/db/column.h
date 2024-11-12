/**
 * @file column.h
 * @brief Contains the declaration of the Column class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "dllexports.h"
#include <QVector>

namespace core::db
{

/**
 * @class Column
 * @brief Represents a basic database column with a name and data type.
 *
 * This class provides the fundamental properties of a database column,
 * specifically its name and data type. It serves as a base class for
 * more complex column types that may include additional properties.
 */
class CORE_API Column
{
  public:
    /**
     * @brief Constructor for the Column class.
     * @param name The name of the column.
     * @param type The data type of the column.
     * @param modifiers The modifiers for the column (default is 0).
     * @param indexName Optional index name
     * @param foreignKey Optional foreign key definition.
     * @param checkCondition Optional SQL condition for the CHECK constraint.
     * @param defaultValue Optional default value for the column.
     */
    Column(QString name, QString type, const unsigned int modifiers = 0,
           std::optional<QString> indexName = std::nullopt, std::optional<QString> defaultValue = std::nullopt,
           std::optional<QString> foreignKey = std::nullopt, std::optional<QString> checkCondition = std::nullopt)
        : m_columnName(std::move(name)), m_dataType(std::move(type)), m_modifiers(modifiers),
          m_indexName(std::move(indexName)), m_foreignKey(std::move(foreignKey)),
          m_defaultValue(std::move(defaultValue)), m_customConstraint(std::move(checkCondition)) {};
    /**
     * @brief Destructor
     */
    virtual ~Column() = default;
    /**
     * @brief Gets the name of the column.
     * @return The name of the column.
     */
    [[nodiscard]] QString columnName() const
    {
        return m_columnName;
    }
    /**
     * @brief Gets the data type of the column.
     * @return The data type of the column.
     */
    [[nodiscard]] QString dataType() const
    {
        return m_dataType;
    }
    /**
     * @brief Gets the column definition to compose the "create table" statement.
     * @return The definition of the column.
     */
    [[nodiscard]] virtual QString columnDefinition() = 0;
    /**
     * @brief Gets the column definition to compose the class.
     * @return The definition of the column on cpp type.
     */
    [[nodiscard]] virtual QString columnToCppType() = 0;
    /**
     * @brief Checks if the column has a specific modifier.
     * @param modifier The modifier to check.
     * @return True if the modifier is present, false otherwise.
     */
    [[nodiscard]] virtual bool hasModifier(unsigned int modifier) const;

    // Accessors for optional properties
    /**
     * @brief Gets the name of the index, if any.
     * @return An optional containing the index name, or nullopt if not set.
     */
    [[nodiscard]] std::optional<QString> indexName() const;
    /**
     * @brief Gets the foreign key constraint, if any.
     * @return An optional containing the foreign key, or nullopt if not set.
     */
    [[nodiscard]] virtual std::optional<QString> foreignKey() const;
    /**
     * @brief Gets the default value for the column, if any.
     * @return An optional containing the default value, or nullopt if not set.
     */
    [[nodiscard]] virtual std::optional<QString> defaultValue() const;
    /**
     * @brief Gets the custom constraint (e.g., unique), if any.
     * @return An optional containing the custom constraint, or nullopt if not set.
     */
    [[nodiscard]] std::optional<QString> customConstraint() const;

    // Modifiers for optional properties

    /**
     * @brief Sets the name of the index.
     * @param index The name of the index to set.
     */
    void setIndexName(const QString& index);
    /**
     * @brief Sets the foreign key constraint.
     * @param foreignKey The foreign key constraint to set.
     */
    void setForeignKey(const QString& foreignKey);
    /**
     * @brief Sets the default value for the column.
     * @param value The default value to set.
     */
    void setDefaultValue(const QString& value);
    /**
     * @brief Sets a custom constraint (e.g., unique).
     * @param constraint The custom constraint to set.
     */
    void setCustomConstraint(const QString& constraint);

  protected:
    QString m_columnName;     ///< Name of the column.
    QString m_dataType;       ///< Data type of the column.
    unsigned int m_modifiers; ///< Modifiers for the column, managed as a bitmask.

    // Optional properties
    std::optional<QString> m_indexName;        ///< Name of the index, if any.
    std::optional<QString> m_foreignKey;       ///< Foreign key constraint, if any.
                                               ///< foreign key clause.
    std::optional<QString> m_defaultValue;     ///< Default value for the column, if any.
    std::optional<QString> m_customConstraint; ///< Custom constraint (unique, etc).
};

} // namespace core::db
