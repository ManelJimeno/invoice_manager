/**
 * @file db_class.h
 * @brief Contains the declaration of the DBClass class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// Includes for interacting with SQLite and handling exceptions
#include "db/sqlite/sqlite_builder.h"
#include "db/sqlite/sqlite_column.h"
#include "exception.h"
#include "statement.h"

#include <QDir>
#include <QString>

/**
 * @class InvalidJSON
 * @brief Exception thrown when invalid JSON data is encountered.
 */
class InvalidJSON final : public core::Exception
{
    using Exception::Exception;
};

/**
 * @class DBClass
 * @brief Class representing a database table structure, including its columns,
 *        statements (like SELECT, INSERT, etc.), and methods to handle them.
 *
 * This class is used to generate SQL statements, map them to C++ methods, and
 * write them to source code files. It can parse a JSON representation of a database
 * table and its associated SQL operations and transform that into a corresponding
 * C++ class.
 */
class DBClass final
{
  public:
    // Constants representing JSON key literals for various database elements
    static constexpr auto TABLE = "table";                    ///< Table information in JSON.
    static constexpr auto TABLE_NAME = "name";                ///< Table name in JSON.
    static constexpr auto COLUMNS = "columns";                ///< Columns information in JSON.
    static constexpr auto COLUMN_NAME = "name";               ///< Column name in JSON.
    static constexpr auto COLUMN_TYPE = "type";               ///< Column type in JSON.
    static constexpr auto STATEMENTS = "statements";          ///< SQL statements in JSON.
    static constexpr auto STATEMENT_NAME = "name";            ///< SQL statement name in JSON.
    static constexpr auto STATEMENT_WHERE = "where";          ///< WHERE clause in SQL statements.
    static constexpr auto STATEMENT_TYPE = "type";            ///< SQL statement type (e.g., SELECT, INSERT).
    static constexpr auto MODIFIERS = "modifiers";            ///< Column modifiers (e.g., NOT NULL).
    static constexpr auto INDEX = "index";                    ///< Column index in JSON.
    static constexpr auto FOREIGN_KEY = "foreignKey";         ///< Foreign key reference in JSON.
    static constexpr auto CHECK_CONDITION = "checkCondition"; ///< Check condition for column.
    static constexpr auto DEFAULT_VALUE = "defaultValue";     ///< Default value for column.
    static constexpr auto COLLATE = "collate";                ///< Collation for column.

    // Constants for default SQL statement names
    static constexpr auto DEFAULT_STATEMENT_CREATE = "create";
    static constexpr auto DEFAULT_STATEMENT_INSERT = "insert";
    static constexpr auto DEFAULT_STATEMENT_UPDATE = "update";
    static constexpr auto DEFAULT_STATEMENT_DELETE = "deleteRow";
    static constexpr auto DEFAULT_STATEMENT_SELECT = "selectPk";

    /**
     * @brief Constructor for DBClass.
     *
     * Initializes the class with the given database connection and verbosity flag.
     * The constructor sets up necessary attributes and prepares the class for parsing
     * and generating SQL-related files.
     *
     * @param database The QSqlDatabase object to interact with the database.
     * @param verbose Flag to enable verbose output for debugging.
     */
    explicit DBClass(const QSqlDatabase& database, bool verbose = false);

    /**
     * @brief Destructor.
     *
     * Cleans up any resources or memory allocated by DBClass.
     */
    ~DBClass() = default;

    /**
     * @brief Loads table information from a JSON object.
     *
     * Parses a JSON object representing the table structure, including columns,
     * types, and other table-related information.
     *
     * @param table A QJsonObject containing the table structure.
     */
    void loadTable(const QJsonObject& table);

    /**
     * @brief Loads statements (SQL operations) from a JSON array.
     *
     * Parses a JSON array containing various SQL statements (e.g., SELECT, INSERT)
     * and stores them for later use.
     *
     * @param statements A QJsonArray containing SQL statements.
     */
    void loadStatements(const QJsonArray& statements);

