cmake_minimum_required(VERSION 3.5)
project(Galacoli)
#set(COMPILE_FLAGS "-std=c++17" "-g")
set(COMPILE_FLAGS "-std=c++17" "-fno-exceptions" "-Wall" "-O3" "-fno-omit-frame-pointer" "-flto" "-fno-rtti" "-Werror=return-type" )


add_subdirectory(3rdparty)
add_subdirectory(core)
add_subdirectory(render)
add_subdirectory(game)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)


add_executable(test_linkedlist test/test_linkedlist.cpp)
target_link_libraries(test_linkedlist glm)
target_link_libraries(test_linkedlist core)
target_compile_options(test_linkedlist PRIVATE ${COMPILE_FLAGS})


add_executable(test_lua test/test_lua.cpp)
target_link_libraries(test_lua liblua)
target_link_libraries(test_lua core)
target_compile_options(test_lua PRIVATE ${COMPILE_FLAGS})
