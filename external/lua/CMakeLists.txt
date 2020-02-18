cmake_minimum_required(VERSION 3.5)
project(lua)

set(LUA_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lua/liblua.a)
set(LUA_TARGET liblua)

add_custom_command(
                   OUTPUT ${LUA_LIB}
                   COMMAND ${CMAKE_MAKE_PROGRAM}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lua/
)
add_custom_target(liblua_target DEPENDS  ${LUA_LIB})
add_library (${LUA_TARGET} STATIC IMPORTED GLOBAL)
target_link_libraries(${LUA_TARGET} INTERFACE dl)

set_target_properties(${LUA_TARGET}
    PROPERTIES
    IMPORTED_LOCATION ${LUA_LIB}
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/lua/

    add_dependencies(${LUA_TARGET} liblua_target)