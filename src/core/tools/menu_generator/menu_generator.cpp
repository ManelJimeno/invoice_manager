/**
 * @file menu_generator.cpp
 * @brief Implementation file for the menu_generator tool.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "menu_generator.h"

#include <QDir>
#include <QRegularExpression>

bool MenuGenerator::generateMenu(const QString &inputFile, const QString &outputDirectory)
{
    QFile file(inputFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Error opening input file:" << inputFile;
        return false;
    }

    QList<MenuItem> menuTree;
    parseFile(file, menuTree);

    file.close();
    const QDir      output(outputDirectory);
    const QFileInfo info(file);
    const QString   outputFile = QDir::toNativeSeparators(output.absoluteFilePath(info.baseName() + ".qml"));

    return writeQmlFile(outputFile, menuTree);
}

void MenuGenerator::parseFile(QFile &file, QList<MenuItem> &menuTree)
{
    QTextStream              in(&file);
    const QRegularExpression lineRegex(R"(^(\s*)([^;]+);?([^;]*)?;?(.*)?)"); // Indent, text, function, properties
    QList<long long>         indentStack = {-1}; // Track indent levels for hierarchy
    QList<MenuItem *>        parents     = {nullptr}; // Track parent nodes

    while (!in.atEnd())
    {
        QString                 line  = in.readLine();
        QRegularExpressionMatch match = lineRegex.match(line);

        if (!match.hasMatch())
            continue;

        QString       indent        = match.captured(1);
        const QString text          = match.captured(2).trimmed();
        const QString function      = match.captured(3).trimmed();
        QString       propertiesStr = match.captured(4).trimmed();

        MenuItem item;
        item.text        = text;
        item.onTriggered = function;

        // Parse properties
        for (const QString &prop: propertiesStr.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts))
        {
            QStringList keyValue = prop.split('=');
            if (keyValue.size() == 2)
            {
                item.properties[keyValue[0]] = keyValue[1];
            }
        }

        // Determine parent based on indentation
        const long long currentIndent = indent.size();
        while (currentIndent <= indentStack.last())
        {
            indentStack.pop_back();
            parents.pop_back();
        }

        // Add the current item to the appropriate parent
        if (parents.last())
        {
            parents.last()->subItems.append(item);
        }
        else
        {
            menuTree.append(item);
        }

        // Update stacks
        indentStack.append(currentIndent);
        parents.append(&menuTree.last());
    }
}

bool MenuGenerator::writeQmlFile(const QString &outputFile, const QList<MenuItem> &menuTree)
{
    QFile file(outputFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Error opening output file:" << outputFile;
        return false;
    }

    QTextStream out(&file);
    out << "import QtQuick 2.15\nimport QtQuick.Controls 2.15\n\n";
    out << "MenuBar {\n";

    for (const MenuItem &item: menuTree)
    {
        writeMenuItem(out, item, 1);
    }

    out << "}\n";
    file.close();
    return true;
}

void MenuGenerator::writeMenuItem(QTextStream &out, const MenuItem &item, const int indentLevel)
{
    const QString indent(indentLevel * 4, ' ');

    if (item.text == "-")
    {
        out << indent << "MenuSeparator {}\n";
    }
    else
    {
        if (item.subItems.isEmpty())
        {
            out << indent << "Action {\n";
            out << indent << "    text: \"" << item.text << "\"\n";
            if (!item.onTriggered.isEmpty())
            {
                out << indent << "    onTriggered: " << item.onTriggered << "\n";
            }
        }
        else
        {
            out << indent << "Menu {\n";
            out << indent << "    title: \"" << item.text << "\"\n";
        }

        for (auto it = item.properties.cbegin(); it != item.properties.cend(); ++it)
        {
            out << indent << "    " << it.key() << ": " << it.value() << "\n";
        }

        for (const MenuItem &subItem: item.subItems)
        {
            writeMenuItem(out, subItem, indentLevel + 1);
        }
        out << indent << "}\n";
    }
}
