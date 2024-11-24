/**
 * @file module.h
 * @brief Contains the definition of the Module class
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once
#include <QObject>
#include <QString>
#include <functional>

namespace core::modules
{

    /**
     * @brief Represents a generic module with initialization capabilities.
     */
    class Module : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Constructor for the Module class.
         * @param name The name of the module.
         * @param description The description of the module.
         * @param initializationTime The approximate time required for initialization (in milliseconds).
         */
        Module(QString name, QString description, int initializationTime);

        /**
         * @brief Gets the name of the module.
         * @return The module name as a QString.
         */
        [[nodiscard]] QString name() const;

        /**
         * @brief Gets the description of the module.
         * @return The module description as a QString.
         */
        [[nodiscard]] QString description() const;

        /**
         * @brief Initializes the module.
         */
        virtual void initialize() = 0;

        /**
         * @brief Stops the module.
         */
        virtual void stop() = 0;

        /**
         * @brief Destructor for the Module class.
         */
        virtual ~Module() = default;

    signals:
        void progressChanged(int progress);

    private:
        QString m_name;
        QString m_description;
        int     m_initializationTime;
    };

} // namespace core::modules
