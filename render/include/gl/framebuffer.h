#ifndef _RENDER_GL_FRAMEBUFFER_
#define _RENDER_GL_FRAMEBUFFER_
#include <cstdint>
#include <GL/glew.h>
#include <GL/gl.h>
#include <ostream>

namespace render{
namespace gl{

class Framebuffer{
public:
  bool create(uint32_t width, uint32_t height, std::ostream& error){
    glGenFramebuffers(1,&m_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,m_frameBuffer);
    
    glGenRenderbuffers(1,&m_depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER,m_depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,width,height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,m_depthBuffer);
    return true;
  }

  bool bind(GLuint textureId){
    glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,textureId,0);
    GLenum drawbuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1,drawbuffers);
    return true;
  }

  bool destroy(){return true;}
private:
  GLuint m_frameBuffer;
  GLuint m_depthBuffer;
};


};
};
#endif
