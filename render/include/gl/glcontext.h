#ifndef _RENDER_GL_CONTEXT_
#define _RENDER_GL_CONTEXT_
#include "shader.h"
#include "buffer.h"
#include "camera.h"
#include "texture.h"
#include "../rcontext.h"
#include <log.h>
#include <sstream>

namespace render{
namespace gl{

struct GLDrawable{
  VBOAttrib vertex;
  VBOAttrib uv;
};

class GLDefaultRenderer{
public:
  void create(){
    GLuint vao;
    std::string ss;
    glGenVertexArrays(1, &vao);
    _check_gl_error("glCreateVertexArrays",ss);
    glBindVertexArray(vao);
    _check_gl_error("glBindVertexArray",ss);
    glEnableVertexAttribArray(0);
    _check_gl_error("glEnableVertexArray",ss);
    m_colorShader.compile();
    _check_gl_error("compile",ss);
    cprint_debug("GLDefaultRenderer") << "Create Errors: " << ss << std::endl;
  }

  template<typename T>
  void draw(T& scene){
    m_colorShader.enable();
    const auto& drawables = scene.drawables();
    const auto& cameras = scene.cameras();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    for(const auto& c :cameras){
      c.component.bind();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      for(const auto& d : drawables){
        m_colorShader.draw(c.component.projection(),c.transform.invTransform(),d.transform.transform(),d.component);
      }
      c.component.unbind();
    }
    m_colorShader.disable();
  }

private:
  ColorShader m_colorShader;
};

/**
 *camera transform
 *camera render-to-texture
 *multi-window
 *
 */

class GLContext : public ::render::RenderContext<GLTexture,GLCamera,VBO,GLDrawable>{
public:
  void create() override{
    cprint_debug("GLContext") << "Create" << std::endl;
    ::render::RenderContext<GLTexture,GLCamera,VBO,GLDrawable>::create();
    m_renderer.create();
  }
  
  void render() override{
    m_scene.update();
    m_renderer.draw(m_scene);  
  };
private:
  GLDefaultRenderer m_renderer;
  
};

}
}


#endif
