/**
 * @file invoice_manager_app.h
 * @brief Contains the declaration of the InvoiceManagerApp class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include <QApplication>
#include <QQmlApplicationEngine>
#include "settings/sqlite_settings.h"
#include "ui/main_window.h"

constexpr auto APP_NAME = "InvoiceManager";

class InvoiceManagerApp final : public QApplication
{
    Q_OBJECT

public:
    // Prevent copy-construction and assignment
    InvoiceManagerApp(const InvoiceManagerApp &)            = delete;
    InvoiceManagerApp &operator=(const InvoiceManagerApp &) = delete;

    /**
     * @brief Constructor for InvoiceManagerApp.
     * @param argc Command line argument count.
     * @param argv Command line arguments.
     */
    InvoiceManagerApp(int &argc, char **argv);

    /**
     * @brief Returns the singleton instance of the QSqlDatabase.
     * @return QSqlDatabase instance.
     */
    static QSqlDatabase &getDatabase();

    /**
     * @brief main loop
     * @return The exit status of the application
     */
    [[nodiscard]] int loop();

private:
    static void onAboutToQuit();

    MainWindow                     m_mainWindow; ///< Main window of the application.
    QQmlApplicationEngine          m_engine; ///< QML engine for loading QML UI.
    core::settings::SQLiteSettings m_settings; ///< Settings of the app.
};
