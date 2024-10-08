/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#pragma once

#include "include/dllexports.h"
#include <QString>

namespace core::db
{

struct TableIndex
{
    QString m_name;
};

template <typename enum_name> class Column
{
  public:
    CORE_API Column(QString name, enum_name type, bool nullAllowed = true, TableIndex* indexPointer = nullptr)
        : m_name(std::move(name)), m_type(type), m_nullAllowed(nullAllowed), m_index(indexPointer)
    {
    }

    CORE_API auto name()
    {
        return m_name;
    }

    CORE_API auto type()
    {
        return m_type;
    }

    CORE_API auto index()
    {
        return m_index;
    }

    CORE_API auto nullAllowed()
    {
        return m_nullAllowed;
    }

  private:
    QString m_name;
    enum_name m_type;
    bool m_nullAllowed;
    TableIndex* m_index;
};

} // namespace core::db
