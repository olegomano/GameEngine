#ifndef _GL_VBO_H_
#define _GL_VBO_H_
#include <cstdint>
#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <iostream>
#include <initializer_list>

#include"utils.h"


namespace render{
namespace gl{

class VBO;

class VBOAttrib{
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

    inline const VBO* const owner() const {return m_owner;}
    inline uint32_t  stride()     const {return m_stride;}
    inline uint32_t  offset()     const {return m_offset;}
    inline uint32_t  count()      const {return m_count;}
    GLuint           glHandle()   const;
private:
    VBO*     m_owner;
    uint32_t m_stride;
    uint32_t m_offset;
    uint32_t m_count;
};

class VBO{
public:
    
     /**
     *@size in bytes
     */
    void create(uint8_t* data, uint32_t size){
        std::cout << " Creating VBO size " << size << " bytes " << std::endl;
        create();
        update(data,size);
    }

    /**
     *@size in bytes
     */
    void update(uint8_t* data, uint32_t size){
      std::string error = "";
      glBindBuffer(GL_ARRAY_BUFFER, m_handle);
      glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
      _check_gl_error("Update Buffer",error);
      if(error.size() > 0){
          std::cout << " error updating VBO " << error << std::endl;
      }
    } 
  
    template<typename T>
    void create(const T& floats){
        float* data = new float[floats.size()];
        int count = 0;
        for(float f : floats){
            data[count++] = f;
        }
        create((uint8_t*)data,floats.size() * sizeof(float));
        delete[] data;
    }

    template<typename T>
    void update(const T& floats){
        float* data = new float[floats.size()];
        int count = 0;
        for(float f : floats){
            data[count++] = f;
        }
        update((uint8_t*)data,floats.size() * sizeof(float));
        delete[] data;
    }

    void create(){
        std::string error = "";          
        if(m_handle != -1) destroy();
        glGenBuffers(1,&m_handle);
        _check_gl_error("Gen Buffers",error);
        if(error.size() > 0){
            std::cout << " error creating VBO " << error << std::endl;
        }
    }


    template<typename T>
    void updateMap(const T& callback){
      std::string error = "";  
      void *ptr = map();
      _check_gl_error("MapBuffer",error);
      callback(ptr);
      _check_gl_error("FillBuffer",error);
      unmap();
      _check_gl_error("UnmapBuffer",error);
      if(error.size() > 0){
        std::cout << "error mapping VBO " << error << std::endl;
      }
    }

    void* map(){
        glBindBuffer(GL_ARRAY_BUFFER, m_handle);
        return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    }

    void unmap(){
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    /*
    void bind(){
        glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    }
    */

    void destroy(){
      std::cout << "Destroying VBO" << std::endl;
    }

    inline GLuint handle() const {
        return m_handle;
    }

    VBOAttrib createAttrib(uint32_t count,uint32_t offset, uint32_t stride){
        return VBOAttrib(this,offset,stride,count);
    }
private:
    GLuint m_handle = -1; 
};



};
};


#endif
