/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#pragma once

#include "db/sqlite_table.h"
#include "sql_settings.h"

namespace core::settings
{

class SQLiteSettings final : public SQLSettings<core::db::SQLiteTable>
{
  public:
    // Constructor
    CORE_API SQLiteSettings(const QSqlDatabase& database, const QString& tableNameParam);

    CORE_API ~SQLiteSettings() override = default;

    CORE_API bool write() override;

    CORE_API bool read() override;
};

} // namespace core::settings
