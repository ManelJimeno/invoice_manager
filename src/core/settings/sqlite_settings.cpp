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

static const std::initializer_list<SQLiteColumn> settingsColumns = {
    SQLiteColumn("name", SQLiteDataType::TEXT,
                 SQLiteModifier::isNotNull | SQLiteModifier::isUnique | SQLiteModifier::isPrimaryKey, "users_pk"),
    SQLiteColumn("value", SQLiteDataType::TEXT)};

SQLiteSettings::SQLiteSettings(const QSqlDatabase& database, const QString& tableNameParam)
    : SQLSettings(core::db::SQLiteTable(database, tableNameParam, settingsColumns))
{
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
