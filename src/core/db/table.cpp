/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#include "table.h"

namespace core ::db
{
const StatementType StatementType::CREATE("create");
const StatementType StatementType::INSERT("insert");
const StatementType StatementType::DELETE("delete");
const StatementType StatementType::UPDATE("update");
const StatementType StatementType::SELECT("select");
const StatementType StatementType::SELECT_PK("select_pk");
} // namespace core::db
