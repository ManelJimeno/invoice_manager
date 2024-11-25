/**
 * @file groups.h
 * @brief Contains the definition of the Groups class, created through automatic generation,
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

class Groups : public core::db::SQLiteDbApi
{
public:
    struct Record
    {
        long long m_id;
        QString   m_groupName;
        QString   m_description;
        QString   m_modified_by;
        QDateTime m_modified_at;
        QString   m_created_by;
        QDateTime m_created_at;
    };

    explicit Groups(const QSqlDatabase &db = core::db::DBManager::manager().main());

    void      create();
    void      insert(Record &record);
    void      update(Record &record);
    void      deleteRow(Record &record);
    bool      selectPk(Record &record);
    long long countRows();
    bool      findUserByUsername(Record &record);
    bool      nextFindUserByUsername(Record &record);

private:
    const QString CREATE = "CREATE TABLE IF NOT EXISTS groups ( id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, groupName "
                           "TEXT UNIQUE, description TEXT, modified_by TEXT, modified_at DATETIME DEFAULT "
                           "CURRENT_TIMESTAMP, created_by TEXT, created_at DATETIME DEFAULT CURRENT_TIMESTAMP );";
    const QString CREATE_INDEX_1 = "CREATE INDEX IF NOT EXISTS idx_groups_groupName ON groups(groupName);";
    const QString CREATE_INDEX_2 = "CREATE INDEX IF NOT EXISTS idx_groups_id ON groups(id);";
    const QString INSERT =
            "INSERT INTO groups (groupName, description, modified_by, modified_at, created_by, created_at) VALUES "
            "(:groupName, :description, :modified_by, CURRENT_TIMESTAMP, :created_by, CURRENT_TIMESTAMP);";
    const QString UPDATE =
            "UPDATE groups SET groupName=:groupName, description=:description, modified_by=:modified_by, "
            "modified_at=CURRENT_TIMESTAMP, created_by=:created_by, created_at=CURRENT_TIMESTAMP WHERE id=:id;";
    const QString DELETE_ROW            = "DELETE FROM groups WHERE id=:id;";
    const QString SELECT_PK             = "SELECT * FROM groups WHERE id=:id;";
    const QString COUNT_ROWS            = "SELECT COUNT(*) rows FROM groups;";
    const QString FIND_USER_BY_USERNAME = "select * from groups  where groupName = :groupName";


    QSqlQuery m_create;
    QSqlQuery m_insert;
    QSqlQuery m_update;
    QSqlQuery m_deleteRow;
    QSqlQuery m_selectPk;
    QSqlQuery m_countRows;
    QSqlQuery m_findUserByUsername;
};
