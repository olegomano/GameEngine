#include "../include/shader.h"
#include <string>
#include <iostream>

static constexpr char* const vertex_shader =
"#version 400\n"
"uniform mat4 u_camera;"
"uniform mat4 u_model;"
"uniform vec4 u_color;"
"in vec4 a_vertex;"
"in vec4 a_normal;"
"void main() {"
"  gl_Position = u_model * a_vertex;"
"}";

static constexpr char* const fragment_shader =
"#version 400\n"
"out vec4 frag_colour;"
"void main() {"
"  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
"}";



void ColorShader::compile(){
    std::string error = "";

    GLuint fragmentShader;
    GLuint vertexShader;

    createShader(vertex_shader,GL_VERTEX_SHADER,vertexShader,error);
    createShader(fragment_shader,GL_FRAGMENT_SHADER,fragmentShader,error);
    linkShaders({fragmentShader,vertexShader},m_program,error);

    bindUniform(m_program, m_u_cameraMatrix, "u_camera",error);
    bindUniform(m_program,m_u_color, "u_color",error);
    bindUniform(m_program,m_u_modelMatrix,"u_model",error);
        
    bindAttribute(m_program,m_a_vertex,"a_vertex",error);
    bindAttribute(m_program,m_a_normal,"a_normal",error);

    if(error.size() > 0){
        std::cout << "Failed to compile ColorShader " << std::endl << error << std::endl; 
    }
}