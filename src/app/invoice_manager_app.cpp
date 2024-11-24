/**
 * @file invoice_manager_app.cpp
 * @brief Implementation file for the InvoiceManagerApp class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "invoice_manager_app.h"

InvoiceManagerApp::InvoiceManagerApp(int &argc, char **argv) :
    QApplication(argc, argv), m_settings(getDatabase(), "settings")
{
    m_engine.rootContext()->setContextProperty("mainWindow", &m_mainWindow);
}

QSqlDatabase &InvoiceManagerApp::getDatabase()
{
    static QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    if (!db.isOpen())
    {
        db.setDatabaseName("invoices.db");
        if (!db.open())
        {
            qFatal("Failed to open the database.");
        }
    }
    return db;
}

int InvoiceManagerApp::loop()
{
    const QUrl url(QStringLiteral("qrc:/ui/main_window.qml"));

    auto value = QObject::connect(
            &m_engine, &QQmlApplicationEngine::objectCreated, this,
            [url](QObject *obj, const QUrl &objUrl)
            {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            },
            Qt::QueuedConnection);

    m_engine.load(url);
    return exec();
}
