/**
 * @file factory.h
 * @brief Header file for the Factory class.
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

class RuntimeError final : public Exception
{
    using Exception::Exception;
};

class CORE_API Factory
{
  public:
    /**
     * @brief Returns the singleton instance of the Factory.
     * @return Factory instance.
     */
    [[nodiscard]] static Factory& factory();
    /**
     * @brief The way to access the SQL builder
     * @return DBMetaInfo object
     */
    [[nodiscard]] static std::shared_ptr<SQLBuilder> builder(const QString& dbType);

  private:
    /**
     * @brief private constructor avoids undesired instances
     */
    Factory();
};

} // namespace core::db
