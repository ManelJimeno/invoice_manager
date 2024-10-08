#
# Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: http://www.opensource.org/licenses/mit-license.php MIT
#

set(CMAKE_AUTOMOC ON) # (Meta-Object Compiler)
set(CMAKE_AUTORCC ON) # (Resource Compiler)

set(_app_sources main.cpp resources.qrc ui/main_window.cpp ui/main_window.h ui/main_window.qml)

add_executable(app ${_app_sources})

find_package(Qt6 REQUIRED COMPONENTS Widgets Quick Qml)
target_link_libraries(app PRIVATE invoice_core_obj Qt6::Gui Qt6::Widgets Qt6::Quick Qt6::Qml)
