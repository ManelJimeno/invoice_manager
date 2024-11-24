/**
 * @file main_window.cpp
 * @brief Implementation of the MainWindow class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "main_window.h"
#include <QApplication>
#include <QMessageBox>

/**
 * @brief MainWindow::MainWindow Constructs a MainWindow object.
 * @param parent A pointer to the parent QObject. Defaults to nullptr.
 */
MainWindow::MainWindow(QObject *parent) : QObject(parent)
{
}

/**
 * @brief MainWindow::~MainWindow Destroys the MainWindow object.
 */
MainWindow::~MainWindow() = default;

/**
 * @brief MainWindow::showAboutDialog Displays the About dialog.
 */
void MainWindow::showAboutDialog()
{
    QMessageBox::about(nullptr, tr("About This Application"), tr("Invoice manager."));
}
