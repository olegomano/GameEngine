#ifndef _RENDER_GL_FRAMEBUFFER_
#define _RENDER_GL_FRAMEBUFFER_
#include <cstdint>
#include <GL/glew.h>
#include <GL/gl.h>
#include <ostream>

namespace render{
namespace gl{

enum FbAttachment{
  Color,
  Depth,
  Stencil
};

class Framebuffer{
public:
  bool create(uint32_t width, uint32_t height,std::ostream& error){
    m_width = width;
    m_height = height; 
    std::string errorString;

    glGenTextures(1, &m_rgbTexture);
    glBindTexture(GL_TEXTURE_2D, m_rgbTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE); // automatic mipmap
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    _check_gl_error("genTexture",errorString);
    // create a renderbuffer object to store depth info
    glGenRenderbuffers(1, &m_renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    _check_gl_error("genRenderBuffer",errorString);
    // create a framebuffer object
    glGenFramebuffers(1, &m_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderBuffer);
    _check_gl_error("getFrameBuffer",errorString);

    // attach the texture to FBO color attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
                       GL_COLOR_ATTACHMENT0,  // 2. attachment point
                       GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                       m_rgbTexture,             // 4. tex ID
                       0);                    // 5. mipmap level: 0(base)

    _check_gl_error("color attachment",errorString);
    // attach the renderbuffer to depth attachment point
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
                          GL_DEPTH_ATTACHMENT, // 2. attachment point
                          GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
                          m_renderBuffer);              // 4. rbo ID
    _check_gl_error("depth attachment",errorString);
    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    bool res = status != GL_FRAMEBUFFER_COMPLETE;

    // switch back to window-system-provided framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    _check_gl_error("final",errorString);
    error << errorString << std::endl;
    return res;
  }

  inline void bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER,m_frameBuffer);
  }

  inline void unbind() const{
    glBindFramebuffer(GL_FRAMEBUFFER,0);
  }

  inline GLuint fbo() const {
    return m_frameBuffer;
  }

  inline GLuint rgbTexture() const{
    return m_rgbTexture;
  }

  inline GLuint depthTexture() const{
    return m_depthBuffer;
  }

  inline uint32_t width() const{
    return m_width;
  }

  inline uint32_t height() const{
    return m_height;
  }

  bool destroy(){return true;}
private:
  GLuint m_frameBuffer;
  GLuint m_renderBuffer;
  GLuint m_rgbTexture;
  GLuint m_depthBuffer;
  uint32_t m_width;
  uint32_t m_height;
};


};
};
#endif
