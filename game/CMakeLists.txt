cmake_minimum_required(VERSION 3.5)
project(Game)
#set(COMPILE_FLAGS "-std=c++17" "-g")

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)


add_executable(game main.cpp src/context.cpp src/config.cpp src/luaEntity.cpp)
target_include_directories(game PRIVATE ${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
target_link_libraries(game ${SDL2_LIBRARIES} ${GLEW_LIBRARIES})
target_link_libraries(game liblua)
target_link_libraries(game core)
target_link_libraries(game renderer)
target_compile_options(game PRIVATE ${COMPILE_FLAGS})
