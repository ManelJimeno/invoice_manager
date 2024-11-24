/**
 * @file statement.h
 * @brief Contains the declaration of the Statement class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once
#include <QVector>
#include "dllexports.h"

/**
 * @class Statement
 * @brief Represents a SQL statement with its type, name, SQL query, and relevant fields.
 *
 * The `Statement` class is used to represent different types of SQL statements such as
 * SELECT, INSERT, UPDATE, DELETE, and CREATE. It encapsulates the SQL query, its
 * associated fields, and other metadata needed for executing the query or generating code.
 */
class Statement
{
public:
    /**
     * @enum SQLTypes
     * @brief Represents the types of SQL statements that can be defined.
     *
     * The SQLTypes enum provides a set of predefined values representing the different
     * types of SQL operations that the Statement class can handle.
     */
    enum class SQLTypes
    {
        select, ///< Represents a SELECT SQL statement.
        update, ///< Represents an UPDATE SQL statement.
        insert, ///< Represents an INSERT SQL statement.
        create, ///< Represents a CREATE SQL statement.
        deleteRow, ///< Represents a DELETE SQL statement.
    };

    /**
     * @brief Constructor for the Statement class.
     *
     * Initializes the Statement object with a given name, SQL query, and type. The `whereFields`
     * parameter is optional and is used to specify the fields involved in the WHERE clause of the
     * SQL query (for queries like SELECT, DELETE, etc.).
     *
     * @param name The name of the SQL statement (e.g., "selectPk").
     * @param sql The SQL query string.
     * @param isUnique Indicates if the statement is unique.
     * @param type The type of SQL statement (default is SELECT).
     * @param whereFields A vector of fields used in the WHERE clause of the query.
     */
    explicit Statement(QString name, QString sql, bool isUnique, SQLTypes type = SQLTypes::select,
                       QVector<QString> whereFields = {});

    /**
     * @brief Overloaded constructor for the Statement class.
     *
     * Initializes the Statement object using a vector of SQL parts (e.g., different SQL fragments).
     * This can be useful for complex SQL queries that might need to be split into multiple parts.
     *
     * @param name The name of the SQL statement.
     * @param sqlVector A vector of strings representing different parts of the SQL query.
     * @param type The type of SQL statement (default is CREATE).
     */
    explicit Statement(QString name, QVector<QString> sqlVector, SQLTypes type = SQLTypes::create);

    // Getter methods for the Statement class:

    /**
     * @brief Retrieves the name of the SQL statement.
     *
     * @return The name of the SQL statement as a QString.
     */
    [[nodiscard]] QString name() const;

    /**
     * @brief Retrieves the SQL query string of the statement.
     *
     * @return The SQL query as a QString.
     */
    [[nodiscard]] QString sql() const;

    /**
     * @brief Retrieves the type of the SQL statement.
     *
     * @return The SQL type (SELECT, UPDATE, INSERT, etc.).
     */
    [[nodiscard]] SQLTypes type() const;

    /**
     * @brief Retrieves the list of fields involved in the WHERE clause of the SQL statement.
     *
     * @return A QVector of QStrings representing the WHERE fields.
     */
    [[nodiscard]] QVector<QString> whereFields() const;

    /**
     * @brief Generates and retrieves the method signature for executing the SQL statement.
     *
     * This could be useful for generating corresponding C++ code methods that interact with the database.
     *
     * @return A QString representing the method signature.
     */
    [[nodiscard]] QString signature() const;

    /**
     * @brief Retrieves the complete list of SQL statements as a string.
     *
     * This may combine all the parts of the SQL query into a single string.
     *
     * @return A QString representing the full SQL query.
     */
    [[nodiscard]] QString sentences() const;

    /**
     * @brief Retrieves the SQL query formatted for use in a C++ application.
     *
     * @return A QString representing the SQL query.
     */
    [[nodiscard]] QString sqlQuery() const;

    /**
     * @brief Retrieves the attributes (columns or fields) involved in the SQL query.
     *
     * @return A QString representing the attributes of the SQL statement.
     */
    [[nodiscard]] QString attributes() const;

    /**
     * @brief Retrieves the size of the SQL statement in terms of the number of fields or components.
     *
     * @return An integer representing the size of the SQL statement.
     */
    [[nodiscard]] int sqlSize() const;

    /**
     * @brief Retrieves the defines for the SQL statement, which might include macros or constants.
     *
     * @return A QString representing the defines associated with the SQL statement.
     */
    [[nodiscard]] QString defines() const;

    /**
     * @brief Checks whether the SQL statement is unique.
     *
     * This is useful for determining whether the statement should be treated as a unique operation,
     * such as when ensuring that only one row is affected by a query.
     *
     * @return A boolean indicating whether the statement is unique.
     */
    [[nodiscard]] bool isUnique() const;

private:
    QString                              m_name; ///< The name of the SQL statement.
    SQLTypes                             m_type; ///< The type of the SQL statement (e.g., SELECT, INSERT).
    QVector<QString>                     m_whereFields; ///< List of fields used in the WHERE clause.
    QVector<std::pair<QString, QString>> m_sqlVector; ///< SQL components for complex queries.
    bool                                 m_isUnique; ///< Flag indicating whether the SQL statement is unique.
};
