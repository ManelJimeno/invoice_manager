/**
 * @file db_api_generator.h
 * @brief Contains the declaration of the DBAPIGenerator class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include <QSqlDatabase>

class DBAPIGenerator
{
  public:
    /**
     * @brief Constructor for DBAPIGenerator.
     * @param database The database.
     * @param verbose Enables verbose output.
     */
    explicit DBAPIGenerator(const QSqlDatabase& database, bool verbose = false);

    /**
     * @brief Parse a JSON file to extract table definitions and SQL statements to a C++ class.
     * @param filePath The path to the JSON file.
     * @param outputDirectory The output folder of the generated classes
     */
    void generateClass(const QString& filePath, const QString& outputDirectory = ".") const;

  private:
    [[nodiscard]] std::string tableName() const;
    [[nodiscard]] std::string className() const;
    [[nodiscard]] std::string Record() const;
    [[nodiscard]] static std::string NoSelectFunctions();
    [[nodiscard]] std::string SelectFunctions() const;
    [[nodiscard]] std::string NoSelectSentences() const;
    [[nodiscard]] std::string SelectSentences() const;
    [[nodiscard]] std::string NoSelectSqlQuery() const;
    [[nodiscard]] std::string SelectSqlQuery() const;
    [[nodiscard]] std::string classMethods() const;

    bool m_verbose;          ///< Enable verbose output.
    QSqlDatabase m_database; ///< Database
};
