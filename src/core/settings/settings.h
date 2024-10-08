/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#pragma once

#include "dllexports.h"
#include <QVariant>

namespace core::settings
{
class Settings
{
  public:
    virtual bool write() = 0;
    virtual bool read() = 0;

    virtual ~Settings() = default;

    CORE_API QVariant& operator[](const QString& key);

  protected:
    QMap<QString, QVariant> m_values;
};
} // namespace core::settings
