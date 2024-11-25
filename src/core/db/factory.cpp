/**
 * @file factory.cpp
 * @brief Implementation file for the Factory class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "factory.h"
#include <memory>
#include "db/sqlite/sqlite_builder.h"
#include "db_manager.h"

namespace core::db
{

    Factory &Factory::factory()
    {
        static Factory factory;
        return factory;
    }

    std::shared_ptr<SQLBuilder> Factory::builder(const QString &dbType)
    {
        if (!DBManager::allowTypes().contains(dbType))
        {
            throw RuntimeError("This type of database is not registered.");
        }
        if (dbType == DBManager::QSQLITE)
        {
            return std::make_shared<SQLiteBuilder>();
        }
        return nullptr;
    }

    Factory::Factory() = default;

} // namespace core::db
