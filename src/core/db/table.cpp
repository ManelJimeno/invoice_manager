/**
 * @file table.cpp
 * @brief Defines the SQL statement types for the core::db module.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "table.h"

namespace core::db
{
/**
 * @brief Statement type for SQL CREATE operation.
 *
 * Represents the SQL `CREATE` statement used to create tables in the database.
 */
const StatementType StatementType::CREATE("create");

/**
 * @brief Statement type for SQL INSERT operation.
 *
 * Represents the SQL `INSERT` statement used to insert data into tables.
 */
const StatementType StatementType::INSERT("insert");

/**
 * @brief Statement type for SQL DELETE operation.
 *
 * Represents the SQL `DELETE` statement used to delete rows from tables.
 */
const StatementType StatementType::DELETE("delete");

/**
 * @brief Statement type for SQL UPDATE operation.
 *
 * Represents the SQL `UPDATE` statement used to modify rows in tables.
 */
const StatementType StatementType::UPDATE("update");

/**
 * @brief Statement type for SQL SELECT operation.
 *
 * Represents the SQL `SELECT` statement used to retrieve data from tables.
 */
const StatementType StatementType::SELECT("select");

/**
 * @brief Statement type for SQL SELECT by primary key operation.
 *
 * Represents the SQL `SELECT` statement that retrieves rows based on their primary key.
 */
const StatementType StatementType::SELECT_PK("select_pk");

} // namespace core::db
