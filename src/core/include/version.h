/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#pragma once

#include "dllexports.h"
#include <QString>

namespace core::package
{

struct Version
{
    QString full;
    QString triplet;
    int major;
    int minor;
    int revision;
};

CORE_API Version getVersion() noexcept;

} // namespace core::package
