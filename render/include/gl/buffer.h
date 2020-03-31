#ifndef _GL_VBO_H_
#define _GL_VBO_H_
#include <cstdint>
#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <iostream>
#include <initializer_list>

#include "utils.h"
#include "../buffer.h"

namespace render{
namespace gl{

class VBOAttrib;

class VBO : public render::IBuffer{
  public:
  void create(uint32_t size) override{
    std::string error = "";          
    if(m_handle != -1) destroy();
    glGenBuffers(1,&m_handle);
    _check_gl_error("Gen Buffers",error);
    if(error.size() > 0){
      std::cout << " error creating VBO " << error << std::endl;
      return;
    }
    write(nullptr,size);
  }
  
  void write(uint8_t* buffer, uint32_t count) override{
    glBindBuffer(GL_ARRAY_BUFFER,m_handle);
    glBufferData(GL_ARRAY_BUFFER,count,buffer,GL_STATIC_DRAW);
  }
  
  void write(uint8_t* buffer, uint32_t offset,uint32_t count) override{
    std::cout << "Warning calling unimplemented function write(buffer,offset,count) " << std::endl;
  }

   

  void* map(){
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  }

  void unmap(){
    glUnmapBuffer(GL_ARRAY_BUFFER);
  }

  void destroy(){
    std::cout << "Destroying VBO" << std::endl;
  }

  inline GLuint handle() const {
    return m_handle;
  }
  
  VBOAttrib createAttrib(uint32_t count,uint32_t offset, uint32_t stride);
private:
  GLuint m_handle = -1; 
};

class VBOAttrib : public render::IAttribute{
  friend std::ostream& operator<<(std::ostream& out, const VBOAttrib& attrib); 
public:
    VBOAttrib():
    m_owner(nullptr)
    {

    }

    VBOAttrib(VBO* owner, uint32_t offset, uint32_t stride, uint32_t count):
    m_owner(owner),
    m_stride(stride),
    m_offset(offset),
    m_count(count)
    {

    }

    VBOAttrib(const VBOAttrib& other):
    m_owner(other.m_owner),
    m_stride(other.m_stride),
    m_offset(other.m_offset),
    m_count(other.m_count)
    {

    }

    render::IBuffer&  owner() {return *m_owner;}
    const VBO* const ownerVBO() const {return m_owner;}
    uint32_t  stride() const  {return m_stride;}
    uint32_t  offset() const  {return m_offset;}
    uint32_t  count()  const  {return m_count;}
private:
    VBO*     m_owner;
    uint32_t m_stride;
    uint32_t m_offset;
    uint32_t m_count;
};


};
};

render::gl::VBOAttrib render::gl::VBO::createAttrib(uint32_t count,uint32_t offset, uint32_t stride){
  return render::gl::VBOAttrib(this,offset,stride,count);
}

#endif
