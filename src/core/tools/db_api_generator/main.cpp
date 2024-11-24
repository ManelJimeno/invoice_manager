/**
 * @file main.cpp
 * @brief main function file for the db_api_generator tool.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <exception.h>
#include "db_api_generator.h"
#include "db_class.h"

int main(int argc, char *argv[])
{
    const QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Database API Generator");
    QCoreApplication::setApplicationVersion("1.0");

    // Configure the command line parser
    QCommandLineParser parser;
    parser.setApplicationDescription("Generates a database access API from JSON files.");
    parser.addHelpOption();
    parser.addVersionOption();

    // Define options
    const QCommandLineOption jsonDirOption(QStringList() << "j" << "json-dir", "Directory containing the JSON files.",
                                           "path/to/json");
    const QCommandLineOption singleJsonOption(QStringList() << "f" << "json-file", "Path to a single JSON file.",
                                              "path/to/file.json");
    const QCommandLineOption outputFolder(QStringList() << "o" << "output", "Path to a output directory.",
                                          "path/to/output", ".");
    const QCommandLineOption dbTypeOption(QStringList() << "t" << "db-type",
                                          "Type of database (e.g., sqlite, mysql, postgres).", "database type");
    const QCommandLineOption connectionInfoOption(QStringList() << "c" << "connection-info",
                                                  "Connection information for the database.", "connection string");
    const QCommandLineOption verboseOption(QStringList() << "verbose", "Enable verbose mode.");

    // Add options to the parser
    parser.addOption(jsonDirOption);
    parser.addOption(singleJsonOption);
    parser.addOption(outputFolder);
    parser.addOption(dbTypeOption);
    parser.addOption(connectionInfoOption);
    parser.addOption(verboseOption);

    // Process arguments
    parser.process(app);

    // Validate required options
    if (!parser.isSet(dbTypeOption) || !parser.isSet(connectionInfoOption) ||
        (!parser.isSet(jsonDirOption) && !parser.isSet(singleJsonOption)))
    {
        qCritical()
                << "You must provide the database type, connection information, and either a JSON file or directory.\n";

        parser.showHelp(EXIT_FAILURE);
    }

    // Retrieve options
    const QString dbType          = parser.value(dbTypeOption);
    const QString connectionInfo  = parser.value(connectionInfoOption);
    const QString outputDirectory = parser.value(outputFolder);
    const bool    verbose         = parser.isSet(verboseOption);

    // Open the database
    auto db = QSqlDatabase::addDatabase(dbType);
    db.setDatabaseName(connectionInfo);
    if (!db.open())
    {
        qCritical() << "I cannot connect to the database " << connectionInfo << "\n" << db.lastError().text();
        exit(-1);
    }

    // Create instance of DBAPIGenerator
    const DBAPIGenerator generator(db, verbose);

    try
    {
        // Process JSON files
        if (parser.isSet(jsonDirOption))
        {
            // If a JSON directory is provided
            const QDir jsonDir(parser.value(jsonDirOption));

            for (QStringList    jsonFiles = jsonDir.entryList(QStringList() << "*.json", QDir::Files);
                 const QString &jsonFile: jsonFiles)
            {
                QString filePath = jsonDir.filePath(jsonFile);
                if (verbose)
                {
                    qDebug() << "Processing JSON file:" << filePath;
                }
                generator.generateClass(filePath, outputDirectory);
            }
        }
        else if (parser.isSet(singleJsonOption))
        {
            // If a single JSON file is provided
            const QString jsonFile = parser.value(singleJsonOption);
            if (verbose)
            {
                qDebug() << "Processing single JSON file:" << jsonFile;
            }
            generator.generateClass(jsonFile, outputDirectory);
        }
    }
    catch (const core::FileNotOpen &e)
    {
        qCritical() << "File not open: " << e.what();
    }
    catch (const InvalidJSON &e)
    {
        qCritical() << "Invalid JSON: " << e.what();
    }
    catch (const core::db::DBManagerException &e)
    {
        qCritical() << e.what();
    }
    catch (const std::exception &e)
    {
        qCritical() << e.what();
    }

    if (verbose)
    {
        qDebug() << "API generation completed.";
    }

    return 0;
}
