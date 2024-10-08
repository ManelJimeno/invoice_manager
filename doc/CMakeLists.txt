#
# Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: http://www.opensource.org/licenses/mit-license.php MIT
#

include(doc_config)
set(PRODUCT_DOC_TEMPLATES ${CMAKE_CURRENT_SOURCE_DIR}/templates)
set(PRODUCT_DOC_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sphinx/output)

add_subdirectory(doxygen)
add_subdirectory(sphinx)

create_documentation_target()
install(
    DIRECTORY ${PRODUCT_DOC_OUTPUT}
    DESTINATION ${INSTALL_DOCDIR}
    COMPONENT ${COMPONENT_DOCUMENTATION})
