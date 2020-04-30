cmake_minimum_required(VERSION 3.5)
project(Core)

add_library(core STATIC src/luatypes.cpp src/log.cpp src/utils.cpp src/unixfile.cpp src/file.cpp)
target_include_directories(core INTERFACE include/)
target_compile_options(core PRIVATE ${COMPILE_FLAGS})
target_link_libraries(core PUBLIC liblua)
target_link_libraries(core PUBLIC glm)
