/**
 * @file db_exception.h
 * @brief Header file for the Exception classes.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "exception.h"

namespace core::db
{

class SQLError final : public Exception
{
    using Exception::Exception;
};

}; // namespace core::db
