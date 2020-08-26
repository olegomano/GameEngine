#include "../include/gl/shader.h"
#include "../include/gl/utils.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

void Shader::enable(){
    std::string error = "";
    glUseProgram(m_program);
    _check_gl_error("   glUseProgram ",error);

    for(GLuint* attr : m_attributes){
        glEnableVertexAttribArray(*attr);
        _check_gl_error("   glEnableVertexAttribArray ",error);
    }
    if(error.size() > 0){
        std::cout << "Error Enabling Shader: " << std::endl <<  error << std::endl;
    }
}

void Shader::disable(){
    std::string error = "";
    for(GLuint* attr : m_attributes){
        glDisableVertexAttribArray(*attr);
        _check_gl_error("   glDisableVertexAttribArray ",error);
    }
    if(error.size() > 0){
        std::cout << "Error Disabling Shader: " << std::endl <<  error << std::endl;
    }
}

bool Shader::createShader(const std::string& shader, GLuint type, GLuint& handle,std::string& error){
    handle = glCreateShader(type);
    const GLchar *source = (const GLchar *)shader.c_str();
    glShaderSource(handle, 1, &source, 0);
    glCompileShader(handle);

    GLint isCompiled = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE){

        GLint maxLength;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &maxLength);
	    std::vector<GLchar> infoLog(maxLength);
	    glGetShaderInfoLog(handle, maxLength, &maxLength, &infoLog[0]);

        for(int i =0; i < infoLog.size(); i++){
            error+=infoLog[i];
        }

	    glDeleteShader(handle);
	    return false;;
    }
    return true;
}

bool Shader::linkShaders(std::initializer_list<GLuint> shaders, GLuint& handle, std::string& error){
    handle = glCreateProgram();
    for(GLuint s : shaders){
        glAttachShader(handle, s);
    }
    glLinkProgram(handle);
    GLint isLinked = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE){

        GLint maxLength;
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &maxLength);
	    std::vector<GLchar> infoLog(maxLength);
	    glGetProgramInfoLog(handle, maxLength, &maxLength, &infoLog[0]);

        for(int i =0; i < infoLog.size(); i++){
            error+=infoLog[i];
        }
        glDeleteProgram(handle);
        return false;
    }
    return true;
}

bool Shader::bindUniform(GLuint program,GLuint& uniform,const std::string& name,std::string& error){
    uniform = glGetUniformLocation(program,name.c_str());
    return _check_gl_error("Failed bind Uniform " + name,error);
}

bool Shader::bindAttribute(GLuint program,GLuint& attr,const std::string& name, std::string& error){
    attr   = glGetAttribLocation(program,name.c_str());
    return _check_gl_error("Failed bind Attribute " + name,error);
}

template<>
bool Shader::setUniform<glm::mat4>(GLuint uniform, const glm::mat4& data, std::string& error){
    glUniformMatrix4fv(uniform,1,false,glm::value_ptr(data));
    return _check_gl_error("Failed tp Set Matrix Uniform ",error);
}

template<>
bool Shader::setUniform<double>(GLuint uniform, const double& data, std::string& error){
    glUniform1d(uniform,data);
    return _check_gl_error("Failed tp Set Double Uniform ",error);
}

template<>
bool Shader::setUniform<float>(GLuint uniform, const float& data, std::string& error){
    glUniform1f(uniform,data);
    return _check_gl_error("Failed tp Set Double Uniform ",error);
}


