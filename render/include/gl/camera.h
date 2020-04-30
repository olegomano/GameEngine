#ifndef _RENDER_GL_CAMERA_H_
#define _RENDER_GL_CAMERA_H_
#include "../camera.h"
#include "texture.h"
#include "framebuffer.h"

namespace render{
namespace gl{

class GLCamera : public ICamera{
protected:
  void create() override {

  }

  void blitToScreen() override {
  
  }

  void bind() override {
  
  }
private:
  //GLTexture m_renderTexture;
  //Framebuffer m_framebuffer; 
};

}
}


#endif
