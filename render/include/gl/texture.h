#ifndef _RENDER_GL_TEXTURE_
#define _RENDER_GL_TEXTURE_
#include <cstdint>
#include <GL/glew.h>
#include <GL/gl.h>

#include "../texture.h"
namespace render{
namespace gl{

class GLTexture : public ITexture{
public:
  GLuint handle() const {return m_handle;} 
protected:
  void createImpl() override{
   glGenTextures(1,&m_handle);
    glBindTexture(GL_TEXTURE_2D,m_handle);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_width,m_height,0,GL_RGB,GL_UNSIGNED_BYTE,0);
  }

  void write(uint8_t* pixels) override{
    glBindTexture(GL_TEXTURE_2D,m_handle);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_width,m_height,0,GL_RGB,GL_UNSIGNED_BYTE,pixels);
  }

  void destroy() override{
  
  }
private:
  GLuint   m_handle;
};

}}


#endif
