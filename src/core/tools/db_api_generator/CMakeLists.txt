#
# Part of https://github.com/ManelJimeno/invoice_manager (C) 2024
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: http://www.opensource.org/licenses/mit-license.php MIT
#

# Set application name and sources
set(DB_API_GENERATOR "db_api_generator")
set(DB_API_GENERATOR_OBJ_LIBRARY ${DB_API_GENERATOR}_obj)
set(DB_API_GENERATOR_SOURCES db_api_generator.h db_api_generator.cpp db_class.cpp db_class.h statement.cpp statement.h)

# Use fmt library
find_package(fmt REQUIRED)

# Create the core object library target
add_library(${DB_API_GENERATOR_OBJ_LIBRARY} OBJECT ${DB_API_GENERATOR_SOURCES})
target_link_libraries(${DB_API_GENERATOR_OBJ_LIBRARY} PUBLIC ${INVOICE_CORE_OBJ_LIBRARY} fmt::fmt)

# Include the source directories for the object library
target_include_directories(${DB_API_GENERATOR_OBJ_LIBRARY} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add executable target
add_executable(${DB_API_GENERATOR} main.cpp)
# Link necessary dependencies
target_link_libraries(${DB_API_GENERATOR} PUBLIC ${DB_API_GENERATOR_OBJ_LIBRARY} ${INVOICE_CORE_OBJ_LIBRARY})

# Install the core library and its components (library, archive, runtime)
install(
    TARGETS ${DB_API_GENERATOR}
    EXPORT ${DB_API_GENERATOR}Targets
    LIBRARY DESTINATION lib COMPONENT ${CMAKE_COMPONENT_CORE}
    ARCHIVE DESTINATION lib COMPONENT ${CMAKE_COMPONENT_CORE}
    RUNTIME DESTINATION bin COMPONENT ${CMAKE_COMPONENT_CORE}
    INCLUDES
    DESTINATION include)

# Export the name of the object library to the parent scope
set(DB_API_GENERATOR_OBJ_LIBRARY
    "${DB_API_GENERATOR_OBJ_LIBRARY}"
    PARENT_SCOPE)
