/**
 * @file groups.cpp
 * @brief Contains the implementation of the Groups class, created through automatic generation,
 * do not modify this source code
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "groups.h"
#include <QSqlError>
#include <QSqlRecord>
#include "db/db_exception.h"

Groups::Groups(const QSqlDatabase &db) :
    core::db::SQLiteDbApi(db), m_create(m_database), m_insert(m_database), m_update(m_database),
    m_deleteRow(m_database), m_selectPk(m_database), m_countRows(m_database), m_findUserByUsername(m_database)
{
    m_insert.prepare(INSERT);
    m_update.prepare(UPDATE);
    m_deleteRow.prepare(DELETE_ROW);
    m_selectPk.prepare(SELECT_PK);
    m_countRows.prepare(COUNT_ROWS);
    m_findUserByUsername.prepare(FIND_USER_BY_USERNAME);
}

void Groups::create()
{
    std::array<QString, 3> sentences = {CREATE, CREATE_INDEX_1, CREATE_INDEX_2};
    for (auto &sentence: sentences)
    {
        if (!m_create.exec(sentence))
        {
            throw core::db::SQLError(m_create.lastError().text());
        }
    }
}

void Groups::insert(Record &record)
{
    m_insert.bindValue(":groupName", record.m_groupName);
    m_insert.bindValue(":description", record.m_description);
    m_insert.bindValue(":modified_by", record.m_modified_by);
    m_insert.bindValue(":created_by", record.m_created_by);
    if (!m_insert.exec())
    {
        throw core::db::SQLError(m_insert.lastError().text());
    }
    record.m_id = getLastInsertRowId();
}

void Groups::update(Record &record)
{

    if (!m_update.exec())
    {
        throw core::db::SQLError(m_update.lastError().text());
    }
}

void Groups::deleteRow(Record &record)
{

    if (!m_deleteRow.exec())
    {
        throw core::db::SQLError(m_deleteRow.lastError().text());
    }
}

bool Groups::selectPk(Record &record)
{

    if (!m_selectPk.exec())
    {
        throw core::db::SQLError(m_selectPk.lastError().text());
    }
    if (m_selectPk.next())
    {
        const auto sqlRecord = m_selectPk.record();
        record.m_id          = sqlRecord.value("id").toLongLong();
        record.m_groupName   = sqlRecord.value("groupName").toString();
        record.m_description = sqlRecord.value("description").toString();
        record.m_modified_by = sqlRecord.value("modified_by").toString();
        record.m_modified_at = sqlRecord.value("modified_at").toDateTime();
        record.m_created_by  = sqlRecord.value("created_by").toString();
        record.m_created_at  = sqlRecord.value("created_at").toDateTime();

        return true;
    }
    return false;
}

long long Groups::countRows()
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

bool Groups::findUserByUsername(Record &record)
{

    if (!m_findUserByUsername.exec())
    {
        throw core::db::SQLError(m_findUserByUsername.lastError().text());
    }
    return nextFindUserByUsername(record);
}

bool Groups::nextFindUserByUsername(Record &record)
{
    if (m_findUserByUsername.next())
    {
        const auto sqlRecord = m_findUserByUsername.record();
        record.m_id          = sqlRecord.value("id").toLongLong();
        record.m_groupName   = sqlRecord.value("groupName").toString();
        record.m_description = sqlRecord.value("description").toString();
        record.m_modified_by = sqlRecord.value("modified_by").toString();
        record.m_modified_at = sqlRecord.value("modified_at").toDateTime();
        record.m_created_by  = sqlRecord.value("created_by").toString();
        record.m_created_at  = sqlRecord.value("created_at").toDateTime();

        return true;
    }
    return false;
}
