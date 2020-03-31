#include "../include/renderer.h"

/**
template<typename T>
void render::gl::Renderer<T>::init(){
    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);
    m_shader.compile();
    m_shader.enable();
    m_drawables.reserve(1024*4);
}

template<typename T>
void render::gl::Renderer<T>::render(const render::Camera& camera){
   // m_shader.enable();
    for(int i = 0; i < m_drawables.size(); i++){
        m_shader.draw(m_drawables[i]);
    }
    //m_shader.disable();
}
**/
