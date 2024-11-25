/**
 * @file source_template.cpp
 * @brief Contains the source template.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

constexpr const char *getHeaderTemplate()
{
    return R"(/**
 * @file {table_name}.h
 * @brief Contains the definition of the {class_name} class, created through automatic generation,
 * do not modify this source code
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once
#include "db/db_manager.h"
#include {header_parent_class_name}
#include <QSqlQuery>
#include <memory>
#include <qdatetime.h>

class {class_name} : public {parent_class_name}
{{
  public:
    struct Record
    {{
{record}
    }};

    explicit {class_name}(const QSqlDatabase& db = core::db::DBManager::manager().main());

{public_signatures}
  private:

{sentences}

{sql_query}
}};
)";
}

constexpr const char *getSourceTemplate()
{
    return R"(/**
 * @file {table_name}.cpp
 * @brief Contains the implementation of the {class_name} class, created through automatic generation,
 * do not modify this source code
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "{table_name}.h"
#include "db/db_exception.h"
#include <QSqlError>
#include <QSqlRecord>

{class_name}::{class_name}(const QSqlDatabase& db)
    : {parent_class_name}(db), {attributes}
{{
{prepare}
}}

void {class_name}::create()
{{
    std::array<QString, {create_sentences_size}> sentences = {{ {create_sentences} }};
    for (auto& sentence : sentences)
    {{
        if (!m_create.exec(sentence))
        {{
            throw core::db::SQLError(m_create.lastError().text());
        }}
    }}
}}

{class_methods}

)";
}

constexpr const char *getNoSelectMethod()
{
    return R"(void {class_name}::{method_name}(Record& record)
{{
    {record_to_bind}
    if (!{sql_query}.exec())
    {{
        throw core::db::SQLError({sql_query}.lastError().text());
    }}
}}

)";
}

constexpr const char *getInsertMethod()
{
    return R"(void {class_name}::{method_name}(Record& record)
{{
    {record_to_bind}
    if (!{sql_query}.exec())
    {{
        throw core::db::SQLError({sql_query}.lastError().text());
    }}
    {recover_autoincrement}
}}

)";
}

constexpr const char *getUniqueSelectMethod()
{
    return R"(bool {class_name}::{method_name}(Record& record)
{{
    {record_to_bind}
    if (!{sql_query}.exec())
    {{
        throw core::db::SQLError({sql_query}.lastError().text());
    }}
    if ({sql_query}.next())
    {{
        const auto sqlRecord = {sql_query}.record();
        {record_to_structure}
        return true;
    }}
    return false;
}}

)";
}

constexpr const char *getSelectCount()
{
    return R"(long long {class_name}::{method_name}()
{{
    {record_to_bind}
    if (!{sql_query}.exec())
    {{
        throw core::db::SQLError({sql_query}.lastError().text());
    }}
    if ({sql_query}.next())
    {{
        const auto sqlRecord = {sql_query}.record();
        return sqlRecord.value("rows").toLongLong();
    }}
    return 0;
}}

)";
}

constexpr const char *getSelectMethod()
{
    return R"(bool {class_name}::{method_name}(Record& record)
{{
    {record_to_bind}
    if (!{sql_query}.exec())
    {{
        throw core::db::SQLError({sql_query}.lastError().text());
    }}
    return next{capitalized_method_name}(record);
}}

bool {class_name}::next{capitalized_method_name}(Record& record)
{{
    if ({sql_query}.next())
    {{
        const auto sqlRecord = {sql_query}.record();
        {record_to_structure}
        return true;
    }}
    return false;
}}

)";
}
