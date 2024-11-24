/**
 * @file sqlite_column.h
 * @brief Contains the declaration of the SQLiteColumn class, an extension of the Column class with SQLite-specific
 * properties.
 *
 * This file defines the SQLiteColumn class, which adds SQLite-specific functionality to represent
 * individual columns in SQLite tables, including data types, modifiers, collation, and constraints.
 *
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once
#include "db/column.h"

namespace core::db
{

    /**
     * @brief Enumeration of SQLite column modifiers.
     *
     * Defines SQLite-specific modifiers that can be applied to table columns,
     * such as primary key, unique constraint, and auto-increment.
     */
    enum class SQLiteModifier : unsigned int
    {
        None            = 0,
        isPrimaryKey    = 1 << 0, ///< Specifies the column as a primary key.
        isAutoIncrement = 1 << 1, ///< Enables auto-increment for the column.
        isUnique        = 1 << 2, ///< Enforces uniqueness for values in the column.
        isNotNull       = 1 << 3, ///< Prevents null values in the column.
    };

    // Enable bitwise operations for SQLiteModifier
    Q_DECLARE_FLAGS(SQLiteModifiers, SQLiteModifier)
    Q_DECLARE_OPERATORS_FOR_FLAGS(SQLiteModifiers)

    /**
     * @class SQLiteColumn
     * @brief Represents a column in an SQLite database with SQLite-specific properties, such as collation and check
     * constraints.
     *
     * Extends the generic Column class by adding SQLite-specific details such as collation sequences,
     * check constraints, and data types tailored to SQLite.
     */
    class CORE_API SQLiteColumn final : public Column
    {
    public:
        /**
         * @brief Enumeration of SQLite data types.
         *
         * Represents SQLite-specific data types that can be used for table columns.
         */
        enum class SQLiteDataType
        {
            INTEGER, ///< Integer type.
            REAL, ///< Floating-point number type.
            TEXT, ///< Text (string) type.
            BLOB, ///< Binary large object type for storing binary data.
            NULL_TYPE, ///< Represents the NULL type.
            BOOLEAN, ///< Boolean type.
            DATETIME, ///< Date and time type.
        };

        /**
         * @brief Constructs an SQLiteColumn with the specified properties.
         *
         * Initializes an SQLiteColumn with properties such as name, type, modifiers,
         * optional default value, foreign key, check condition, and collation.
         *
         * @param name The column name.
         * @param type The SQLite data type for the column.
         * @param modifiers Bitmask of SQLiteModifiers for properties like PRIMARY KEY, UNIQUE, etc.
         * @param indexName Optional index name for the column.
         * @param defaultValue Optional default value.
         * @param foreignKey Optional foreign key reference.
         * @param checkCondition Optional SQL check constraint.
         * @param collate Optional collation sequence for the column.
         */
        SQLiteColumn(QString name, const SQLiteDataType type,
                     QFlags<SQLiteModifiers::enum_type> modifiers      = SQLiteModifier::None,
                     std::optional<QString>             indexName      = std::nullopt,
                     std::optional<QString>             defaultValue   = std::nullopt,
                     std::optional<QString>             foreignKey     = std::nullopt,
                     std::optional<QString>             checkCondition = std::nullopt,
                     std::optional<QString>             collate        = std::nullopt) :
            Column(std::move(name), toSQLiteType(type), static_cast<unsigned int>(modifiers), std::move(indexName),
                   std::move(defaultValue), std::move(foreignKey), std::move(checkCondition)),
            m_collate(std::move(collate)), m_columnType(type)
        {
        }

        /**
         * @brief Retrieves the collation sequence for the column, if defined.
         *
         * @return An optional QString containing the collation sequence name, or std::nullopt if not set.
         */
        [[nodiscard]] auto collate() const
        {
            return m_collate;
        }

        using Column::hasModifier;

        /**
         * @brief Checks if the column has a specific SQLite modifier.
         *
         * @param modifier The SQLite modifier to check (e.g., PRIMARY KEY, UNIQUE).
         * @return True if the column has the modifier, false otherwise.
         */
        [[nodiscard]] bool hasModifier(SQLiteModifiers modifier) const;

        /**
         * @brief Converts an SQLiteDataType enum value to a corresponding SQL type string.
         *
         * Maps the given SQLiteDataType enum to a string (e.g., INTEGER, TEXT) for use in SQL queries.
         *
         * @param type The SQLiteDataType enum value.
         * @return A QString representing the SQL data type.
         */
        [[nodiscard]] static QString toSQLiteType(SQLiteColumn::SQLiteDataType type);

        /**
         * @brief Converts a SQL type string to its corresponding SQLiteDataType enum value.
         *
         * Maps a SQL data type string (e.g., INTEGER, TEXT) to the corresponding SQLiteDataType enum.
         *
         * @param typeStr The SQL type string.
         * @return The corresponding SQLiteDataType enum.
         */
        [[nodiscard]] static SQLiteDataType fromSQLiteType(const QString &typeStr);

        /**
         * @brief Converts an SQLiteDataType enum to a C++ type string.
         *
         * Returns the C++ data type string (e.g., int, QString) that corresponds to the SQLiteDataType.
         *
         * @param type The SQLiteDataType enum value.
         * @return A QString representing the C++ type.
         */
        [[nodiscard]] static QString dataTypeToCppType(SQLiteDataType type);

        /**
         * @brief Converts a list of modifier strings to a bitmask.
         *
         * Takes a list of modifier strings and returns a bitmask representing those modifiers.
         *
         * @param modifiers A QStringList of modifier names.
         * @return A bitmask of SQLiteModifier values.
         */
        static SQLiteModifier getModifierMask(const QStringList &modifiers);

        /**
         * @brief Generates the SQL definition of the column, including SQLite-specific properties.
         *
         * Constructs the complete SQL column definition with properties such as type, modifiers, and constraints.
         *
         * @return A QString containing the SQL column definition.
         */
        [[nodiscard]] QString columnDefinition() override;

        /**
         * @brief Generates the C++ type definition of the column.
         *
         * Maps the SQLite column type to its equivalent C++ type.
         *
         * @return A QString representing the column type as a C++ type.
         */
        [[nodiscard]] QString columnToCppType() override;

        /**
         * @brief Retrieves the SQLite data type of the column.
         *
         * @return The SQLiteDataType enum value representing the column's type.
         */
        [[nodiscard]] SQLiteDataType columnType() const;

    private:
        std::optional<QString> m_collate; ///< Optional collation sequence specific to SQLite.
        SQLiteDataType         m_columnType; ///< SQLite data type of the column.
    };

} // namespace core::db
