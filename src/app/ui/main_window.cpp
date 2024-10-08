/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#include "main_window.h"
#include <QApplication>
#include <QMessageBox>

MainWindow::MainWindow(QObject* parent) : QObject(parent)
{
}

MainWindow::~MainWindow() = default;

void MainWindow::showAboutDialog()
{
    QMessageBox::about(nullptr, tr("About This Application"), tr("Invoice manager."));
}
