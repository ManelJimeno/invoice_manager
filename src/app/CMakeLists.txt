#
# CMake configuration file for the invoice_manager project.
#
# This configuration sets up necessary Qt components, handles platform-specific settings, and configures the application
# build and installer.
#
# Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: http://www.opensource.org/licenses/mit-license.php MIT
#

# Enable Qt's Meta-Object Compiler (MOC) and Resource Compiler (RCC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set application name and sources
set(APPLICATION_NAME "invoice_manager")
set(APP_SOURCES
    main.cpp
    qml/resources.qrc
    qml/main_window.qml
    modules/module.cpp
    modules/module.h
    modules/security/security.cpp
    modules/security/security.h
    modules/security/groups.cpp
    modules/security/groups.h
    modules/security/users.cpp
    modules/security/users.h
    modules/ui/main_window.cpp
    modules/ui/main_window.h
    invoice_manager_app.cpp
    invoice_manager_app.h
    init_dialog.cpp
    init_dialog.h)
set(INSTALLER_NAME "Invoice manager")

# macOS Specific Configuration
if(APPLE)
    # Set application icon for macOS
    set(APPLICATION_ICNS "${APPLICATION_NAME}.icns")
    set(ICNS_SOURCE "${CMAKE_TOOLS_HOME}/osx_bundle/application.icns")
    set(ICNS_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_ICNS}")

    # Copy and rename icon file
    file(COPY ${ICNS_SOURCE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(RENAME "${CMAKE_CURRENT_BINARY_DIR}/application.icns" ${ICNS_DESTINATION})

    # Add the icon file to the application sources and set bundle properties
    list(APPEND APP_SOURCES ${ICNS_DESTINATION})
    set(MACOSX_BUNDLE_ICON_FILE ${APPLICATION_ICNS})
    set_source_files_properties(${ICNS_DESTINATION} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

# Windows Specific Configuration
if(WIN32)
    # Configure and add the resource file
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/icon.rc.in ${CMAKE_CURRENT_BINARY_DIR}/icon.rc)
    list(APPEND APP_SOURCES icon.rc)
endif()

# List of dependencies
find_package(Qt6 REQUIRED COMPONENTS Widgets Quick Qml Concurrent QuickControls2)
set(PROGRAM_DEPENDENCIES
    invoice_core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Qml
    Qt6::Sql
    Qt6::Concurrent)

# Add executable target
add_executable(${APPLICATION_NAME} ${APP_SOURCES})

# Add table classes generation
generate_json_sources(${APPLICATION_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/modules/security/tables"
                      "${CMAKE_CURRENT_SOURCE_DIR}/modules/security")

# Add QML menus
generate_qml_sources(${APPLICATION_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/modules/ui/menus"
                     "${CMAKE_CURRENT_SOURCE_DIR}/qml")

# Link necessary dependencies
target_link_libraries(${APPLICATION_NAME} PRIVATE ${PROGRAM_DEPENDENCIES})

# Include the source directories for the object library
target_include_directories(${APPLICATION_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${APPLICATION_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/modules)

# Setup Qt deployment and configure the installer
setup_qt_deployment(${APPLICATION_NAME})
configure_installer(${INSTALLER_NAME} ${APPLICATION_NAME})

if(APPLE)
    # Set target properties for macOS bundle
    set_target_properties(
        ${APPLICATION_NAME}
        PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info.plist"
                   MACOSX_BUNDLE_GUI_IDENTIFIER "com.manel_jimeno.${APPLICATION_NAME}"
                   MACOSX_BUNDLE TRUE)
endif()

# Optionally build documentation
if(INVOICE_BUILD_DOC)
    add_input_folder_to_doc(${CMAKE_CURRENT_SOURCE_DIR})
endif()

# Windows Specific Configuration
if(WIN32)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/icon.rc.in ${CMAKE_CURRENT_BINARY_DIR}/icon.rc)
    list(APPEND APP_SOURCES icon.rc)
endif()
