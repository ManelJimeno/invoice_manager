/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#include "settings/sqlite_settings.h"
#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>
#include <QTimer>
#include <QUuid>
#include <gtest/gtest.h>

QSqlDatabase db;

TEST(SQLiteSettings, set_value)
{
    core::settings::SQLiteSettings settings(db, "GoogleTest");

    settings["value_1"] = "hi";
    settings["value_2"] = 3;

    settings.write();
}

TEST(SQLiteSettings, get_value)
{
    core::settings::SQLiteSettings settings(db, "GoogleTest");

    settings.read();

    EXPECT_EQ(settings["value_1"].toString(), "hi");
    EXPECT_EQ(settings["value_2"].toInt(), 3);
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
