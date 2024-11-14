/**
 * @file db_exception.h
 * @brief Header file for the database-specific exception classes.
 *
 * This file defines the SQLError class, which extends a general Exception class
 * to handle database-related errors, particularly those arising from SQL operations.
 *
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "exception.h" // Base exception class to inherit from

namespace core::db
{

/**
 * @class SQLError
 * @brief Represents a database-specific error related to SQL operations.
 *
 * The SQLError class is a specialized exception type that is thrown when an SQL-related
 * error occurs within the database module. It inherits from the general Exception class,
 * which handles core exception functionality.
 *
 * SQLError is marked as final to prevent further inheritance, ensuring that SQL-related
 * exceptions are uniformly handled through this class.
 */
class SQLError final : public Exception
{
    using Exception::Exception; ///< Inherit constructors from Exception for standard error message handling.
};

}; // namespace core::db
