/**
 * @file sqlite_column.h
 * @brief Contains the declaration of the SQLiteColumn class that extends the Column class with SQLite-specific
 * properties.
 * @copyright Copyright 2024 Manel Jimeno.
 * @date 2024
 * @author Manel Jimeno
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once
#include "db/column.h"

namespace core::db
{

/**
 * @brief Enumeration of SQLite column modifier.
 *
 * This enum represents the different SQLite modifiers that can be used for table columns.
 */
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
 * @brief Represents a column in an SQLite database with SQLite-specific properties, such as collation and check
 * constraints.
 *
 * This class extends the generic Column class, adding properties specific to SQLite databases.
 * It includes fields for collation sequence, check constraints, and other SQLite-only modifiers.
 */
class CORE_API SQLiteColumn final : public Column
{
  public:
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

    /**
     * @brief Constructs an SQLiteColumn with given name, type, modifiers, and optional properties.
     *
     * @param name The name of the column.
     * @param type The data type of the column (e.g., INTEGER, TEXT).
     * @param modifiers A bitmask of SQLite-specific modifiers (e.g., PRIMARY KEY, AUTOINCREMENT).
     * @param indexName Optional index name for the column.
     * @param defaultValue Optional default value for the column.
     * @param foreignKey Optional foreign key reference for the column.
     * @param checkCondition Optional SQL condition for a CHECK constraint.
     * @param collate Optional collation sequence name for the column.
     */
    SQLiteColumn(QString name, const SQLiteDataType type,
                 QFlags<SQLiteModifiers::enum_type> modifiers = SQLiteModifier::None,
                 std::optional<QString> indexName = std::nullopt, std::optional<QString> defaultValue = std::nullopt,
                 std::optional<QString> foreignKey = std::nullopt, std::optional<QString> checkCondition = std::nullopt,
                 std::optional<QString> collate = std::nullopt)
        : Column(std::move(name), toSQLiteType(type), static_cast<unsigned int>(modifiers), std::move(indexName),
                 std::move(defaultValue), std::move(foreignKey), std::move(checkCondition)),
          m_collate(std::move(collate)), m_columnType(type)
    {
    }

    /**
     * @brief Retrieves the collation sequence name for the column, if set.
     * @return The optional collation sequence name, or nullopt if not set.
     */
    [[nodiscard]] auto collate() const
    {
        return m_collate;
    }

    /**
     * @brief Checks if the column has a specific modifier.
     * @param modifier The modifier to check.
     * @return True if the modifier is present, false otherwise.
     */
    [[nodiscard]] bool hasModifier(SQLiteModifiers modifier) const;
    /**
     * @brief Converts a SQLiteDataType enum value to its corresponding SQLite type string.
     *
     * This static method translates a SQLiteDataType enum into a string representation
     * that can be used in SQL queries (e.g., INTEGER, REAL, TEXT).
     *
     * @param type The SQLiteDataType enum value.
     * @return A QString representing the SQL type.
     */
    [[nodiscard]] static QString toSQLiteType(SQLiteColumn::SQLiteDataType type);
    /**
     * @brief Converts a SQLite type string to its corresponding SQLiteDataType enum value.
     *
     * This static method translates a SQLite type string into a SQLiteDataType enum
     *
     * @param typeStr The SQLiteDataType enum value.
     * @return A QString representing the SQL type.
     */
    [[nodiscard]] static SQLiteDataType fromSQLiteType(const QString& typeStr);

    /**
     * @brief Converts an SQLite data type to its corresponding C++ type.
     *
     * This function takes an SQLiteDataType enumeration value and returns
     * a string representation of the corresponding C++ data type.
     *
     * @param type The SQLite data type to convert.
     * @return A string representing the equivalent C++ type.
     */
    [[nodiscard]] static QString dataTypeToCppType(SQLiteDataType type);
    /**
     * @brief Converts a set of strings in a mask.
     *
     * @param modifiers The SQLiteDataType enum value.
     * @return A mask with the modifiers.
     */
    static SQLiteModifier getModifierMask(const QStringList& modifiers);

    /**
     * @brief Generates the full SQL definition of the column, including SQLite-specific properties.
     * @return A QString containing the SQL column definition.
     */
    [[nodiscard]] QString columnDefinition() override;
    /**
     * @brief Gets the column definition to compose the class.
     * @return The definition of the column on cpp type.
     */
    [[nodiscard]] QString columnToCppType() override;

    SQLiteDataType columnType() const;

  private:
    std::optional<QString> m_collate; ///< Optional collation sequence for the column, specific to SQLite.
    SQLiteDataType m_columnType;
};

} // namespace core::db