    /**
     * @brief Loads default SQL statements (like SELECT, INSERT, etc.).
     *
     * Loads the default SQL statements such as CREATE, SELECT, INSERT, etc., if
     * not provided by the user.
     */
    void loadDefaultSentences();

    /**
     * @brief Loads a full JSON document and processes it.
     *
     * This method is responsible for loading the entire JSON document, which can
     * include table and statement definitions, and parsing them into the internal
     * data structures.
     *
     * @param document A QJsonDocument containing the full table and statement definitions.
     */
    void load(const QJsonDocument& document);

    /**
     * @brief Gets the header file name for the generated C++ class.
     *
     * Generates and returns the file name for the header file that will contain
     * the class definition.
     *
     * @return The header file name as a QString.
     */
    [[nodiscard]] QString getHeaderFile() const;

    /**
     * @brief Gets the source file name for the generated C++ class.
     *
     * Generates and returns the file name for the source file that will contain
     * the class implementation.
     *
     * @return The source file name as a QString.
     */
    [[nodiscard]] QString getSourceFile() const;

    /**
     * @brief Generates a method for a given SQL statement.
     *
     * This method generates a C++ method based on a provided SQL statement,
     * which can be used to execute the statement in a database.
     *
     * @param statement A shared pointer to a Statement object representing the SQL statement.
     * @return The generated C++ method as a QString.
     */
    [[nodiscard]] QString method(const std::shared_ptr<Statement>& statement) const;

    /**
     * @brief Binds fields to a SQL statement.
     *
     * This method generates the necessary binding code for the fields associated
     * with the given SQL statement (e.g., for prepared statements).
     *
     * @param statement A shared pointer to a Statement object representing the SQL statement.
     * @return The binding code as a std::string.
     */
    [[nodiscard]] std::string getBindFields(const std::shared_ptr<Statement>& statement) const;

    /**
     * @brief Converts record data to fields.
     *
     * This method converts a record object (representing a row of data) into
     * individual field values for use in SQL statements.
     *
     * @param statement A shared pointer to a Statement object representing the SQL statement.
     * @return The field conversion code as a std::string.
     */
    [[nodiscard]] std::string getRecordToFields(const std::shared_ptr<Statement>& statement) const;

    /**
     * @brief Saves the generated C++ files to the specified output folder.
     *
     * This method writes the generated header and source files to the given output folder.
     *
     * @param outputFolder The folder where the generated files will be saved.
     */
    void save(const QString& outputFolder) const;

    /**
     * @brief Creates a Statement object from the given JSON data.
     *
     * This method converts a JSON object representing a SQL statement into a
     * C++ Statement object, which can then be used to execute SQL operations.
     *
     * @param statement A QJsonObject containing the statement data.
     * @return A shared pointer to a Statement object.
     */
    [[nodiscard]] std::shared_ptr<Statement> statementFromJSON(const QJsonObject& statement) const;

    /**
     * @brief Creates a Column object from the given JSON data.
     *
     * This method converts a JSON object representing a column (with its name, type, etc.)
     * into a C++ Column object, which can be used to define the structure of the table.
     *
     * @param column A QJsonObject containing the column data.
     * @return A shared pointer to a Column object.
     */
    [[nodiscard]] static std::shared_ptr<core::db::Column> columnFromJSON(const QJsonObject& column);

  private:
    QDir m_output;                                    ///< Directory where the generated files will be saved.
    bool m_verbose;                                   ///< Flag for enabling verbose output during generation.
    QSqlDatabase m_database;                          ///< The database connection used to fetch table data.
    QString m_className;                              ///< The name of the generated class.
    QVector<std::shared_ptr<Statement>> m_statements; ///< List of SQL statements associated with the class.
    std::shared_ptr<core::db::SQLBuilder> m_builder;  ///< The SQL builder used to generate SQL statements.
};
