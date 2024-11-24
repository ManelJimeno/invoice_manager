/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#include "db/db_manager.h"
#include "db/factory.h"
#include "db_class.h"
#include "tools/tools.h"

#include <QDir>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QTimer>
#include <QtCore/QJsonArray>

#include "tools/db_api_generator/db_api_generator.h"

#include <QCommandLineParser>
#include <gtest/gtest.h>
using namespace core::db;
QSqlDatabase db;
QDir         dataDirectory;
bool         checkFiles = false;

void saveJsonToFile(const QString &filePath)
{
    QJsonObject tableObj{
            {"name", "Users"},
            {"columns",
             QJsonArray{QJsonObject{{"name", "id"},
                                    {"type", "INTEGER"},
                                    {"modifiers", QJsonArray{"is_primary_key", "is_unique", "is_auto_increment"}}

                        },
                        QJsonObject{{"name", "username"},
                                    {"type", "TEXT"},
                                    {"index", "users_pk"},
                                    {"modifiers", QJsonArray{"is_unique"}}},
                        QJsonObject{
                                {"name", "email"},
                                {"type", "TEXT"},
                        },
                        QJsonObject{
                                {"name", "created_at"},
                                {"type", "DATETIME"},
                        }}}};

    const QJsonObject findUserByIdQuery{{"name", "findUserById"}, {"where", "id = :id"}, {"type", "select"}};
    const QJsonObject findUserByUsername{
            {"name", "findUserByUsername"}, {"where", "username = :username"}, {"type", "select"}};
    const QJsonObject findUserByEmail{{"name", "findUserByEmail"}, {"where", "email = :email"}, {"type", "select"}};

    const QJsonObject root{{"table", tableObj},
                           {"statements", QJsonArray{findUserByIdQuery, findUserByUsername, findUserByEmail}}};

    const QJsonDocument jsonDoc(root);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning("Cannot open file for writing.");
        return;
    }

    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();
}

TEST(DBAPIGenerator, create_db_api_generator)
{
    const auto tempDir    = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    const auto randomName = QUuid::createUuid().toString().remove("{").remove("}").replace("-", "_");
    const auto path       = QDir(tempDir).filePath(randomName);

    saveJsonToFile(path + ".json");

    try
    {
        const QDir    output(".");
        const QString headerFileName = QDir::toNativeSeparators(output.absoluteFilePath("users.h"));
        const QString sourceFileName = QDir::toNativeSeparators(output.absoluteFilePath("users.cpp"));
        GTEST_LOG_(INFO) << "Generating files: \n"
                         << headerFileName.toStdString() << "\n"
                         << sourceFileName.toStdString() << std::endl;

        const DBAPIGenerator generator(db, true);
        generator.generateClass(path + ".json");

        if (checkFiles)
        {
            const QString cppGenerated = QDir(".").filePath("users.cpp");
            const QString cppStored    = dataDirectory.filePath("users.cpp");
            EXPECT_EQ(core::tools::areFilesEqual(cppGenerated, cppStored), true);
            const QString hGenerated = QDir(".").filePath("users.h");
            const QString hStored    = dataDirectory.filePath("users.h");
            EXPECT_EQ(core::tools::areFilesEqual(hGenerated, hStored), true);
        }
    }
    catch (const core::FileNotOpen &e)
    {
        GTEST_LOG_(ERROR) << "File not open: " << e.what() << std::endl;
    }
    catch (const InvalidJSON &e)
    {
        GTEST_LOG_(ERROR) << "Invalid JSON: " << e.what() << std::endl;
    }
    catch (const core::db::DBManagerException &e)
    {
        GTEST_LOG_(ERROR) << "Cannot connect to the database: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        GTEST_LOG_(ERROR) << e.what() << std::endl;
    }

    QFile::remove(path + ".json");
    QFile::remove(path + ".db");
}

int main(int argc, char *argv[])
{
    QCoreApplication   app{argc, argv};
    QCommandLineParser parser;

    const QCommandLineOption sourceFolder(QStringList() << "s" << "source-folder",
                                          "Directory containing the source code to compare.", "path/to/code");
    const QCommandLineOption compareFiles(QStringList() << "compare-files",
                                          "Compare the resulting files with the stored files.");
    parser.addOption(sourceFolder);
    parser.addOption(compareFiles);
    parser.process(app);

    if (!parser.isSet(sourceFolder))
    {
        dataDirectory = QDir("./data");
    }
    else
    {
        dataDirectory = QDir(parser.value(sourceFolder));
    }

    checkFiles = parser.isSet(compareFiles);

    QTimer::singleShot(0,
                       [&]()
                       {
                           const auto dbPath = core::tools::getTemporaryFileName(".db");
                           db                = QSqlDatabase::addDatabase("QSQLITE");
                           db.setDatabaseName(dbPath);

                           ASSERT_TRUE(db.open());

                           ::testing::InitGoogleTest(&argc, argv);
                           const auto testResult = RUN_ALL_TESTS();
                           QCoreApplication::exit(testResult);
                       });

    return QCoreApplication::exec();
}
