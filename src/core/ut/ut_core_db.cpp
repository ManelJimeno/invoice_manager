/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#include <QCoreApplication>
#include <QFile>
#include <QTimer>
#include <gtest/gtest.h>
#include <memory>
#include "db/dynamic_table.h"
#include "db/sqlite/sqlite_column.h"
#include "tools/tools.h"

using namespace core::db;
QSqlDatabase                                                db;
std::unique_ptr<DynamicTable>                               table;
static const std::initializer_list<std::shared_ptr<Column>> settingsColumns = {
        std::make_shared<SQLiteColumn>("name", SQLiteColumn::SQLiteDataType::TEXT,
                                       SQLiteModifier::isNotNull | SQLiteModifier::isUnique |
                                               SQLiteModifier::isPrimaryKey),
        std::make_shared<SQLiteColumn>("value", SQLiteColumn::SQLiteDataType::TEXT)};

TEST(SQLiteTable, create_table_when_not_exists)
{
    table = std::make_unique<core::db::DynamicTable>(db, "TestTable", settingsColumns);
    table->create();
}

TEST(SQLiteTable, insert)
{
    table->insert({{"name", "name_1"}, {"value", "value1"}});
    table->insert({{"name", "name_2"}, {"value", "value2"}});
}

TEST(SQLiteTable, select)
{
    const auto records = table->select();
    EXPECT_EQ(records.size(), 2);
    table.reset();
}

TEST(SQLiteTable, not_create_table_when_exists)
{
    table = std::make_unique<core::db::DynamicTable>(db, "TestTable", settingsColumns);
    table->create();
    const auto records = table->select();
    EXPECT_EQ(records.size(), 2);
}

TEST(SQLiteTable, selectPk)
{
    const auto records = table->selectPk({{"name", "name_1"}});
    EXPECT_EQ(records.size(), 1);
}

TEST(SQLiteTable, update)
{
    table->update({{"name", "name_1"}, {"value", "value2"}});
    auto records = table->selectPk({{"name", "name_1"}});
    EXPECT_GT(records.size(), 0);
    EXPECT_EQ(records[0].value("value"), "value2");
}

TEST(SQLiteTable, deleteRow)
{
    table->deleteRows({{"name", "name_1"}});
    const auto records = table->select();
    EXPECT_EQ(records.size(), 1);
}

int main(int argc, char *argv[])
{
    QCoreApplication app{argc, argv};

    QTimer::singleShot(0,
                       [&]()
                       {
                           const auto dbPath = core::tools::getTemporaryFileName(".db");
                           db                = QSqlDatabase::addDatabase("QSQLITE");
                           db.setDatabaseName(dbPath);

                           ASSERT_TRUE(db.open());

                           ::testing::InitGoogleTest(&argc, argv);
                           const auto testResult = RUN_ALL_TESTS();

                           QFile::remove(dbPath);
                           QCoreApplication::exit(testResult);
                       });

    return QCoreApplication::exec();
}
