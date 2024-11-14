/**
 * @file tools.h
 * @brief Header file for the tools functions.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "dllexports.h" // Includes the dllexports header for platform-specific exports.
#include <QString>      // Includes the Qt QString class for string manipulation.

namespace core::tools
{
/**
 * @brief Generates a temporary file name with an optional extension.
 *
 * This function returns a string representing a temporary file name. The extension is optional
 * and can be specified; if not provided, the generated file name does not have an extension.
 *
 * @param extension The file extension to append to the temporary file name (default is empty).
 * @return A QString containing the generated temporary file name.
 */
[[nodiscard]] CORE_API QString getTemporaryFileName(const QString& extension = "");

/**
 * @brief Capitalizes the first letter of the input string.
 *
 * This function takes an input string and returns a new string where the first letter is capitalized
 * and the rest of the string remains unchanged.
 *
 * @param input The input string to process.
 * @return A QString with the first letter capitalized.
 */
[[nodiscard]] CORE_API QString capitalizeFirstLetter(const QString& input);

/**
 * @brief Converts the input string to uppercase with underscores separating words.
 *
 * This function takes an input string and transforms it into uppercase letters while converting any
 * spaces or separators into underscores. It is commonly used for converting strings into a format
 * suitable for constants (e.g., `MY_CONSTANT`).
 *
 * @param input The input string to convert.
 * @return A QString representing the input string in uppercase with underscores.
 */
[[nodiscard]] CORE_API QString upperSnake(const QString& input);

/**
 * @brief Converts the input string to lowercase with underscores separating words.
 *
 * Similar to `upperSnake`, this function converts the input string to lowercase and replaces any
 * spaces or separators with underscores. It is typically used for variable names or file naming conventions.
 *
 * @param input The input string to convert.
 * @return A QString representing the input string in lowercase with underscores.
 */
[[nodiscard]] CORE_API QString lowerSnake(const QString& input);

/**
 * @brief Saves a string to a file.
 *
 * This function takes a string and writes it to a specified file path. If the file already exists,
 * it will be overwritten.
 *
 * @param text The string to save to the file.
 * @param filePath The path where the string should be saved.
 */
CORE_API void saveStringToFile(const QString& text, const QString& filePath);

/**
 * @brief Extracts bound fields from a SQL query.
 *
 * This function analyzes a SQL query (e.g., SELECT or INSERT) and extracts any bound fields used
 * in the query. Bound fields are placeholders (e.g., `?`) in the SQL query that will be replaced
 * with actual values when the query is executed.
 *
 * @param query The SQL query from which to extract the bound fields.
 * @return A QVector of QStrings containing the names of the bound fields.
 */
[[nodiscard]] CORE_API QVector<QString> extractBoundFields(const QString& query);

/**
 * @brief Compares two files to check if they are identical.
 *
 * This function compares two files at the given file paths to determine if they are the same.
 * It compares the contents of the files and returns true if they are identical, otherwise false.
 *
 * @param filePath1 The path to the first file.
 * @param filePath2 The path to the second file.
 * @return A boolean indicating whether the files are identical.
 */
[[nodiscard]] CORE_API bool areFilesEqual(const QString& filePath1, const QString& filePath2);

} // namespace core::tools
