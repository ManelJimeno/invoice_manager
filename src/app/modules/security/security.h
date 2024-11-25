/**
 * @file security.h
 * @brief Contains the declaration of the Security class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "module.h"

/**
 * @namespace core::modules::security
 * @brief Encapsulates the security module within the core framework.
 *
 * This namespace contains the Security class, responsible for managing
 * authentication and security-related operations in the application.
 */
namespace core::modules::security
{
    /**
     * @class Security
     * @brief Provides security-related functionality, such as user authentication.
     *
     * The `Security` class is a final implementation of the `core::modules::Module` base class.
     * It manages user authentication, password hashing, and other related functionalities.
     */
    class Security final : public core::modules::Module
    {
    public:
        /**
         * @enum LoginStatus
         * @brief Represents the status of a login attempt.
         *
         * This enum defines the possible outcomes when attempting to log in a user.
         */
        enum class LoginStatus
        {
            NOT_LOGGED_IN, ///< The user is not logged in.
            USER_DOES_NOT_EXIST, ///< The provided username does not exist.
            PASSWORD_IS_INCORRECT, ///< The password provided is incorrect.
            USER_IDENTIFIED ///< The user has been successfully identified.
        };

        /**
         * @brief Retrieves the singleton instance of the Security class.
         *
         * The `security` method provides access to the single instance of the Security class,
         * ensuring that the module follows the Singleton design pattern.
         *
         * @return A reference to the Security instance.
         */
        static Security &security();

        /**
         * @brief Initializes the Security module.
         *
         * This method sets up necessary resources and prepares the module for use.
         * Overrides the initialize method in the base `Module` class.
         */
        void initialize() override;

        /**
         * @brief Stops the Security module.
         *
         * This method releases resources and performs any necessary cleanup for the module.
         * Overrides the stop method in the base `Module` class.
         */
        void stop() override;

        /**
         * @brief Attempts to log in a user with the provided credentials.
         *
         * This method verifies the provided username and password, and returns the status
         * of the login attempt.
         *
         * @param user The username of the user attempting to log in.
         * @param chars The password associated with the username.
         * @return The login status as a `LoginStatus` value.
         */
        static LoginStatus login(const QString &user, const QString &chars);

        /**
         * @brief Checks if a user exists in the system.
         *
         * This method verifies whether a user with the provided username exists in the database.
         *
         * @param user The username to check.
         * @return The result as a `LoginStatus` value.
         */
        static LoginStatus checkUser(const QString &user);

        /**
         * @brief Hashes a string using the application's hashing algorithm.
         *
         * This method securely hashes the given string, typically used for password storage
         * or verification purposes.
         *
         * @param value The string to hash.
         * @return A hashed representation of the input string.
         */
        static QString hashString(const QString &value);

    private:
        /**
         * @brief Constructs the Security object.
         *
         * This constructor is private to enforce the Singleton design pattern.
         */
        explicit Security();
    };
} // namespace core::modules::security
