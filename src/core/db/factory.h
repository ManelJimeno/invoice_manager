/**
 * @file factory.h
 * @brief Header file for the Factory class.
 *
 * This file declares the Factory class, a singleton responsible for providing access
 * to essential database components, such as the SQL builder.
 *
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "dynamic_table.h"
#include "sql_builder.h"

#include <memory>

namespace core::db
{

    /**
     * @class RuntimeError
     * @brief Exception class for handling runtime errors in the database operations.
     *
     * This class is derived from the base `Exception` class and is used to represent
     * runtime errors that occur within the Factory class or related operations.
     */
    class RuntimeError final : public Exception
    {
        using Exception::Exception;
    };

    /**
     * @class Factory
     * @brief Singleton class for providing database-related components.
     *
     * The Factory class ensures that only one instance of the factory exists, providing
     * access to essential tools like the SQL builder for database operations.
     */
    class CORE_API Factory
    {
    public:
        /**
         * @brief Provides access to the singleton instance of the Factory.
         *
         * This method ensures that only one instance of the Factory class exists and
         * provides global access to it.
         *
         * @return The singleton instance of the Factory class.
         */
        [[nodiscard]] static Factory &factory();

        /**
         * @brief Provides access to the SQL builder for a specified database type.
         *
         * This method returns a shared pointer to an instance of the `SQLBuilder`
         * specific to the given database type. The builder is responsible for generating
         * SQL queries compatible with the target database.
         *
         * @param dbType The type of database (e.g., "QSQLITE").
         * @return A shared pointer to the corresponding SQLBuilder instance.
         */
        [[nodiscard]] static std::shared_ptr<SQLBuilder> builder(const QString &dbType);

    private:
        /**
         * @brief Private constructor to prevent instantiating multiple instances.
         *
         * The constructor is private to ensure that only one instance of the Factory class
         * exists, following the Singleton design pattern.
         */
        Factory();
    };

} // namespace core::db
