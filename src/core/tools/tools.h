/**
 * @file tools.h
 * @brief Header file for the tools functions
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once
#include "dllexports.h"
#include <QString>

namespace core::tools
{
/**
 * @return a temporary name without extension.
 */
[[nodiscard]] CORE_API QString getTemporaryFileName(const QString& extension = "");
[[nodiscard]] CORE_API QString capitalizeFirstLetter(const QString& input);
[[nodiscard]] CORE_API QString upperSnake(const QString& input);
[[nodiscard]] CORE_API QString lowerSnake(const QString& input);
CORE_API void saveStringToFile(const QString& text, const QString& filePath);
[[nodiscard]] CORE_API QVector<QString> extractBoundFields(const QString& query);
[[nodiscard]] CORE_API bool areFilesEqual(const QString& filePath1, const QString& filePath2);

} // namespace core::tools
