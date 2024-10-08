/**
 * @file main_window.h
 * @brief Contains the definition of the MainWindow class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include <QObject>

/**
 * @class MainWindow
 * @brief Main application window.
 */
class MainWindow : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief Constructor for MainWindow class.
     * @param parent The parent object.
     */
    explicit MainWindow(QObject* parent = nullptr);

    /**
     * @brief Destructor for MainWindow class.
     */
    ~MainWindow() override;

  public slots:
    /**
     * @brief Shows the About dialog for the application.
     */
    static void showAboutDialog();
};
