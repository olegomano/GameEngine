cmake_minimum_required(VERSION 3.5)
project(render)
find_package(OpenGL REQUIRED)

add_library(renderer STATIC src/shader.cpp 
  src/color_shader.cpp 
  src/transform.cpp 
  src/gl/buffer.cpp
  src/scene.cpp
  #src/scene_events.cpp
  src/entity.cpp
  )
target_include_directories(renderer PRIVATE ${OPENGL_INCLUDE_DIRS})
target_include_directories(renderer PUBLIC core)
target_include_directories(renderer INTERFACE include/)
target_compile_options(renderer PRIVATE ${COMPILE_FLAGS})
target_link_libraries(renderer glm ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} core)
