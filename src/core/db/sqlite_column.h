/**
 * @file sqlite_column.h
 * @brief Contains the declaration of the ColumnSQLite class that extends the Column class template with SQLite-specific
 * properties.
 * @copyright Copyright 2024 Manel Jimeno.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "column.h"
#include "dllexports.h"

namespace core::db
{
/**
 * @brief Enumeration of SQLite data types.
 *
 * This enum represents the different SQLite data types that can be used for table columns.
 */
enum class SQLiteDataType
{
    INTEGER,   ///< Represents an INTEGER type in SQLite.
    REAL,      ///< Represents a REAL (floating point) type in SQLite.
    TEXT,      ///< Represents a TEXT type in SQLite.
    BLOB,      ///< Represents a BLOB type in SQLite for binary data.
    NULL_TYPE, ///< Represents a NULL type in SQLite.
    BOOLEAN,   ///< Represents a BOOLEAN type in SQLite.
    DATETIME,  ///< Represents a DATETIME type in SQLite.
};

enum class SQLiteModifier : unsigned int
{
    None = 0,
    isPrimaryKey = 1 << 0,    ///< The column is a primary key.
    isAutoIncrement = 1 << 1, ///< The column auto-increments.
    isUnique = 1 << 2,        ///< The column values must be unique.
    isNotNull = 1 << 3,       ///< The column cannot have null values.
};

// Enable bitwise operations for SQLiteModifier
Q_DECLARE_FLAGS(SQLiteModifiers, SQLiteModifier)
Q_DECLARE_OPERATORS_FOR_FLAGS(SQLiteModifiers)

/**
 * @class SQLiteColumn
 * @brief Represents a column in an SQLite database with SQLite-specific properties.
 * @tparam enum_name The type of the column.
 */
class SQLiteColumn final : public Column<SQLiteDataType>
{
  public:
    /**
     * @brief Constructor for the SQLiteColumn class that accepts modifiers.
     * @param name The name of the column.
     * @param type The type of the column (e.g., INTEGER, TEXT, etc.).
     * @param modifiers A combination of SQLite modifiers (e.g., PRIMARY KEY, AUTOINCREMENT, etc.).
     * @param index Optional index name
     * @param foreignKey Optional foreign key definition.
     * @param checkCondition Optional SQL condition for the CHECK constraint.
     * @param defaultValue Optional default value for the column.
     * @param collate Optional collation sequence for the column.
     */
    CORE_API SQLiteColumn(QString name, const SQLiteDataType type,
                          const SQLiteModifiers modifiers = SQLiteModifier::None, const QString& index = "",
                          QString foreignKey = "", QString checkCondition = "", QString defaultValue = "",
                          QString collate = "")
        : Column<SQLiteDataType>(std::move(name), type, !(modifiers & SQLiteModifier::isNotNull), index),
          m_isPrimaryKey(modifiers & SQLiteModifier::isPrimaryKey),
          m_isAutoIncrement(modifiers & SQLiteModifier::isAutoIncrement),
          m_isUnique(modifiers & SQLiteModifier::isUnique), m_checkCondition(std::move(checkCondition)),
          m_defaultValue(std::move(defaultValue)), m_foreignKey(std::move(foreignKey)), m_collate(std::move(collate))
    {
    }

    // Getters for new SQLite-specific properties

    /**
     * @brief Checks if the column is a primary key.
     * @return True if the column is a primary key, otherwise false.
     */
    [[nodiscard]] CORE_API auto isPrimaryKey() const
    {
        return m_isPrimaryKey;
    }

    /**
     * @brief Checks if the column auto-increments.
     * @return True if the column auto-increments, otherwise false.
     */
    [[nodiscard]] CORE_API auto isAutoIncrement() const
    {
        return m_isAutoIncrement;
    }

    /**
     * @brief Checks if the column values must be unique.
     * @return True if the column is unique, otherwise false.
     */
    [[nodiscard]] CORE_API auto isUnique() const
    {
        return m_isUnique;
    }

    /**
     * @brief Gets the CHECK condition for the column.
     * @return The SQL condition for CHECK constraint.
     */
    [[nodiscard]] CORE_API auto checkCondition() const
    {
        return m_checkCondition;
    }

    /**
     * @brief Gets the default value for the column.
     * @return The default value of the column.
     */
    [[nodiscard]] CORE_API auto defaultValue() const
    {
        return m_defaultValue;
    }

    /**
     * @brief Gets the foreign key definition for the column.
     * @return The foreign key reference.
     */
    [[nodiscard]] CORE_API auto foreignKey() const
    {
        return m_foreignKey;
    }

    /**
     * @brief Gets the collation sequence for the column.
     * @return The collation sequence.
     */
    [[nodiscard]] CORE_API auto collate() const
    {
        return m_collate;
    }

  private:
    bool m_isPrimaryKey;      ///< Indicates if the column is a primary key.
    bool m_isAutoIncrement;   ///< Indicates if the column auto-increments.
    bool m_isUnique;          ///< Indicates if the column values must be unique.
    QString m_checkCondition; ///< Optional CHECK condition for the column.
    QString m_defaultValue;   ///< Optional default value for the column.
    QString m_foreignKey;     ///< Optional foreign key reference.
    QString m_collate;        ///< Optional collation sequence for the column.
};
} // namespace core::db
