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
    m_database(db), m_create(CREATE, m_database), m_insert(INSERT, m_database), m_update(UPDATE, m_database),
    m_deleteRow(DELETE_ROW, m_database), m_selectPk(SELECT_PK, m_database), m_findUserById(FIND_USER_BY_ID, m_database),
    m_findUserByUsername(FIND_USER_BY_USERNAME, m_database), m_findUserByEmail(FIND_USER_BY_EMAIL, m_database)
{
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

void Users::insert(const std::shared_ptr<Record> &record)
{
    m_insert.bindValue(":id", record->m_id);
    m_insert.bindValue(":username", record->m_username);
    m_insert.bindValue(":email", record->m_email);
    m_insert.bindValue(":created_at", record->m_created_at);
    if (!m_insert.exec())
    {
        throw core::db::SQLError(m_insert.lastError().text());
    }
}

void Users::update(const std::shared_ptr<Record> &record)
{

    if (!m_update.exec())
    {
        throw core::db::SQLError(m_update.lastError().text());
    }
}

void Users::deleteRow(const std::shared_ptr<Record> &record)
{

    if (!m_deleteRow.exec())
    {
        throw core::db::SQLError(m_deleteRow.lastError().text());
    }
}

bool Users::selectPk(const std::shared_ptr<Record> &record)
{

    if (!m_selectPk.exec())
    {
        throw core::db::SQLError(m_selectPk.lastError().text());
    }
    if (m_selectPk.next())
    {
        const auto sqlRecord = m_selectPk.record();
        record->m_id         = sqlRecord.value("id").toLongLong();
        record->m_username   = sqlRecord.value("username").toString();
        record->m_email      = sqlRecord.value("email").toString();
        record->m_created_at = sqlRecord.value("created_at").toDateTime();

        return true;
    }
    return false;
}

bool Users::findUserById(const std::shared_ptr<Record> &record)
{
    m_findUserById.bindValue(":id", record->m_id);
    if (!m_findUserById.exec())
    {
        throw core::db::SQLError(m_findUserById.lastError().text());
    }
    if (m_findUserById.next())
    {
        const auto sqlRecord = m_findUserById.record();
        record->m_id         = sqlRecord.value("id").toLongLong();
        record->m_username   = sqlRecord.value("username").toString();
        record->m_email      = sqlRecord.value("email").toString();
        record->m_created_at = sqlRecord.value("created_at").toDateTime();

        return true;
    }
    return false;
}

bool Users::findUserByUsername(const std::shared_ptr<Record> &record)
{
    m_findUserByUsername.bindValue(":username", record->m_username);
    if (!m_findUserByUsername.exec())
    {
        throw core::db::SQLError(m_findUserByUsername.lastError().text());
    }
    if (m_findUserByUsername.next())
    {
        const auto sqlRecord = m_findUserByUsername.record();
        record->m_id         = sqlRecord.value("id").toLongLong();
        record->m_username   = sqlRecord.value("username").toString();
        record->m_email      = sqlRecord.value("email").toString();
        record->m_created_at = sqlRecord.value("created_at").toDateTime();

        return true;
    }
    return false;
}

bool Users::findUserByEmail(const std::shared_ptr<Record> &record)
{
    m_findUserByEmail.bindValue(":email", record->m_email);
    if (!m_findUserByEmail.exec())
    {
        throw core::db::SQLError(m_findUserByEmail.lastError().text());
    }
    return nextFindUserByEmail(record);
}

bool Users::nextFindUserByEmail(const std::shared_ptr<Record> &record)
{
    if (m_findUserByEmail.next())
    {
        const auto sqlRecord = m_findUserByEmail.record();
        record->m_id         = sqlRecord.value("id").toLongLong();
        record->m_username   = sqlRecord.value("username").toString();
        record->m_email      = sqlRecord.value("email").toString();
        record->m_created_at = sqlRecord.value("created_at").toDateTime();

        return true;
    }
    return false;
}
