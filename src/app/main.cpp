/**
 * @file main.cpp
 * @brief The entry point for the Invoice Manager application.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "ui/main_window.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

/**
 * @brief The main function, entry point of the application.
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments.
 * @return The exit status of the application.
 */
int main(int argc, char* argv[])
{
    const QApplication app(argc, argv);
    MainWindow mainWindow;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("mainWindow", &mainWindow);

    const QUrl url(QStringLiteral("qrc:/ui/main_window.qml"));
    auto value = QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);
    return QApplication::exec();
}
