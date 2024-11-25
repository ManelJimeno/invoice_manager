/**
 * @file exception.h
 * @brief Contains the declaration of the Exception class and its derived classes.
 *
 * This header file defines a base `Exception` class for handling custom exceptions
 * within the core library, as well as several derived exception classes that represent
 * specific error conditions such as "NotImplemented", "FileNotFound", and "FileNotOpen".
 *
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// Import necessary libraries and definitions
#include <QString>
#include <exception>
#include "dllexports.h"

namespace core
{

    /**
     * @class Exception
     * @brief A base class for custom exceptions in the core library.
     *
     * The `Exception` class extends the standard `std::exception` and is used as a base class
     * for all custom exceptions in the core library. It encapsulates an error message that can
     * be accessed when the exception is thrown.
     */
    class CORE_API Exception : public std::exception
    {
    public:
        /**
         * @brief Constructs an Exception with a custom message.
         * @param msg The error message associated with the exception.
         */
        explicit Exception(QString msg);

        /**
         * @brief Retrieves the error message associated with the exception.
         * @return The error message as a C-style string.
         *
         * This function overrides the `what()` function from `std::exception` and returns
         * the error message stored in `m_msg` as a C-style string, making it compatible
         * with standard exception handling systems.
         */
        [[nodiscard]] char const *what() const noexcept override;

    private:
        QString m_msg; ///< The error message for this exception.
    };

    /**
     * @class NotImplemented
     * @brief Exception thrown when a feature is not yet implemented.
     *
     * This exception class is used to represent an error when a feature or method has
     * not yet been implemented in the code. It inherits from the `Exception` class.
     */
    class NotImplemented final : public Exception
    {
        using Exception::Exception; ///< Inherit the constructor from Exception.
    };

    /**
     * @class FileNotFound
     * @brief Exception thrown when a file is not found.
     *
     * This exception class is used to represent an error when a file is not found.
     * It inherits from the `Exception` class.
     */
    class FileNotFound final : public Exception
    {
        using Exception::Exception; ///< Inherit the constructor from Exception.
    };

    /**
     * @class FileNotOpen
     * @brief Exception thrown when a file cannot be opened.
     *
     * This exception class is used to represent an error when a file cannot be opened.
     * It inherits from the `Exception` class.
     */
    class FileNotOpen final : public Exception
    {
        using Exception::Exception; ///< Inherit the constructor from Exception.
    };

} // namespace core
