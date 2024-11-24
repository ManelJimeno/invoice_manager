/**
 * @file users.cpp
 * @brief Contains the implementation of the Users class, created through automatic generation,
 * do not modify this source code
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "users.h"
#include <QSqlError>
#include <QSqlRecord>
#include "db/db_exception.h"

Users::Users(const QSqlDatabase &db) :
    core::db::SQLiteDbApi(db), m_create(m_database), m_insert(m_database), m_update(m_database),
    m_deleteRow(m_database), m_selectPk(m_database), m_countRows(m_database), m_findUserByUsername(m_database),
    m_findUserByUsernamePassword(m_database), m_findUserByEmail(m_database)
{
    m_insert.prepare(INSERT);
    m_update.prepare(UPDATE);
    m_deleteRow.prepare(DELETE_ROW);
    m_selectPk.prepare(SELECT_PK);
    m_countRows.prepare(COUNT_ROWS);
    m_findUserByUsername.prepare(FIND_USER_BY_USERNAME);
    m_findUserByUsernamePassword.prepare(FIND_USER_BY_USERNAME_PASSWORD);
    m_findUserByEmail.prepare(FIND_USER_BY_EMAIL);
}

void Users::create()
{
    std::array<QString, 2> sentences = {CREATE, CREATE_INDEX_1};
    for (auto &sentence: sentences)
    {
        if (!m_create.exec(sentence))
        {
            throw core::db::SQLError(m_create.lastError().text());
        }
    }
}

void Users::insert(Record &record)
{
    m_insert.bindValue(":username", record.m_username);
    m_insert.bindValue(":password", record.m_password);
    m_insert.bindValue(":email", record.m_email);
    m_insert.bindValue(":groupId", record.m_groupId);
    m_insert.bindValue(":modified_by", record.m_modified_by);
    m_insert.bindValue(":created_by", record.m_created_by);
    if (!m_insert.exec())
    {
        throw core::db::SQLError(m_insert.lastError().text());
    }
    record.m_id = getLastInsertRowId();
}

void Users::update(Record &record)
{

    if (!m_update.exec())
    {
        throw core::db::SQLError(m_update.lastError().text());
    }
}

void Users::deleteRow(Record &record)
{

    if (!m_deleteRow.exec())
    {
        throw core::db::SQLError(m_deleteRow.lastError().text());
    }
}

bool Users::selectPk(Record &record)
{

    if (!m_selectPk.exec())
    {
        throw core::db::SQLError(m_selectPk.lastError().text());
    }
    if (m_selectPk.next())
    {
        const auto sqlRecord = m_selectPk.record();
        record.m_id          = sqlRecord.value("id").toLongLong();
        record.m_username    = sqlRecord.value("username").toString();
        record.m_password    = sqlRecord.value("password").toString();
        record.m_email       = sqlRecord.value("email").toString();
        record.m_groupId     = sqlRecord.value("groupId").toLongLong();
        record.m_modified_by = sqlRecord.value("modified_by").toString();
        record.m_modified_at = sqlRecord.value("modified_at").toDateTime();
        record.m_created_by  = sqlRecord.value("created_by").toString();
        record.m_created_at  = sqlRecord.value("created_at").toDateTime();

        return true;
    }
    return false;
}

long long Users::countRows()
{

    if (!m_countRows.exec())
    {
        throw core::db::SQLError(m_countRows.lastError().text());
    }
    if (m_countRows.next())
    {
        const auto sqlRecord = m_countRows.record();
        return sqlRecord.value("rows").toLongLong();
    }
    return 0;
}

bool Users::findUserByUsername(Record &record)
{
    m_findUserByUsername.bindValue(":username", record.m_username);
    if (!m_findUserByUsername.exec())
    {
        throw core::db::SQLError(m_findUserByUsername.lastError().text());
    }
    if (m_findUserByUsername.next())
    {
        const auto sqlRecord = m_findUserByUsername.record();
        record.m_id          = sqlRecord.value("id").toLongLong();
        record.m_username    = sqlRecord.value("username").toString();
        record.m_password    = sqlRecord.value("password").toString();
        record.m_email       = sqlRecord.value("email").toString();
        record.m_groupId     = sqlRecord.value("groupId").toLongLong();
        record.m_modified_by = sqlRecord.value("modified_by").toString();
        record.m_modified_at = sqlRecord.value("modified_at").toDateTime();
        record.m_created_by  = sqlRecord.value("created_by").toString();
        record.m_created_at  = sqlRecord.value("created_at").toDateTime();

        return true;
    }
    return false;
}

bool Users::findUserByUsernamePassword(Record &record)
{
    m_findUserByUsernamePassword.bindValue(":username", record.m_username);
    m_findUserByUsernamePassword.bindValue(":password", record.m_password);
    if (!m_findUserByUsernamePassword.exec())
    {
        throw core::db::SQLError(m_findUserByUsernamePassword.lastError().text());
    }
    if (m_findUserByUsernamePassword.next())
    {
        const auto sqlRecord = m_findUserByUsernamePassword.record();
        record.m_id          = sqlRecord.value("id").toLongLong();
        record.m_username    = sqlRecord.value("username").toString();
        record.m_password    = sqlRecord.value("password").toString();
        record.m_email       = sqlRecord.value("email").toString();
        record.m_groupId     = sqlRecord.value("groupId").toLongLong();
        record.m_modified_by = sqlRecord.value("modified_by").toString();
        record.m_modified_at = sqlRecord.value("modified_at").toDateTime();
        record.m_created_by  = sqlRecord.value("created_by").toString();
        record.m_created_at  = sqlRecord.value("created_at").toDateTime();

        return true;
    }
    return false;
}

bool Users::findUserByEmail(Record &record)
{
    m_findUserByEmail.bindValue(":email", record.m_email);
    if (!m_findUserByEmail.exec())
    {
        throw core::db::SQLError(m_findUserByEmail.lastError().text());
    }
    return nextFindUserByEmail(record);
}

bool Users::nextFindUserByEmail(Record &record)
{
    if (m_findUserByEmail.next())
    {
        const auto sqlRecord = m_findUserByEmail.record();
        record.m_id          = sqlRecord.value("id").toLongLong();
        record.m_username    = sqlRecord.value("username").toString();
        record.m_password    = sqlRecord.value("password").toString();
        record.m_email       = sqlRecord.value("email").toString();
        record.m_groupId     = sqlRecord.value("groupId").toLongLong();
        record.m_modified_by = sqlRecord.value("modified_by").toString();
        record.m_modified_at = sqlRecord.value("modified_at").toDateTime();
        record.m_created_by  = sqlRecord.value("created_by").toString();
        record.m_created_at  = sqlRecord.value("created_at").toDateTime();

        return true;
    }
    return false;
}
