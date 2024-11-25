#
# Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: https://www.opensource.org/licenses/mit-license.php MIT
#

# Set up deployment for Qt applications Arguments: application_name - The name of the application target for which to
# set up Qt deployment
function(setup_qt_deployment application_name)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(qt_BIN_DIRS ${qt_BIN_DIRS_DEBUG})
    else()
        set(qt_BIN_DIRS ${qt_BIN_DIRS_RELEASE})
    endif()

    set(qml_lint "${qt_BIN_DIRS}/qmllint")
    set(qml_format "${qt_BIN_DIRS}/qmlformat")

    if(WIN32)
        set(qml_lint "${qml_lint}.exe")
        string(REPLACE "/" "\\\\" qml_lint "${qml_lint}")
        set(qml_format "${qml_format}.exe")
        string(REPLACE "/" "\\\\" qml_format "${qml_format}")
    endif()

    configure_file("${CMAKE_SOURCE_DIR}/.pre-commit-config.yaml.in" "${CMAKE_SOURCE_DIR}/.pre-commit-config.yaml" @ONLY)

    if(WIN32)
        add_custom_command(
            TARGET ${application_name}
            POST_BUILD
            COMMAND ${qt_BIN_DIRS}/windeployqt.exe $<TARGET_FILE:${application_name}>
            COMMENT "Running windeployqt to include Qt dependencies")
    endif()
    if(APPLE)
        add_custom_command(
            TARGET ${application_name}
            POST_BUILD
            COMMAND ${qt_BIN_DIRS}/macdeployqt "${CMAKE_BINARY_DIR}/bin/${application_name}.app"
            COMMENT "Running macdeployqt to bundle Qt libraries")
    endif()
    if(UNIX AND NOT APPLE)
        install(FILES ${Qt6Gui_LIBRARIES} ${Qt6Widgets_LIBRARIES} ${Qt6Quick_LIBRARIES} ${Qt6Qml_LIBRARIES}
                DESTINATION lib)
    endif()
    install(
        DIRECTORY ${CMAKE_SOURCE_DIR}/qml
        DESTINATION qml
        COMPONENT ${COMPONENT_RUNTIME}
        FILES_MATCHING
        PATTERN "*.qml")
    install(
        DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
        DESTINATION bin
        COMPONENT ${COMPONENT_RUNTIME}
        FILES_MATCHING
        PATTERN "*.dll"
        PATTERN "*.exe"
        PATTERN "*.qml"
        PATTERN "ut_*" EXCLUDE)
    if(EXISTS "${CMAKE_BINARY_DIR}/bin/plugins/")
        install(
            DIRECTORY "${INSTALL_DIR}/plugins/"
            DESTINATION bin/plugins
            COMPONENT ${COMPONENT_RUNTIME})
    endif()
endfunction()
