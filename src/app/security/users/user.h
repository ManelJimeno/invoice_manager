/**
 * @file user.h
 * @brief Contains the declaration of the User class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "db/sqlite_table.h"

namespace security::users
{
class User
{
  public:
    explicit User();
    explicit User(const QSqlDatabase& database);

  private:
    core::db::SQLiteTable m_table;
};
} // namespace security::users
