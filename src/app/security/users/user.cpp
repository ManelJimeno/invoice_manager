/**
 * @file user.cpp
 * @brief Implementation file for the User class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "user.h"
#include "../../invoice_manager_app.h"

using namespace core::db;

/*
id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL,
    email TEXT,
    is_active BOOLEAN DEFAULT 1,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
 */
namespace security::users
{
static const std::initializer_list<std::pair<QString, SQLiteColumn>> userColumns = {
    {"id", SQLiteColumn("id", SQLiteDataType::INTEGER,
                        SQLiteModifier::isNotNull | SQLiteModifier::isAutoIncrement | SQLiteModifier::isPrimaryKey,
                        "users_pk")},
    {"username", SQLiteColumn("username", SQLiteDataType::TEXT, SQLiteModifier::isNotNull)},
    {"password", SQLiteColumn("password", SQLiteDataType::TEXT, SQLiteModifier::isNotNull)},
    {"email", SQLiteColumn("email", SQLiteDataType::TEXT, SQLiteModifier::isNotNull)},
    {"created_at",
     SQLiteColumn("created_at", SQLiteDataType::DATETIME, SQLiteModifier::isNotNull, "", "", "", "CURRENT_TIMESTAMP")},
};

User::User() : User(InvoiceManagerApp::getDatabase())
{
}

User::User(const QSqlDatabase& database) : m_table(core::db::SQLiteTable(database, "users", userColumns))
{
}
} // namespace security::users
