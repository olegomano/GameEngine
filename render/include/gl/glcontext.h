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
    for(const auto& d : drawables){
    //  cprint_debug("GLDefaultRenderer") << "Drawing entity " << d.entityId << std::endl;
      m_colorShader.draw(d.transform,d.component);
      cprint_debug("GLDefaultRenderer") << std::endl << d.transform << " " << d.component.vertex << " " << std::endl;

    }
    m_colorShader.disable();
  }

private:
  ColorShader m_colorShader;
};

class GLContext : public ::render::RenderContext<GLTexture,GLCamera,VBO,GLDrawable>{
public:
  void create() override{
    cprint_debug("GLContext") << "Create" << std::endl;
    ::render::RenderContext<GLTexture,GLCamera,VBO,GLDrawable>::create();
    m_renderer.create();
  }
  
  void render() override{
    m_renderer.draw(m_scene);  
  };
private:
  GLDefaultRenderer m_renderer;
  
};

}
}


#endif
