cmake_minimum_required(VERSION 3.5)
project(Galacoli)
add_subdirectory(render)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

set(COMPILE_FLAGS "-fno-exceptions" "-Wall" "-O3" "-fno-omit-frame-pointer" "-flto")

add_executable(test_1 test/test_1.cpp src/sdlcontext.cpp)
target_include_directories(test_1 PRIVATE ${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
target_link_libraries(test_1 ${SDL2_LIBRARIES} ${GLEW_LIBRARIES})

#add_executable(test_2 test/test_2.cpp)
add_executable(test_linkedlist test/test_linkedlist.cpp)
target_compile_options(test_linkedlist PRIVATE ${COMPILE_FLAGS})
