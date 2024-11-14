/**
 * @file column.h
 * @brief Contains the declaration of the Column class.
 *
 * This file defines the Column class, a foundational abstraction for database columns,
 * which includes core properties such as name, data type, and optional constraints.
 * It serves as a base for more specialized column classes with database-specific requirements.
 *
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
 * @brief Represents a basic database column with a name, data type, and modifiers.
 *
 * This class encapsulates essential column properties, including its name, data type,
 * and optional properties like constraints and default values. It provides a base class
 * for defining more complex column types with additional behavior or SQL-specific features.
 */
class CORE_API Column
{
  public:
    /**
     * @brief Constructor for the Column class.
     *
     * Initializes a Column with a specified name, type, and optional parameters
     * for modifiers, index, foreign key, and constraints.
     *
     * @param name The name of the column.
     * @param type The data type of the column as a string.
     * @param modifiers Bitmask representing column modifiers (default is 0).
     * @param indexName Optional index name for indexing the column.
     * @param foreignKey Optional foreign key definition for referential integrity.
     * @param checkCondition Optional SQL condition for a CHECK constraint.
     * @param defaultValue Optional default value for the column.
     */
    Column(QString name, QString type, const unsigned int modifiers = 0,
           std::optional<QString> indexName = std::nullopt, std::optional<QString> defaultValue = std::nullopt,
           std::optional<QString> foreignKey = std::nullopt, std::optional<QString> checkCondition = std::nullopt)
        : m_columnName(std::move(name)), m_dataType(std::move(type)), m_modifiers(modifiers),
          m_indexName(std::move(indexName)), m_foreignKey(std::move(foreignKey)),
          m_defaultValue(std::move(defaultValue)), m_customConstraint(std::move(checkCondition)) {};

    /**
     * @brief Virtual destructor for the Column class.
     */
    virtual ~Column() = default;

    /**
     * @brief Retrieves the name of the column.
     *
     * @return A QString containing the column's name.
     */
    [[nodiscard]] QString columnName() const
    {
        return m_columnName;
    }

    /**
     * @brief Retrieves the data type of the column.
     *
     * @return A QString containing the column's data type.
     */
    [[nodiscard]] QString dataType() const
    {
        return m_dataType;
    }

    /**
     * @brief Constructs the SQL column definition for the "CREATE TABLE" statement.
     *
     * This is a pure virtual method, to be implemented by derived classes.
     *
     * @return A QString representing the SQL definition of the column.
     */
    [[nodiscard]] virtual QString columnDefinition() = 0;

    /**
     * @brief Retrieves the C++ type that represents the column.
     *
     * This is a pure virtual method, to be implemented by derived classes, which converts
     * the SQL data type to an equivalent C++ type representation.
     *
     * @return A QString representing the column's type as a C++ type.
     */
    [[nodiscard]] virtual QString columnToCppType() = 0;

    /**
     * @brief Checks if the column has a specific modifier.
     *
     * This virtual method evaluates if a specified bitmask modifier is set.
     *
     * @param modifier The bitmask modifier to check.
     * @return True if the column has the specified modifier, false otherwise.
     */
    [[nodiscard]] virtual bool hasModifier(unsigned int modifier) const;

    // Accessors for optional properties

    /**
     * @brief Retrieves the index name, if set.
     *
     * @return An optional QString containing the index name, or std::nullopt if not defined.
     */
    [[nodiscard]] std::optional<QString> indexName() const;

    /**
     * @brief Retrieves the foreign key constraint, if defined.
     *
     * @return An optional QString containing the foreign key, or std::nullopt if not defined.
     */
    [[nodiscard]] virtual std::optional<QString> foreignKey() const;

    /**
     * @brief Retrieves the default value for the column, if set.
     *
     * @return An optional QString containing the default value, or std::nullopt if not defined.
     */
    [[nodiscard]] virtual std::optional<QString> defaultValue() const;

    /**
     * @brief Retrieves any custom constraint applied to the column, if set.
     *
     * Examples of constraints might include UNIQUE or CHECK constraints.
     * @return An optional QString containing the custom constraint, or std::nullopt if not defined.
     */
    [[nodiscard]] std::optional<QString> customConstraint() const;

    // Modifiers for optional properties

    /**
     * @brief Sets the name of the index for the column.
     *
     * @param index The name of the index.
     */
    void setIndexName(const QString& index);

    /**
     * @brief Sets the foreign key constraint for the column.
     *
     * @param foreignKey The foreign key constraint.
     */
    void setForeignKey(const QString& foreignKey);

    /**
     * @brief Sets the default value for the column.
     *
     * @param value The default value for the column.
     */
    void setDefaultValue(const QString& value);

    /**
     * @brief Sets a custom constraint for the column.
     *
     * @param constraint The custom constraint, such as UNIQUE or CHECK.
     */
    void setCustomConstraint(const QString& constraint);

  protected:
    QString m_columnName;     ///< The name of the column.
    QString m_dataType;       ///< The SQL data type of the column.
    unsigned int m_modifiers; ///< Bitmask representing column modifiers.

    // Optional properties
    std::optional<QString> m_indexName;        ///< Optional index name for the column.
    std::optional<QString> m_foreignKey;       ///< Optional foreign key constraint.
    std::optional<QString> m_defaultValue;     ///< Optional default value for the column.
    std::optional<QString> m_customConstraint; ///< Optional custom constraint (e.g., UNIQUE, CHECK).
};

} // namespace core::db
