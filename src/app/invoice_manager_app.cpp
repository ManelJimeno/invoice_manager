/**
 * @file invoice_manager_app.cpp
 * @brief Implementation file for the InvoiceManagerApp class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "invoice_manager_app.h"

#include <QQuickStyle>
#include "init_dialog.h"
#include "security/security.h"

#include <QQmlContext>
#include <QQuickWindow>
#include <QTimer>

#include "db/db_manager.h"

InvoiceManagerApp::InvoiceManagerApp(int &argc, char **argv) :
    QApplication(argc, argv), m_settings(getDatabase(), "settings")
{
    auto aboutQuit = connect(this, &QCoreApplication::aboutToQuit, this, &InvoiceManagerApp::onAboutToQuit);
    QQuickStyle::setStyle("Material");
    m_engine.rootContext()->setContextProperty("mainWindow", &m_mainWindow);
    m_engine.addImportPath(QStringLiteral("qrc:/themes"));
}

QSqlDatabase &InvoiceManagerApp::getDatabase()
{
    static QSqlDatabase db = core::db::DBManager::manager().connect("QSQLITE", "./invoice_manager.db", "main");

    if (!db.isOpen())
    {
        if (!db.open())
        {
            qFatal("Failed to open the database.");
        }
    }
    return db;
}

int InvoiceManagerApp::loop()
{
    {
        auto      &security         = core::modules::security::Security::security();
        auto       functionCallback = [&]() { security.initialize(); };
        InitDialog init(&m_engine, functionCallback);

        auto progressChanged = connect(&security, &core::modules::security::Security::progressChanged, &init,
                                       &InitDialog::incrementProgress);
        try
        {
            init.show();
        }
        catch (LoginError)
        {
            throw;
        }
    }

    const QUrl url(QStringLiteral("qrc:/main_window.qml"));

    auto value = QObject::connect(
            &m_engine, &QQmlApplicationEngine::objectCreated, this,
            [url](const QObject *obj, const QUrl &objUrl)
            {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            },
            Qt::QueuedConnection);

    m_engine.load(url);
    return exec();
}

void InvoiceManagerApp::onAboutToQuit()
{
    qDebug() << "Application is about to quit, stopping modules...";
    core::modules::security::Security::security().stop();
}
