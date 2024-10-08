/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#pragma once

#include "table.h"
#include <QSqlQuery>
#include <QVariant>

namespace core::db
{

enum class SQLiteDataType
{
    INTEGER,
    REAL,
    TEXT,
    BLOB,
    NULL_TYPE
};

class SQLiteTable : public Table<SQLiteDataType>
{
  public:
    // Constructor
    CORE_API explicit SQLiteTable(const QSqlDatabase& database, QString name,
                                  QMap<QString, Column<SQLiteDataType>> columns)
        : Table(database, std::move(name), std::move(columns))
    {
    }

    CORE_API explicit SQLiteTable(const QSqlDatabase& database, QString name,
                                  std::initializer_list<std::pair<QString, Column<SQLiteDataType>>> columns)
        : Table(database, std::move(name), columns)
    {
    }

  private:
    [[nodiscard]] QString createTable() const override;

    [[nodiscard]] QString createInsert() const override;

    [[nodiscard]] QString createUpdate() const override;

    [[nodiscard]] QString createSelect() const override;

    [[nodiscard]] QString createSelectPk() const override;

    [[nodiscard]] QString createDelete() const override;

    [[nodiscard]] QString whereClause() const override;

    [[nodiscard]] static QString toSQLiteType(SQLiteDataType type);
};

} // namespace core::db
