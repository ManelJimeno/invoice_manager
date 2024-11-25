/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */
#include "settings.h"

namespace core::settings
{
    QVariant &Settings::operator[](const QString &key)
    {
        return m_values[key];
    }

}; // namespace core::settings
