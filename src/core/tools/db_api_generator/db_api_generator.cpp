/**
 * @file db_api_generator.cpp
 * @brief Implementation file for the db_api_generator tool.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "db_api_generator.h"
#include <QJsonDocument>
#include "db_class.h"

DBAPIGenerator::DBAPIGenerator(const QSqlDatabase &database, const bool verbose) :
    m_verbose(verbose), m_database(database)
{
}

void DBAPIGenerator::generateClass(const QString &filePath, const QString &outputDirectory) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw core::FileNotOpen(file.errorString());
    }

    DBClass dbClass(m_database, m_verbose);
    dbClass.load(QJsonDocument::fromJson(file.readAll()));
    dbClass.save(outputDirectory);

    if (m_verbose)
    {
        qDebug() << "Parsed JSON file:" << filePath;
    }
}
