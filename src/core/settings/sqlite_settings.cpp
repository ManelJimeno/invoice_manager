/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#include "sqlite_settings.h"
#include "db/sqlite_table.h"

namespace core::settings
{
using namespace core::db;

SQLiteSettings::SQLiteSettings(const QSqlDatabase& database, QString tableNameParam)
    : SQLSettings(
          core::db::SQLiteTable(database, std::move(tableNameParam),
                                {{"name", Column<SQLiteDataType>("name", SQLiteDataType::TEXT, false, &m_indexName)},
                                 {"value", Column<SQLiteDataType>("value", SQLiteDataType::TEXT, true, nullptr)}}))
{
    m_indexName = {QString("%1_index").arg(m_table.name())};
    m_table.create();
}

bool SQLiteSettings::write()
{
    for (auto it = m_values.cbegin(); it != m_values.cend(); ++it)
    {
        m_table.insert({{"name", it.key()}, {"value", it.value()}});
    }
    return true;
}

bool SQLiteSettings::read()
{
    auto records = m_table.select();
    for (QSqlRecord& record : records)
    {
        m_values[record.value("name").toString()] = record.value("value");
    }
    return true;
}

} // namespace core::settings
