/**
 * @file main.cpp
 * @brief main function file for the menu_generator tool.
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

#include "menu_generator.h"

int main(int argc, char *argv[])
{
    const QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Menu Generator");
    QCoreApplication::setApplicationVersion("1.0");

    // Configure the command line parser
    QCommandLineParser parser;
    parser.setApplicationDescription("Generates a QML menus from a simple text file.");
    parser.addHelpOption();
    parser.addVersionOption();

    // Define options
    const QCommandLineOption inputDirectory(QStringList() << "d" << "source-dir",
                                            "Directory containing the source files.", "path/to/files");
    const QCommandLineOption inputFile(QStringList() << "i" << "input-file", "Path to a single source file.",
                                       "path/to/file.menu");
    const QCommandLineOption outputFolder(QStringList() << "o" << "output", "Path to a output directory.",
                                          "path/to/output", ".");
    const QCommandLineOption verboseOption(QStringList() << "verbose", "Enable verbose mode.");

    // Add options to the parser
    parser.addOption(inputDirectory);
    parser.addOption(inputFile);
    parser.addOption(outputFolder);
    parser.addOption(verboseOption);

    // Process arguments
    parser.process(app);

    // Validate required options
    if (!parser.isSet(inputDirectory) && !parser.isSet(inputFile))
    {
        qCritical() << "You must provide the and either a source file or directory.\n";
        parser.showHelp(EXIT_FAILURE);
    }

    // Retrieve options
    const QString outputDirectory = parser.value(outputFolder);
    const bool    verbose         = parser.isSet(verboseOption);

    try
    {
        MenuGenerator generator;
        // Process source files
        if (parser.isSet(inputDirectory))
        {
            // If a source directory is provided
            const QDir sourceDir(parser.value(inputDirectory));

            for (QStringList    sourceFiles = sourceDir.entryList(QStringList() << "*.menu", QDir::Files);
                 const QString &sourceFile: sourceFiles)
            {
                QString filePath = sourceDir.filePath(sourceFile);
                if (verbose)
                {
                    qDebug() << "Processing source file:" << filePath;
                }
                generator.generateMenu(filePath, outputDirectory);
            }
        }
        else if (parser.isSet(inputFile))
        {
            // If a single source file is provided
            const QString sourceFile = parser.value(sourceFile);
            if (verbose)
            {
                qDebug() << "Processing single source file:" << sourceFile;
            }
            generator.generateMenu(sourceFile, outputDirectory);
        }
    }
    catch (const core::FileNotOpen &e)
    {
        qCritical() << "File not open: " << e.what();
    }
    catch (const InvalidSource &e)
    {
        qCritical() << "Invalid syntax: " << e.what();
    }
    catch (const std::exception &e)
    {
        qCritical() << e.what();
    }

    if (verbose)
    {
        qDebug() << "QML generation completed.";
    }

    return 0;
}
