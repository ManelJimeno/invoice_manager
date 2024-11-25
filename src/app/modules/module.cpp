/**
 * @file module.cpp
 * @brief Contains the implementation of the Module class
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "module.h"

#include <utility>

namespace core::modules
{

    Module::Module(QString name, QString description, const int initializationTime) :
        m_name(std::move(name)), m_description(std::move(description)), m_initializationTime(initializationTime)
    {
    }

    QString Module::name() const
    {
        return m_name;
    }

    QString Module::description() const
    {
        return m_description;
    }

} // namespace core::modules
