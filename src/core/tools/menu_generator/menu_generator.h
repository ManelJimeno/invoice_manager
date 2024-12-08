/**
 * @file menu_generator.h
 * @brief Contains the declaration of the MenuGenerator class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include <QFile>
#include <QRegularExpression>
#include <exception.h>

/**
 * @class InvalidSource
 * @brief Exception thrown when invalid source data is encountered during menu generation.
 *
 * This exception is used to indicate issues with the source file provided for
 * generating a menu structure.
 */
class InvalidSource final : public core::Exception
{
    using Exception::Exception;
};

/**
 * @class MenuGenerator
 * @brief Provides functionality for generating a menu structure from a source file and outputting it as QML.
 *
 * The `MenuGenerator` class is responsible for parsing a source file, constructing a hierarchical
 * menu structure, and generating the corresponding QML file. It includes support for nested menu
 * items and their properties.
 */
class MenuGenerator
{
public:
    /**
     * @struct MenuItem
     * @brief Represents a single menu item in the menu structure.
     *
     * A `MenuItem` contains properties for its display text, an associated action, additional
     * attributes, and any sub-items that form part of a hierarchical menu.
     */
    struct MenuItem
    {
        QString                text; ///< The display text for the menu item.
        QString                onTriggered; ///< The action triggered when the menu item is selected.
        QMap<QString, QString> properties; ///< Additional properties of the menu item (e.g., shortcuts, roles).
        QList<MenuItem>        subItems; ///< List of nested sub-items for hierarchical menus.
    };

    /**
     * @brief Generates a menu from an input file and writes the output as QML files.
     *
     * This static method reads a source file, parses its contents into a menu structure, and
     * writes the resulting menu hierarchy into the specified output directory as QML.
     *
     * @param inputFile The path to the input file containing the menu definitions.
     * @param outputDirectory The directory where the generated QML file will be written.
     * @return `true` if the menu was successfully generated and written; `false` otherwise.
     */
    static bool generateMenu(const QString &inputFile, const QString &outputDirectory);

private:
    /**
     * @brief Parses the contents of a source file to construct a menu tree.
     *
     * This static method reads and processes the given file, constructing a hierarchical
     * menu tree from its contents.
     *
     * @param file A reference to the file to parse.
     * @param menuTree A reference to the list of `MenuItem` objects representing the menu structure.
     * @throws InvalidSource if the file contains invalid data.
     */
    static void parseFile(QFile &file, QList<MenuItem> &menuTree);

    /**
     * @brief Writes the generated menu tree to a QML file.
     *
     * This static method takes a menu tree and outputs it to a QML file, preserving the
     * hierarchical structure.
     *
     * @param outputFile The path to the output QML file.
     * @param menuTree A list of `MenuItem` objects representing the menu hierarchy.
     * @return `true` if the QML file was successfully written; `false` otherwise.
     */
    static bool writeQmlFile(const QString &outputFile, const QList<MenuItem> &menuTree);

    /**
     * @brief Writes a single menu item and its children to the QML output stream.
     *
     * This static method recursively writes a `MenuItem` and its sub-items to a QML file,
     * properly formatted and indented.
     *
     * @param out The output stream to write to.
     * @param item The `MenuItem` to write.
     * @param indentLevel The current indentation level for formatting the QML.
     */
    static void writeMenuItem(QTextStream &out, const MenuItem &item, int indentLevel);
};
