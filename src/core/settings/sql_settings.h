/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#pragma once

#include "settings.h"

namespace core::settings
{

template <class table_type> class SQLSettings : public Settings
{
  protected:
    // Constructor
    CORE_API explicit SQLSettings(table_type mTable) : m_table(mTable)
    {
    }

  public:
    CORE_API auto table()
    {
        return m_table;
    };

    CORE_API bool write() override = 0;

    CORE_API bool read() override = 0;

    CORE_API ~SQLSettings() override = default;

  protected:
    table_type m_table;
};

} // namespace core::settings
