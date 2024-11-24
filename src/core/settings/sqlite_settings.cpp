/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#include "sqlite_settings.h"
#include "db/sqlite/sqlite_column.h"

namespace core::settings
{
    using namespace core::db;

    static const std::initializer_list<std::shared_ptr<db::Column>> settingsColumns = {
            std::make_shared<SQLiteColumn>("name", SQLiteColumn::SQLiteDataType::TEXT,
                                           SQLiteModifier::isNotNull | SQLiteModifier::isUnique |
                                                   SQLiteModifier::isPrimaryKey),
            std::make_shared<SQLiteColumn>("value", SQLiteColumn::SQLiteDataType::TEXT)};

    SQLiteSettings::SQLiteSettings(const QSqlDatabase &database, QString name) :
        SQLSettings(database, std::move(name), settingsColumns)
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
        for (QSqlRecord &record: records)
        {
            m_values[record.value("name").toString()] = record.value("value");
        }
        return true;
    }
} // namespace core::settings
