#include "../include/gl/shader.h"
#include <string>
#include <iostream>

static constexpr char* const vertex_shader =
"#version 400\n"
"uniform mat4 u_camera;"
"uniform mat4 u_model;"
"uniform vec4 u_color;"
"in vec4 a_vertex;"
"in vec4 a_normal;"
"in vec2 a_uv;"
"out vec2 a_uv_frag;"
"void main() {"
"  gl_Position = u_camera * ( u_model * a_vertex );"
"  gl_Position.w = gl_Position.z;"
"  gl_Position.z = (gl_Position.z - 1)/100;"
"  a_uv_frag = a_uv;"
"}";

static constexpr char* const fragment_shader =
"#version 400\n"
"in vec2 a_uv_frag;"
"out vec4 frag_colour;"
"void main() {" 
"  frag_colour = vec4(a_uv_frag.x,a_uv_frag.y,1,1.0);"
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
    bindAttribute(m_program,m_a_uv,"a_uv",error);
    if(error.size() > 0){
        std::cout << "Failed to compile ColorShader " << std::endl << error << std::endl; 
    }
}
