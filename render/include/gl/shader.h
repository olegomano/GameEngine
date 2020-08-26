#ifndef _RENDER_SHADER_H_
#define _RENDER_SHADER_H_
#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <initializer_list>
#include <vector>
#include "../transform.h"
#include "buffer.h"
#include <core.h>

class Shader{
public:
    void enable();
    void disable();
    virtual void compile() = 0;
    virtual ~Shader(){}
protected:
    bool createShader(const std::string& shader, GLuint type, GLuint& handle,std::string& error);
    bool linkShaders(std::initializer_list<GLuint> shaders,GLuint& handle, std::string& error);    
    bool bindUniform(GLuint program,GLuint& uniform,const std::string& name,std::string& error);
    bool bindAttribute(GLuint program,GLuint& attr,const std::string& name, std::string& error);

    template<typename T>
    bool setUniform(GLuint uniform, const T& data, std::string& error);
    
    template<int ELEMENTS>
    bool setAttribute(GLuint attribute, const render::gl::VBOAttrib& attrib, std::string& error){
        glBindBuffer(GL_ARRAY_BUFFER, attrib.ownerVBO()->handle());
        _check_gl_error("Failed to bind buffer ",error);
        glVertexAttribPointer(attribute, ELEMENTS, GL_FLOAT, GL_FALSE, attrib.stride(), (void*)attrib.offset());
        return _check_gl_error("Failed to set attribute ",error);
    }
protected:
    GLuint               m_program;
    std::vector<GLuint*> m_uniforms;
    std::vector<GLuint*> m_attributes;
};

class ColorShader : public Shader{
public:
    ColorShader(){
        m_uniforms   = {&m_u_color,&m_u_modelMatrix,&m_u_cameraMatrix};
        m_attributes = {&m_a_vertex,&m_a_uv};
    }
    
    void compile();
    
    /**
    template<typename T>
    void draw(const Transform& cameraMatrix, const Transform& modelMatrix, const T&& item){
        using render::gl::VBOAttrib;
        std::string error = "";

        ///Transform modelMatrix  = item.position;
        VBOAttrib vertexAttrib = item.vertex;
        VBOAttrib uvAttrib = item.uv;    
        
        setAttribute<4>(m_a_vertex,vertexAttrib,error);
        setAttribute<2>(m_a_uv,uvAttrib,error);
        setUniform(m_u_modelMatrix,modelMatrix.transform(),error);
        glDrawArrays(GL_TRIANGLES,0,vertexAttrib.count());
        _check_gl_error("Draw Arrays",error);

        if(error.size() > 0){
            std::cout << "ERROR: color shader " << std::endl << error << std::endl;
        }
    }
    **/

    template<typename T>
    void draw(const glm::mat4& projection, const glm::mat4& camera, const glm::mat4& model,const T& item){
        using render::gl::VBOAttrib;
        std::string error = "";

        VBOAttrib vertexAttrib = item.vertex; 
        VBOAttrib uvAttrib = item.uv;    
       
        setAttribute<4>(m_a_vertex,vertexAttrib,error);
        setAttribute<2>(m_a_uv,uvAttrib,error);
        setUniform(m_u_cameraMatrix,projection*camera, error);
        setUniform(m_u_modelMatrix,model,error);
        glDrawArrays(GL_TRIANGLES,0,vertexAttrib.count());
        _check_gl_error("Draw Arrays",error);
        
        if(error.size() > 0){
            std::cout << "ERROR: color shader " << std::endl << error << std::endl;
        }
    }

    ~ColorShader(){

    }
private:
    GLuint m_u_color;
    GLuint m_u_modelMatrix;
    GLuint m_u_cameraMatrix;

    GLuint m_a_vertex;
    GLuint m_a_normal;
    GLuint m_a_uv;
};


#endif
