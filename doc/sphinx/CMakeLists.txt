#
# Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: http://www.opensource.org/licenses/mit-license.php MIT
#

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in ${CMAKE_CURRENT_BINARY_DIR}/conf.py)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/index.rst.in ${CMAKE_CURRENT_BINARY_DIR}/index.rst)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/settings.rst.in ${CMAKE_CURRENT_BINARY_DIR}/settings.rst)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/classes-and-functions.rst
               ${CMAKE_CURRENT_BINARY_DIR}/classes-and-functions.rst @ONLY)
