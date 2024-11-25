/**
 * @file security.cpp
 * @brief Implementation file for the Security class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "security.h"
#include "groups.h"
#include "users.h"

#include <qcryptographichash.h>

namespace core::modules::security
{
    Security::Security() : Module("Security", "Security module", 10)
    {
    }

    Security &Security::security()
    {
        static Security self;
        return self;
    }

    void Security::initialize()
    {
        // If the table does not exist, the preparation of all the statements will fail,
        // so we first create the table and then make queries on it.
        {
            Groups groupTable;
            groupTable.create();
            Users userTable;
            userTable.create();
        }
        Groups         groupTable;
        Groups::Record group;
        Users          userTable;
        if (groupTable.countRows() == 0)
        {
            group.m_id          = 0;
            group.m_groupName   = "admin";
            group.m_description = "default admin group";
            group.m_created_by  = "admin";
            group.m_modified_by = "admin";
            groupTable.insert(group);
        }
        emit progressChanged(50);
        if (userTable.countRows() == 0)
        {
            Users::Record user;
            user.m_username    = "admin";
            user.m_password    = hashString("admin");
            user.m_created_by  = "admin";
            user.m_modified_by = "admin";
            user.m_groupId     = group.m_id;
            userTable.insert(user);
        }
        emit progressChanged(50);
    }

    void Security::stop()
    {
    }

    Security::LoginStatus Security::login(const QString &user, const QString &password)
    {
        Users         users;
        Users::Record record;
        record.m_username = user;
        record.m_password = hashString(password);
        if (!users.findUserByUsername(record))
        {
            return LoginStatus::USER_DOES_NOT_EXIST;
        }
        if (!users.findUserByUsernamePassword(record))
        {
            return LoginStatus::PASSWORD_IS_INCORRECT;
        }
        return LoginStatus::USER_IDENTIFIED;
    }

    Security::LoginStatus Security::checkUser(const QString &user)
    {
        Users         users;
        Users::Record record;
        record.m_username = user;
        if (!users.findUserByUsername(record))
        {
            return LoginStatus::USER_DOES_NOT_EXIST;
        }
        return LoginStatus::USER_IDENTIFIED;
    }

    QString Security::hashString(const QString &value)
    {
        const QByteArray   byteArray = value.toUtf8();
        QCryptographicHash hash(QCryptographicHash::Sha512);
        hash.addData(byteArray);
        return hash.result().toHex();
    }

}; // namespace core::modules::security
