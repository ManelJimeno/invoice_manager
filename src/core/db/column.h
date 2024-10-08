/**
 * @file dllexports.h
 * @brief Contains the declaration of the Column and TableIndex structs and the Column class template.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "include/dllexports.h"
#include <QString>

namespace core::db
{

/**
 * @struct TableIndex
 * @brief Represents an index in a database table.
 */
struct TableIndex
{
    QString m_name; ///< Name of the table index.
};

/**
 * @class Column
 * @brief Template class representing a column in a database.
 * @tparam enum_name The type of the column.
 */
template <typename enum_name> class Column
{
  public:
    /**
     * @brief Constructor for the Column class.
     * @param name The name of the column.
     * @param type The type of the column.
     * @param nullAllowed Indicates if null values are allowed.
     * @param indexPointer Pointer to the TableIndex.
     */
    CORE_API Column(QString name, enum_name type, bool nullAllowed = true, TableIndex* indexPointer = nullptr)
        : m_name(std::move(name)), m_type(type), m_nullAllowed(nullAllowed), m_index(indexPointer)
    {
    }

    /**
     * @brief Gets the name of the column.
     * @return The name of the column.
     */
    CORE_API auto name()
    {
        return m_name;
    }

    /**
     * @brief Gets the type of the column.
     * @return The type of the column.
     */
    CORE_API auto type()
    {
        return m_type;
    }

    /**
     * @brief Gets the index of the column.
     * @return Pointer to the TableIndex.
     */
    CORE_API auto index()
    {
        return m_index;
    }

    /**
     * @brief Checks if null values are allowed.
     * @return True if null values are allowed, otherwise false.
     */
    CORE_API auto nullAllowed()
    {
        return m_nullAllowed;
    }

  private:
    QString m_name;      ///< Name of the column.
    enum_name m_type;    ///< Type of the column.
    bool m_nullAllowed;  ///< Indicates if null values are allowed.
    TableIndex* m_index; ///< Pointer to the TableIndex.
};

} // namespace core::db
