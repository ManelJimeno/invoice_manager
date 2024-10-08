/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#pragma once

#include <QObject>

class MainWindow : public QObject
{
    Q_OBJECT

  public:
    explicit MainWindow(QObject* parent = nullptr);

    ~MainWindow();

  public slots:

    static void showAboutDialog();
};
