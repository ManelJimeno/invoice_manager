/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#include "db/sqlite_table.h"
#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>
#include <QTimer>
#include <QUuid>
#include <gtest/gtest.h>
#include <memory>

using namespace core::db;
QSqlDatabase db;
std::unique_ptr<SQLiteTable> table;
static const std::initializer_list<SQLiteColumn> settingsColumns = {
    SQLiteColumn("name", SQLiteDataType::TEXT,
                 SQLiteModifier::isNotNull | SQLiteModifier::isUnique | SQLiteModifier::isPrimaryKey, "users_pk"),
    SQLiteColumn("value", SQLiteDataType::TEXT)};

TEST(SQLiteTable, create_table_when_not_exists)
{
    table = std::make_unique<core::db::SQLiteTable>(db, "TestTable", settingsColumns);
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
    table = std::make_unique<core::db::SQLiteTable>(db, "TestTable", settingsColumns);
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

int main(int argc, char* argv[])
{
    QCoreApplication app{argc, argv};

    QTimer::singleShot(0, [&]() {
        const auto tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        const auto randomDbName = QUuid::createUuid().toString().remove("{").remove("}").replace("-", "_") + ".db";
        const auto dbPath = QDir(tempDir).filePath(randomDbName);
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbPath);

        ASSERT_TRUE(db.open());

        ::testing::InitGoogleTest(&argc, argv);
        const auto testResult = RUN_ALL_TESTS();

        QFile::remove(dbPath);
        QCoreApplication::exit(testResult);
    });

    return QCoreApplication::exec();
}
