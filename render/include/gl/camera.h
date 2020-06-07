#ifndef _RENDER_GL_CAMERA_H_
#define _RENDER_GL_CAMERA_H_
#include "../camera.h"
#include "texture.h"
#include "framebuffer.h"

#include <log.h>

namespace render{
namespace gl{

class GLCamera : public ICamera{
public:
  void create(uint32_t w, uint32_t h) override {
    ICamera::create(w,h);
    m_framebuffer.create(w,h,cprint_debug("GLCamera"));
  }

  void blitToScreen(uint32_t l, uint32_t t, uint32_t r, uint32_t b) override {
    std::string error = "";

    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebuffer.fbo());
    _check_gl_error("glBindFrameBuffer_read",error);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
    _check_gl_error("glBindFrameBuffer_write",error); 
    glBlitFramebuffer(0, 0, m_framebuffer.width(), m_framebuffer.height(), l, t, r-l, b-t, GL_COLOR_BUFFER_BIT,GL_NEAREST);  
    _check_gl_error("glBlitFramebuffer",error);

    if(error.size() > 0){
      cprint_error("GLCamera") << error << std::endl;
    }
  }

  void bind() const{
    std::string error = "";
    m_framebuffer.bind();
    glViewport(0,0,m_framebuffer.width(),m_framebuffer.height());
    _check_gl_error("framebuffer.bind()",error);
    if(error.size() != 0){
      cprint_debug("GLCamera") << error << std::endl;
    }
  }

  void unbind() const {
    m_framebuffer.unbind();
  }
private:
  //GLTexture m_renderTexture;
  Framebuffer m_framebuffer; 
};

}
}


#endif
