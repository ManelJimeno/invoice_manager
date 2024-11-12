/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#pragma once

#include "db/dynamic_table.h"
#include "settings.h"
#include <QSqlDatabase>

namespace core::settings
{

class CORE_API SQLSettings : public Settings
{
  protected:
    // Constructor
    explicit SQLSettings(const QSqlDatabase& database, QString name,
                         const std::initializer_list<std::shared_ptr<db::Column>> columns)
        : m_table(database, std::move(name), columns)
    {
    }

  public:
    auto table()
    {
        return m_table;
    };

    bool write() override = 0;

    bool read() override = 0;

    ~SQLSettings() override = default;

  protected:
    db::DynamicTable m_table;
};

} // namespace core::settings
