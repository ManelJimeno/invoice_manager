/**
 * @file db_class.cpp
 * @brief Implementation file for the DBClass class in the db_api_generator tool.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "db_class.h"
#include <QProcess>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <ranges>
#include "db/factory.h"
#include "fmt/args.h"
#include "source_template.cpp"
#include "tools/tools.h"

DBClass::DBClass(const QSqlDatabase &database, const bool verbose) :
    m_verbose(verbose), m_database(database), m_builder(core::db::Factory::builder(m_database.driverName()))
{
}

void DBClass::save(const QString &outputFolder) const
{
    const QDir    output(outputFolder);
    const QString fileName       = core::tools::lowerSnake(m_className);
    const QString headerFileName = QDir::toNativeSeparators(output.absoluteFilePath(fileName + ".h"));
    const QString sourceFileName = QDir::toNativeSeparators(output.absoluteFilePath(fileName + ".cpp"));
    core::tools::saveStringToFile(getHeaderFile(), headerFileName);
    core::tools::saveStringToFile(getSourceFile(), sourceFileName);
    auto runFormat = [&](const QString &fileToFormat)
    {
        QProcess process;
        process.start("clang-format", {"-i", fileToFormat});
        const auto result = process.waitForFinished();
        if (!result)
        {
            qWarning() << process.errorString();
        }
        return result;
    };
    runFormat(headerFileName);
    runFormat(sourceFileName);
}

void DBClass::loadTable(const QJsonObject &table)
{
    m_className = core::tools::capitalizeFirstLetter(table[DBClass::TABLE_NAME].toString());
    m_builder->setTableName(m_className.toLower());
    if (m_verbose)
    {
        qDebug() << "Table name:" << m_builder->name();
    }
    for (const auto &value: table[DBClass::COLUMNS].toArray())
    {
        auto column = value.toObject();
        m_builder->addColumn(columnFromJSON(column));
        if (m_verbose)
        {
            qDebug() << "Parsed table definition:" << column;
        }
    }
}

void DBClass::loadStatements(const QJsonArray &statements)
{
    loadDefaultSentences();
    for (const auto &statement: statements)
    {
        auto object = statement.toObject();
        m_statements.push_back(statementFromJSON(object));
        if (m_verbose)
        {
            qDebug() << "Parsed SQL statement definition:" << object;
        }
    }
}

void DBClass::loadDefaultSentences()
{
    QVector<QString> createTable;
    createTable.append(m_builder->createTable());
    createTable.append(m_builder->createIndexes());

    m_statements.push_back(std::make_shared<Statement>(DEFAULT_STATEMENT_CREATE, createTable));
    m_statements.push_back(std::make_shared<Statement>(DEFAULT_STATEMENT_INSERT, m_builder->createInsert(), true,
                                                       Statement::SQLTypes::insert));
    m_statements.push_back(std::make_shared<Statement>(DEFAULT_STATEMENT_UPDATE, m_builder->createUpdate(), true,
                                                       Statement::SQLTypes::update));
    m_statements.push_back(std::make_shared<Statement>(DEFAULT_STATEMENT_DELETE, m_builder->createDelete(), true,
                                                       Statement::SQLTypes::deleteRow));
    m_statements.push_back(std::make_shared<Statement>(DEFAULT_STATEMENT_SELECT, m_builder->createSelectPk(), true,
                                                       Statement::SQLTypes::select));
    m_statements.push_back(std::make_shared<Statement>(DEFAULT_STATEMENT_COUNT, m_builder->createSelectCount(), true,
                                                       Statement::SQLTypes::count));
}

void DBClass::load(const QJsonDocument &document)
{
    // Validate that the document is a JSON object
    if (!document.isObject())
    {
        throw InvalidJSON("Expected JSON object at root level.");
    }

    const QJsonObject rootObj = document.object();

    // Validate required top-level arrays in JSON structure

    if (!(rootObj.contains(DBClass::TABLE) && rootObj[DBClass::TABLE].isObject()))
    {
        throw InvalidJSON("JSON must contain 'TABLE' arrays.");
    }

    // Access the 'TABLE' object and validate required keys
    const QJsonObject tableObject = rootObj[DBClass::TABLE].toObject();

    for (const auto &key: {DBClass::TABLE_NAME, DBClass::COLUMNS})
    {
        if (!tableObject.contains(key))
        {
            throw InvalidJSON(QString("Missing required key in 'TABLE': %1").arg(key));
        }
    }

    loadTable(tableObject);

    if (rootObj.contains(DBClass::STATEMENTS) && rootObj[DBClass::STATEMENTS].isArray())
    {
        loadStatements(rootObj[DBClass::STATEMENTS].toArray());
    }
    else
    {
        loadDefaultSentences();
    }
}

QString DBClass::getHeaderFile() const
{
    const std::string recordStruct =
            std::accumulate(m_builder->columns().begin(), m_builder->columns().end(), std::string{},
                            [](const std::string &acc, const std::shared_ptr<core::db::Column> &column)
                            { return acc + column->columnToCppType().toStdString(); });
    const std::string signatures =
            std::accumulate(m_statements.begin(), m_statements.end(), std::string{},
                            [](const std::string &acc, const std::shared_ptr<Statement> &statement)
                            { return acc + statement->signature().toStdString(); });

    const std::string sentences =
            std::accumulate(m_statements.begin(), m_statements.end(), std::string{},
                            [&](const std::string &acc, const std::shared_ptr<Statement> &statement)
                            { return acc + statement->sentences().toStdString(); });

    const std::string sqlQuery =
            std::accumulate(m_statements.begin(), m_statements.end(), std::string{},
                            [&](const std::string &acc, const std::shared_ptr<Statement> &statement)
                            { return acc + statement->sqlQuery().toStdString(); });

    fmt::dynamic_format_arg_store<fmt::format_context> headerArgs;
    headerArgs.push_back(fmt::arg("header_parent_class_name", m_builder->headerParentClass().toStdString()));
    headerArgs.push_back(fmt::arg("table_name", m_builder->name().toStdString()));
    headerArgs.push_back(fmt::arg("class_name", m_className.toStdString()));
    headerArgs.push_back(fmt::arg("parent_class_name", m_builder->parentClass().toStdString()));
    headerArgs.push_back(fmt::arg("record", recordStruct));
    headerArgs.push_back(fmt::arg("public_signatures", signatures));
    headerArgs.push_back(fmt::arg("sentences", sentences));
    headerArgs.push_back(fmt::arg("sql_query", sqlQuery));

    const auto headerInput  = getHeaderTemplate();
    const auto headerOutput = fmt::vformat(headerInput, headerArgs);
    return headerOutput.c_str();
}

QString DBClass::getSourceFile() const
{
    const std::string attributes =
            std::accumulate(m_statements.begin(), m_statements.end(), std::string{},
                            [&](const std::string &acc, const std::shared_ptr<Statement> &statement)
                            {
                                const std::string separator = (&statement == &m_statements.back()) ? "" : ",";
                                return acc + statement->attributes().toStdString() + separator;
                            });
    const std::string prepare             = std::accumulate(m_statements.begin(), m_statements.end(), std::string{},
                                                            [&](const std::string &acc, const std::shared_ptr<Statement> &statement)
                                                            { return acc + statement->prepare().toStdString(); });
    const std::string createSentencesSize = fmt::format("{}", m_statements.at(0)->sqlSize());
    const std::string createSentences     = m_statements.at(0)->defines().toStdString();
    const std::string classMethods =
            std::accumulate(m_statements.begin() + 1, m_statements.end(), std::string{},
                            [&](const std::string &acc, const std::shared_ptr<Statement> &statement)
                            { return acc + method(statement).toStdString(); });

    fmt::dynamic_format_arg_store<fmt::format_context> sourceArguments;
    sourceArguments.push_back(fmt::arg("table_name", m_builder->name().toStdString()));
    sourceArguments.push_back(fmt::arg("class_name", m_className.toStdString()));
    sourceArguments.push_back(fmt::arg("parent_class_name", m_builder->parentClass().toStdString()));
    sourceArguments.push_back(fmt::arg("attributes", attributes));
    sourceArguments.push_back(fmt::arg("prepare", prepare));
    sourceArguments.push_back(fmt::arg("create_sentences_size", createSentencesSize));
    sourceArguments.push_back(fmt::arg("create_sentences", createSentences));
    sourceArguments.push_back(fmt::arg("class_methods", classMethods));

    const auto sourceInput  = getSourceTemplate();
    const auto sourceOutput = fmt::vformat(sourceInput, sourceArguments);
    return sourceOutput.c_str();
}

std::string DBClass::getAutoincrement(const std::shared_ptr<Statement> &shared) const
{
    std::string autoincrement;
    for (const auto &item: m_builder->columns())
    {
        const auto column = std::dynamic_pointer_cast<core::db::SQLiteColumn>(item);
        if (column->hasModifier(core::db::SQLiteModifier::isAutoIncrement))
        {
            autoincrement = QString("record.m_%1 = getLastInsertRowId();").arg(column->columnName()).toStdString();
        }
    }
    return autoincrement;
}

std::string DBClass::getBindFields(const std::shared_ptr<Statement> &statement) const
{
    auto getBindField = [&](const QString &statementName, const QString &columnName) -> std::string
    {
        auto name   = statementName.toStdString();
        auto column = columnName.toStdString();
        return fmt::format("m_{}.bindValue(\":{}\", record.m_{});", name, column, column);
    };

    switch (statement->type())
    {
        case Statement::SQLTypes::create:
            break;
        case Statement::SQLTypes::insert:
            return std::accumulate(
                    m_builder->columns().begin(), m_builder->columns().end(), std::string{},
                    [&](const std::string &acc, const std::shared_ptr<core::db::Column> &column)
                    {
                        if (column->hasModifier(static_cast<unsigned int>(core::db::SQLiteModifier::isAutoIncrement)) ||
                            column->defaultValue() != std::nullopt)
                        {
                            return acc;
                        }
                        return acc + getBindField(statement->name(), column->columnName());
                    });
        case Statement::SQLTypes::select:
        case Statement::SQLTypes::count:
        case Statement::SQLTypes::update:
        case Statement::SQLTypes::deleteRow:
            auto list = statement->whereFields();
            return std::accumulate(list.begin(), list.end(), std::string{},
                                   [&](const std::string &acc, const QString &columnName)
                                   { return acc + getBindField(statement->name(), columnName); });
    }
    return {};
}

std::string DBClass::getRecordToFields(const std::shared_ptr<Statement> &statement) const
{
    std::string result;
    for (const auto &item: m_builder->columns())
    {
        const auto column = std::dynamic_pointer_cast<core::db::SQLiteColumn>(item);
        auto       name   = column->columnName().toStdString();

        switch (column->columnType())
        {
            case core::db::SQLiteColumn::SQLiteDataType::INTEGER:
                result += fmt::format("record.m_{} = sqlRecord.value(\"{}\").toLongLong();\n", name, name);
                break;
            case core::db::SQLiteColumn::SQLiteDataType::REAL:
                result += fmt::format("record.m_{} = sqlRecord.value(\"{}\").toDouble();\n", name, name);
                break;
            case core::db::SQLiteColumn::SQLiteDataType::BLOB:
                result += fmt::format("record.m_{} = sqlRecord.value(\"{}\").toByteArray();\n", name, name);
                break;
            case core::db::SQLiteColumn::SQLiteDataType::BOOLEAN:
                result += fmt::format("record.m_{} = sqlRecord.value(\"{}\").toBool();\n", name, name);
                break;
            case core::db::SQLiteColumn::SQLiteDataType::DATETIME:
                result += fmt::format("record.m_{} = sqlRecord.value(\"{}\").toDateTime();\n", name, name);
                break;
            default:
                result += fmt::format("record.m_{} = sqlRecord.value(\"{}\").toString();\n", name, name);
                break;
        }
    }
    return result;
}

QString DBClass::method(const std::shared_ptr<Statement> &statement) const
{
    const char       *sourceInput          = nullptr;
    const std::string recordToFields       = getRecordToFields(statement);
    const auto        sqlQuery             = QString("m_%1").arg(statement->name());
    const std::string recordToBind         = getBindFields(statement);
    const std::string recoverAutoincrement = getAutoincrement(statement);

    fmt::dynamic_format_arg_store<fmt::format_context> sourceArguments;
    sourceArguments.push_back(fmt::arg("class_name", m_className.toStdString()));
    sourceArguments.push_back(fmt::arg("method_name", statement->name().toStdString()));
    sourceArguments.push_back(fmt::arg("record_to_bind", recordToBind));
    sourceArguments.push_back(fmt::arg("recover_autoincrement", recoverAutoincrement));
    sourceArguments.push_back(fmt::arg("sql_query", sqlQuery.toStdString()));
    if (statement->type() == Statement::SQLTypes::select)
    {
        sourceArguments.push_back(fmt::arg("record_to_structure", recordToFields));
        if (statement->isUnique())
        {
            sourceInput = getUniqueSelectMethod();
        }
        else
        {
            sourceArguments.push_back(fmt::arg("capitalized_method_name",
                                               core::tools::capitalizeFirstLetter(statement->name()).toStdString()));
            sourceInput = getSelectMethod();
        }
    }
    else
    {
        switch (statement->type())
        {
            case Statement::SQLTypes::count:
                sourceInput = getSelectCount();
                break;
            case Statement::SQLTypes::insert:
                sourceInput = getInsertMethod();
                break;
            default:
                sourceInput = getNoSelectMethod();
                break;
        }
    }
    const auto sourceOutput = fmt::vformat(sourceInput, sourceArguments);

    return sourceOutput.c_str();
}

std::shared_ptr<core::db::Column> DBClass::columnFromJSON(const QJsonObject &column)
{
    // Checking mandatory fields
    for (const auto &key: {DBClass::COLUMN_NAME, DBClass::COLUMN_TYPE})
    {
        if (!column.contains(key))
        {
            throw InvalidJSON(QString("Missing required key in 'COLUMN': %1").arg(key));
        }
    }

    // Extract and initialize column properties with default values
    const auto columnName = column[DBClass::COLUMN_NAME].toString();
    const auto type       = core::db::SQLiteColumn::fromSQLiteType(column[DBClass::COLUMN_TYPE].toString());

    auto modifiers = core::db::SQLiteModifier::None;

    // Lambda to extract optional fields efficiently
    auto extractOptional = [&column](const QString &key) -> std::optional<QString>
    { return column.contains(key) ? std::optional<QString>(column[key].toString()) : std::nullopt; };

    // Initialize optional properties in one step
    const auto index          = extractOptional(DBClass::INDEX);
    const auto foreignKey     = extractOptional(DBClass::FOREIGN_KEY);
    const auto checkCondition = extractOptional(DBClass::CHECK_CONDITION);
    const auto defaultValue   = extractOptional(DBClass::DEFAULT_VALUE);
    const auto collate        = extractOptional(DBClass::COLLATE);

    // Process modifiers if they exist
    if (const auto modifiersArray = column[DBClass::MODIFIERS].toArray(); !modifiersArray.isEmpty())
    {
        QStringList modifiersList;
        modifiersList.reserve(modifiersArray.size());

        // Transform JSON array directly into the list of modifiers
        std::ranges::transform(modifiersArray, std::back_inserter(modifiersList),
                               [](const QJsonValue &value) { return value.toString(); });

        modifiers = core::db::SQLiteColumn::getModifierMask(modifiersList);
    }

    // Create and return the Column shared pointer
    return std::make_shared<core::db::SQLiteColumn>(columnName, type, modifiers, index, defaultValue, foreignKey,
                                                    checkCondition, collate);
}

std::shared_ptr<Statement> DBClass::statementFromJSON(const QJsonObject &statement) const
{
    auto       name  = statement[DBClass::STATEMENT_NAME].toString();
    const auto where = statement[DBClass::STATEMENT_WHERE].toString();
    const auto type  = statement[DBClass::STATEMENT_TYPE].toString();

    if (type == "select")
    {
        QString sql = "select * from " + m_builder->name() + " ";
        if (!where.isEmpty())
        {
            sql += " where " + where;
        }
        auto whereFields = core::tools::extractBoundFields(sql);
        bool isUnique    = false;
        for (const auto &columnName: whereFields)
        {
            auto column = m_builder->column(columnName);
            if (column.has_value() && column.value()->hasModifier(core::db::SQLiteModifier::isUnique |
                                                                  core::db::SQLiteModifier::isPrimaryKey))
            {
                isUnique = true;
            }
        }
        return std::make_shared<Statement>(name, sql, isUnique, Statement::SQLTypes::select, whereFields);
    }

    return nullptr;
}
