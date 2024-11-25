/**
 * @file tools.cpp
 * @brief Implementation file for the tools functions
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "tools.h"
#include <QDir>
#include <QStandardPaths>
#include <QUuid>
#include <exception.h>
#include <qregularexpression.h>

namespace core::tools
{

    QString getTemporaryFileName(const QString &extension)
    {
        const auto tempDir      = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        const auto randomDbName = QUuid::createUuid().toString().remove("{").remove("}").replace("-", "_") + extension;
        return QDir(tempDir).filePath(randomDbName);
    }

    QString capitalizeFirstLetter(const QString &input)
    {
        if (input.isEmpty())
        {
            return input;
        }
        return input.left(1).toUpper() + input.mid(1);
    }

    QString toSnake(const QString &input, const bool lower)
    {
        QString result;
        for (int i = 0; i < input.size(); ++i)
        {
            QChar ch = input[i];
            if (ch.isUpper() && i > 0)
            {
                result += '_';
            }
            if (lower)
            {
                result += ch.toLower();
            }
            else
            {
                result += ch.toUpper();
            }
        }
        return result;
    }

    QString upperSnake(const QString &input)
    {
        return toSnake(input, false);
    }

    QString lowerSnake(const QString &input)
    {
        return toSnake(input, true);
    }

    void saveStringToFile(const QString &text, const QString &filePath)
    {
        QFile file(filePath);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            throw core::FileNotOpen(file.errorString());
        }

        QTextStream out(&file);
        out << text;

        file.close();
    }

    QVector<QString> extractBoundFields(const QString &query)
    {
        static const QRegularExpression re(R"(WHERE\s+(.*?)(?=\s*(GROUP|ORDER|LIMIT|$)))",
                                           QRegularExpression::CaseInsensitiveOption);
        static const QRegularExpression paramRe(R"(:\b(\w+)\b)");

        QVector<QString> boundFields;

        QRegularExpressionMatch match = re.match(query);
        if (match.hasMatch())
        {
            const QString whereClause = match.captured(1);

            QRegularExpressionMatchIterator it = paramRe.globalMatch(whereClause);
            while (it.hasNext())
            {
                QRegularExpressionMatch paramMatch = it.next();
                boundFields << paramMatch.captured(1);
            }
        }

        return boundFields;
    }

    bool areFilesEqual(const QString &filePath1, const QString &filePath2)
    {
        QFile file1(filePath1);
        QFile file2(filePath2);

        // Check if both files can be opened
        if (!file1.open(QIODevice::ReadOnly))
        {
            qWarning() << "Could not open file:" << filePath1;
            return false;
        }
        if (!file2.open(QIODevice::ReadOnly))
        {
            qWarning() << "Could not open file:" << filePath2;
            return false;
        }

        // Compare file sizes
        if (file1.size() != file2.size())
        {
            return false;
        }

        // Compare content in chunks
        while (!file1.atEnd())
        {
            QByteArray chunk1 = file1.read(4096); // Read in 4 KB chunks
            QByteArray chunk2 = file2.read(4096);

            if (chunk1 != chunk2)
            {
                return false; // Differences found
            }
        }

        return true; // Files are identical
    }

} // namespace core::tools
