/**
 * @file exception.h
 * @brief Contains the declaration of the Exception class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */
#pragma once

#include "dllexports.h"
#include <QString>
#include <exception>
#include <string>

namespace core
{

class CORE_API Exception : public std::exception
{
  public:
    explicit Exception(QString msg);
    [[nodiscard]] char const* what() const noexcept override;

  private:
    QString m_msg;
};

class NotImplemented final : public Exception
{
    using Exception::Exception;
};

class FileNotFound final : public Exception
{
    using Exception::Exception;
};

class FileNotOpen final : public Exception
{
    using Exception::Exception;
};

} // namespace core
