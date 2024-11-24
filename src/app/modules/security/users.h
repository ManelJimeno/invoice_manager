/**
 * @file users.h
 * @brief Contains the definition of the Users class, created through automatic generation,
 * do not modify this source code
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once
#include <QSqlQuery>
#include <memory>
#include <qdatetime.h>
#include "db/db_manager.h"
#include "db/sqlite/sqlite_db_api.h"

class Users : public core::db::SQLiteDbApi
{
public:
    struct Record
    {
        long long m_id;
        QString   m_username;
        QString   m_password;
        QString   m_email;
        long long m_groupId;
        QString   m_modified_by;
        QDateTime m_modified_at;
        QString   m_created_by;
        QDateTime m_created_at;
    };

    explicit Users(const QSqlDatabase &db = core::db::DBManager::manager().main());

    void      create();
    void      insert(Record &record);
    void      update(Record &record);
    void      deleteRow(Record &record);
    bool      selectPk(Record &record);
    long long countRows();
    bool      findUserByUsername(Record &record);
    bool      findUserByUsernamePassword(Record &record);
    bool      findUserByEmail(Record &record);
    bool      nextFindUserByEmail(Record &record);

private:
    const QString CREATE =
            "CREATE TABLE IF NOT EXISTS users ( id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, username TEXT UNIQUE, "
            "password TEXT, email TEXT, groupId INTEGER, modified_by TEXT, modified_at DATETIME DEFAULT "
            "CURRENT_TIMESTAMP, created_by TEXT, created_at DATETIME DEFAULT CURRENT_TIMESTAMP );";
    const QString CREATE_INDEX_1 = "CREATE INDEX IF NOT EXISTS idx_users_username ON users(username);";
    const QString INSERT         = "INSERT INTO users (username, password, email, groupId, modified_by, modified_at, "
                                   "created_by, created_at) VALUES (:username, :password, :email, :groupId, :modified_by, "
                                   "CURRENT_TIMESTAMP, :created_by, CURRENT_TIMESTAMP);";
    const QString UPDATE = "UPDATE users SET username=:username, password=:password, email=:email, groupId=:groupId, "
                           "modified_by=:modified_by, modified_at=CURRENT_TIMESTAMP, created_by=:created_by, "
                           "created_at=CURRENT_TIMESTAMP WHERE id=:id;";
    const QString DELETE_ROW            = "DELETE FROM users WHERE id=:id;";
    const QString SELECT_PK             = "SELECT * FROM users WHERE id=:id;";
    const QString COUNT_ROWS            = "SELECT COUNT(*) rows FROM users;";
    const QString FIND_USER_BY_USERNAME = "select * from users  where username = :username";
    const QString FIND_USER_BY_USERNAME_PASSWORD =
            "select * from users  where username = :username and password = :password";
    const QString FIND_USER_BY_EMAIL = "select * from users  where email = :email";


    QSqlQuery m_create;
    QSqlQuery m_insert;
    QSqlQuery m_update;
    QSqlQuery m_deleteRow;
    QSqlQuery m_selectPk;
    QSqlQuery m_countRows;
    QSqlQuery m_findUserByUsername;
    QSqlQuery m_findUserByUsernamePassword;
    QSqlQuery m_findUserByEmail;
};
