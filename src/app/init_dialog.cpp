/**
 * @file init_dialog.cpp
 * @brief Implementation file for the InitDialog class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "init_dialog.h"

#include <QApplication>
#include <QQmlContext>
#include <QtConcurrent>


InitDialog::InitDialog(QQmlApplicationEngine *engine, std::function<void()> initCallback, QObject *parent) :
    QObject(parent), m_engine(engine), m_window(nullptr), m_initCallback(std::move(initCallback)),
    m_loginUrl("qrc:/login_page.qml"), m_status(core::modules::security::Security::LoginStatus::NOT_LOGGED_IN)
{
    m_engine->load(QUrl("qrc:/init_dialog.qml"));

    if (m_engine->rootObjects().isEmpty())
    {
        qWarning() << "Failed to load the modal window.";
        throw std::runtime_error("Failed to load the modal window.");
    }

    QObject *rootObject = m_engine->rootObjects().first();
    m_window            = qobject_cast<QQuickWindow *>(rootObject);

    if (m_window)
    {
        m_engine->rootContext()->setContextProperty("initDialog", this);

        m_progressBar = m_window->findChild<QObject *>("progressBar");
        if (!m_progressBar)
        {
            qWarning() << "ProgressBar not found in the modal window.";
        }
        m_stackView = m_window->findChild<QObject *>("stackView");
        if (!m_stackView)
        {
            qWarning() << "StackView not found in the modal window.";
        }
        qDebug() << "Modal window successfully loaded.";
    }
    else
    {
        throw std::runtime_error("The root object is not a valid window.");
    }
}

InitDialog::~InitDialog()
{
    close();
    m_engine->clearComponentCache();
}

void InitDialog::show()
{
    if (m_window && m_initCallback)
    {
        m_eventLoop = std::make_shared<QEventLoop>(this);
        m_window->show();
        auto future = QtConcurrent::run(
                [this]()
                {
                    m_initCallback();

                    const auto result = QMetaObject::invokeMethod(
                            this,
                            [this]() { QMetaObject::invokeMethod(m_stackView, "pushItem", Q_ARG(QUrl, m_loginUrl)); });
                    if (!result)
                    {
                        qWarning() << "Failed to call QMetaObject::invokeMethod(stackView, push, qrc:/login_page.qml)";
                    }
                    else
                    {
                        m_engine->rootContext()->setContextProperty("initDialog", this);
                    }
                });
        m_eventLoop->exec();

        qDebug() << "Modal window successfully showed.";

        switch (m_status)
        {
            case core::modules::security::Security::LoginStatus::NOT_LOGGED_IN:
                throw LoginError("Login failed.");
            case core::modules::security::Security::LoginStatus::USER_DOES_NOT_EXIST:
                throw LoginError("User does not exist.");
            case core::modules::security::Security::LoginStatus::PASSWORD_IS_INCORRECT:
                throw LoginError("Password is incorrect.");
            case core::modules::security::Security::LoginStatus::USER_IDENTIFIED:
                break;
        }
    }
    else
    {
        qWarning() << "No modal window loaded to show.";
    }
}

void InitDialog::exit(const int returnCode) const
{
    close();
    if (returnCode != 0)
    {
        QApplication::exit(returnCode);
    }
}

void InitDialog::close() const
{
    m_eventLoop->quit();
    m_window->close();
}

void InitDialog::incrementProgress(const int value) const
{
    if (m_progressBar)
    {
        const int currentValue = m_progressBar->property("value").toInt();
        if (currentValue >= 100)
        {
            return;
        }
        m_progressBar->setProperty("value", std::min(currentValue + value, 100));
    }
    else
    {
        qWarning() << "ProgressBar not found or not initialized.";
    }
}

core::modules::security::Security::LoginStatus InitDialog::login(const QString &user, const QString &password)
{
    if (user.isEmpty() || password.isEmpty())
    {
        m_status = core::modules::security::Security::LoginStatus::NOT_LOGGED_IN;
    }
    else
    {
        m_status = core::modules::security::Security::login(user, password);
    }
    return m_status;
}

core::modules::security::Security::LoginStatus InitDialog::checkUser(const QString &user)
{
    if (user.isEmpty())
    {
        return core::modules::security::Security::LoginStatus::NOT_LOGGED_IN;
    }
    return core::modules::security::Security::checkUser(user);
}
