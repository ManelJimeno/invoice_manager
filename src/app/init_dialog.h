/**
 * @file init_dialog.h
 * @brief Contains the declaration of the InitDialog class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <exception.h>

#include "security/security.h"

class LoginError final : public core::Exception
{
    using Exception::Exception; ///< Inherit the constructor from Exception.
};

class InitDialog final : public QObject
{
    Q_OBJECT
public:
    explicit InitDialog(QQmlApplicationEngine *engine, std::function<void()> initCallback = nullptr,
                        QObject *parent = nullptr);

    ~InitDialog() override;

    void             show();
    Q_INVOKABLE void exit(int returnCode = 0) const;
    Q_INVOKABLE void close() const;
    Q_INVOKABLE core::modules::security::Security::LoginStatus login(const QString &user, const QString &password);
    Q_INVOKABLE static core::modules::security::Security::LoginStatus checkUser(const QString &user);

public slots:
    void incrementProgress(int value) const;

private:
    QQmlApplicationEngine                         *m_engine;
    QQuickWindow                                  *m_window;
    QObject                                       *m_progressBar;
    QObject                                       *m_stackView;
    const QUrl                                     m_loginUrl;
    std::shared_ptr<QEventLoop>                    m_eventLoop;
    const std::function<void()>                    m_initCallback;
    core::modules::security::Security::LoginStatus m_status;
};
