/**
 * Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#include "version.h"
#include <gtest/gtest.h>

TEST(CorePackage, version)
{
    auto v = core::package::getVersion();
    EXPECT_EQ(v.full.isEmpty(), false);
}
