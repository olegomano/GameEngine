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
  friend std::ostream& operator<<(std::ostream& out, const VBOAttrib& attrib); 
public:
  typedef VBOAttrib Attribute;
  void create(uint32_t size) override; 
  void write(uint8_t* buffer, uint32_t count) override; 
  void write(uint8_t* buffer, uint32_t offset,uint32_t count) override;
  void* map();
  void unmap();
  void destroy();

  inline GLuint handle() const {return m_handle;}
  
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
#endif
