cmake_minimum_required(VERSION 3.5)
project(LuaBuild)
add_custom_target(liblua_target ALL
  COMMAND ${CMAKE_MAKE_COMMAND}
  WORKING_DIRECTORY "lua/"
)

add_library(liblua STATIC IMPORTED GLOBAL)
set_target_properties(liblua
  PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lua/liblua.a")
target_include_directories(liblua INTERFACE "lua/")
add_dependencies(liblua liblua_target)
target_link_libraries(liblua INTERFACE ${CMAKE_DL_LIBS})
