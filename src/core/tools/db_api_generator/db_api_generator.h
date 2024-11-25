/**
 * @file db_api_generator.h
 * @brief Contains the declaration of the DBAPIGenerator class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// Includes QSqlDatabase class to interact with databases
#include <QSqlDatabase>

/**
 * @class DBAPIGenerator
 * @brief Generates C++ classes based on a database structure.
 *
 * The `DBAPIGenerator` class is responsible for generating C++ classes that interact with a database,
 * based on table definitions extracted from a JSON file. The generated classes may include methods
 * for performing SQL operations on the database.
 */
class DBAPIGenerator
{
public:
    /**
     * @brief Constructor for the DBAPIGenerator class.
     *
     * Initializes the DBAPIGenerator with a database connection and an option
     * to enable verbose output during the class generation process.
     *
     * @param database The database (QSqlDatabase) used to fetch table information.
     * @param verbose Enables or disables verbose output. Defaults to false.
     */
    explicit DBAPIGenerator(const QSqlDatabase &database, bool verbose = false);

    /**
     * @brief Parses a JSON file to extract table definitions and generate a C++ class.
     *
     * This method takes a JSON file containing table definitions and generates a corresponding C++ class
     * that includes the necessary SQL operations (such as SELECT, INSERT, etc.).
     *
     * @param filePath The path to the JSON file containing the table definitions.
     * @param outputDirectory The directory where the generated C++ classes will be placed.
     *                        Defaults to the current directory (".").
     */
    void generateClass(const QString &filePath, const QString &outputDirectory = ".") const;

private:
    bool         m_verbose; ///< Flag to enable verbose output during the generation process.
    QSqlDatabase m_database; ///< The database connection used to generate the classes.
};
