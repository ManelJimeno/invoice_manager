#
# Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: http://www.opensource.org/licenses/mit-license.php MIT
#

# Set application name and sources
set(MENU_GENERATOR "menu_generator")
set(MENU_GENERATOR_OBJ_LIBRARY ${MENU_GENERATOR}_obj)
set(MENU_GENERATOR_SOURCES menu_generator.h menu_generator.cpp)

# Use fmt library
find_package(fmt REQUIRED)

# Create the core object library target
add_library(${MENU_GENERATOR_OBJ_LIBRARY} OBJECT ${MENU_GENERATOR_SOURCES})
target_link_libraries(${MENU_GENERATOR_OBJ_LIBRARY} PUBLIC ${INVOICE_CORE_OBJ_LIBRARY} fmt::fmt)

# Include the source directories for the object library
target_include_directories(${MENU_GENERATOR_OBJ_LIBRARY} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add executable target
add_executable(${MENU_GENERATOR} main.cpp)
# Link necessary dependencies
target_link_libraries(${MENU_GENERATOR} PUBLIC ${MENU_GENERATOR_OBJ_LIBRARY} ${INVOICE_CORE_OBJ_LIBRARY})

# Install the core library and its components (library, archive, runtime)
install(
    TARGETS ${MENU_GENERATOR}
    EXPORT ${MENU_GENERATOR}Targets
    LIBRARY DESTINATION lib COMPONENT ${CMAKE_COMPONENT_CORE}
    ARCHIVE DESTINATION lib COMPONENT ${CMAKE_COMPONENT_CORE}
    RUNTIME DESTINATION bin COMPONENT ${CMAKE_COMPONENT_CORE}
    INCLUDES
    DESTINATION include)

# Export the name of the object library to the parent scope
set(MENU_GENERATOR_OBJ_LIBRARY
    "${MENU_GENERATOR_OBJ_LIBRARY}"
    PARENT_SCOPE)
