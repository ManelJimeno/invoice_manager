/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#include "ui/main_window.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
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
