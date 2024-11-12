/**
 * @file statement.h
 * @brief Contains the declaration of the Statement class.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once
#include "dllexports.h"
#include <QVector>

class Statement
{
  public:
    enum class SQLTypes
    {
        select,
        update,
        insert,
        create,
        deleteRow,
    };

    explicit Statement(QString name, QString sql, bool isUnique, SQLTypes type = SQLTypes::select,
                       QVector<QString> whereFields = {});
    explicit Statement(QString name, QVector<QString> sqlVector, SQLTypes type = SQLTypes::create);

    [[nodiscard]] QString name() const;
    [[nodiscard]] QString sql() const;
    [[nodiscard]] SQLTypes type() const;
    [[nodiscard]] QVector<QString> whereFields() const;
    [[nodiscard]] QString signature() const;
    [[nodiscard]] QString sentences() const;
    [[nodiscard]] QString sqlQuery() const;
    [[nodiscard]] QString attributes() const;
    [[nodiscard]] int sqlSize() const;
    [[nodiscard]] QString defines() const;
    [[nodiscard]] bool isUnique() const;

  private:
    QString m_name;
    SQLTypes m_type;
    QVector<QString> m_whereFields;
    QVector<std::pair<QString, QString>> m_sqlVector;
    bool m_isUnique;
};
