/**
 * @file db_class.h
 * @brief Contains the declaration of the DBClass class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "db/sqlite/sqlite_builder.h"
#include "db/sqlite/sqlite_column.h"
#include "exception.h"
#include "statement.h"

#include <QDir>
#include <QString>

class InvalidJSON final : public core::Exception
{
    using Exception::Exception;
};

class DBClass final
{
  public:
    static constexpr auto TABLE = "table";                    ///< Represents the literal of the table info.
    static constexpr auto TABLE_NAME = "name";                ///< Represents the literal of the table name.
    static constexpr auto COLUMNS = "columns";                ///< Represents the literal of the column's info.
    static constexpr auto COLUMN_NAME = "name";               ///< Represents the literal of the column name.
    static constexpr auto COLUMN_TYPE = "type";               ///< Represents the literal of the column type.
    static constexpr auto STATEMENTS = "statements";          ///< Represents the literal of statements.
    static constexpr auto STATEMENT_NAME = "name";            ///< Represents the literal of statement name.
    static constexpr auto STATEMENT_WHERE = "where";          ///< Represents the literal of where sql.
    static constexpr auto STATEMENT_TYPE = "type";            ///< Represents the literal of type statement.
    static constexpr auto MODIFIERS = "modifiers";            ///< Represents the literal of the column modifiers.
    static constexpr auto INDEX = "index";                    ///< Represents the literal of the column index.
    static constexpr auto FOREIGN_KEY = "foreignKey";         ///< Represents the literal of the column foreign key.
    static constexpr auto CHECK_CONDITION = "checkCondition"; ///< Represents the literal of the column check condition.
    static constexpr auto DEFAULT_VALUE = "defaultValue";     ///< Represents the literal of the column default value.
    static constexpr auto COLLATE = "collate";                ///< Represents the literal of the column collate.

    static constexpr auto DEFAULT_STATEMENT_CREATE = "create";
    static constexpr auto DEFAULT_STATEMENT_INSERT = "insert";
    static constexpr auto DEFAULT_STATEMENT_UPDATE = "update";
    static constexpr auto DEFAULT_STATEMENT_DELETE = "deleteRow";
    static constexpr auto DEFAULT_STATEMENT_SELECT = "selectPk";

    /**
     * Constructor
     */
    explicit DBClass(const QSqlDatabase& database, bool verbose = false);
    /**
     * Destructor
     */
    ~DBClass() = default;

    void loadTable(const QJsonObject& table);
    void loadStatements(const QJsonArray& statements);
    void loadDefaultSentences();
    void load(const QJsonDocument& document);
    [[nodiscard]] QString getHeaderFile() const;
    [[nodiscard]] QString getSourceFile() const;
    [[nodiscard]] QString method(const std::shared_ptr<Statement>& statement) const;
    [[nodiscard]] std::string getBindFields(const std::shared_ptr<Statement>& statement) const;
    [[nodiscard]] std::string getRecordToFields(const std::shared_ptr<Statement>& statement) const;
    void save(const QString& outputFolder) const;

    /**
     * @brief Creates a statement object from the JSON info.
     * @param statement A QJsonObject containing statement information.
     * @return A shared pointer to a Statement object.
     */
    [[nodiscard]] std::shared_ptr<Statement> statementFromJSON(const QJsonObject& statement) const;

    /**
     * @brief Creates a column object from the JSON info.
     * @param column A QJsonObject containing column information.
     * @return A shared pointer to a Column object.
     */
    [[nodiscard]] static std::shared_ptr<core::db::Column> columnFromJSON(const QJsonObject& column);

  private:
    QDir m_output;
    bool m_verbose;          ///< Enable verbose output.
    QSqlDatabase m_database; ///< Database
    QString m_className;
    QVector<std::shared_ptr<Statement>> m_statements;
    std::shared_ptr<core::db::SQLBuilder> m_builder; ///< The SQL builder adapted to the QSqlDatabase::driverName().
};
