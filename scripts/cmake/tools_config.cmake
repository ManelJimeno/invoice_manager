#
# Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: https://www.opensource.org/licenses/mit-license.php MIT
#

# Function to generate .cpp files from .json files
function(generate_json_sources target json_dir generated_dir)
    message(STATUS "Loading JSON files from ${json_dir} the output directory is ${generated_dir}")

    add_custom_target(
        ${target}_gen_tables
        COMMAND db_api_generator --json-dir ${json_dir} --output ${generated_dir} --db-type QSQLITE --connection-info
                "${CMAKE_SOURCE_DIR}/src/app/db/${target}.db"
        COMMENT "Generating source code from ${json_dir} to ${generated_dir}"
        VERBATIM)

    add_dependencies(${target}_gen_tables db_api_generator)
endfunction()

# Function to generate .cpp files from .json files
function(generate_qml_sources target menu_dir generated_dir)
    message(STATUS "Loading JSON files from ${json_dir} the output directory is ${generated_dir}")

    add_custom_target(
        ${target}_gen_menus
        COMMAND menu_generator --source-dir ${menu_dir} --output ${generated_dir}
        COMMENT "Generating QML code from ${menu_dir} to ${generated_dir}"
        VERBATIM)

    add_dependencies(${target}_gen_tables db_api_generator)
endfunction()
