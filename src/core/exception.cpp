/**
 * @file exception.cpp
 * @brief Implementation file for the Column class in the core module.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "exception.h"

namespace core
{

    Exception::Exception(QString msg) : m_msg(std::move(msg))
    {
    }

    char const *Exception::what() const noexcept
    {
        return m_msg.toUtf8().constData();
    }

} // namespace core
