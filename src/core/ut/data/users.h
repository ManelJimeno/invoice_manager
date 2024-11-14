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
#include "db/db_manager.h"
#include <QSqlQuery>
#include <memory>
#include <qdatetime.h>

class Users
{
  public:
    struct Record
    {
        int m_id;
        QString m_username;
        QString m_email;
        QDateTime m_created_at;
    };

    explicit Users(const QSqlDatabase& db = core::db::DBManager::manager().main());

    void create();
    void insert(const std::shared_ptr<Record>& record);
    void update(const std::shared_ptr<Record>& record);
    void deleteRow(const std::shared_ptr<Record>& record);
    bool selectPk(const std::shared_ptr<Record>& record);
    bool findUserById(const std::shared_ptr<Record>& record);
    bool findUserByUsername(const std::shared_ptr<Record>& record);
    bool findUserByEmail(const std::shared_ptr<Record>& record);
    bool nextFindUserByEmail(const std::shared_ptr<Record>& record);

  private:
    const QString CREATE = "CREATE TABLE IF NOT EXISTS users ( id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, username "
                           "TEXT UNIQUE, email TEXT, created_at DATETIME );";
    const QString CREATE_INDEX_1 = "CREATE INDEX users_pk ON users(username);";
    const QString INSERT =
        "INSERT INTO users (id, username, email, created_at) VALUES (:id, :username, :email, :created_at);";
    const QString UPDATE =
        "UPDATE users SET id=:id, username=:username, email=:email, created_at=:created_at WHERE id=:id;";
    const QString DELETE_ROW = "DELETE FROM users WHERE id=:id;";
    const QString SELECT_PK = "SELECT * FROM users WHERE id=:id;";
    const QString FIND_USER_BY_ID = "select * from users  where id = :id";
    const QString FIND_USER_BY_USERNAME = "select * from users  where username = :username";
    const QString FIND_USER_BY_EMAIL = "select * from users  where email = :email";

    QSqlDatabase m_database;

    QSqlQuery m_create;
    QSqlQuery m_insert;
    QSqlQuery m_update;
    QSqlQuery m_deleteRow;
    QSqlQuery m_selectPk;
    QSqlQuery m_findUserById;
    QSqlQuery m_findUserByUsername;
    QSqlQuery m_findUserByEmail;
};
