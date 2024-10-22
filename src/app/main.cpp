/**
 * @file main.cpp
 * @brief The entry point for the Invoice Manager application.
 * @copyright Copyright 2024 Manel Jimeno. All rights reserved.
 * @author Manel Jimeno <manel.jimeno@gmail.com>
 * @date 2024
 * @license MIT http://www.opensource.org/licenses/mit-license.php
 */

#include "invoice_manager_app.h"

/**
 * @brief The main function, entry point of the application.
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments.
 * @return The exit status of the application.
 */
int main(int argc, char* argv[])
{
    InvoiceManagerApp app(argc, argv);
    return app.loop();
}
