#ifndef _RENDER_GL_CONTEXT_
#define _RENDER_GL_CONTEXT_
#include "shader.h"
#include "buffer.h"
#include "camera.h"
#include "texture.h"
#include "../rcontext.h"


namespace render{
namespace gl{

struct GLDrawable{
  VBOAttrib vertex;
};

class GLNullRenderer{
public:
  void create(){}

  template<typename T>
  void draw(T& t){}
};


class GLDefaultRenderer{
public:
  void create(){
    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);
    m_colorShader.compile();
  }
 
  template<typename T>
  void draw(T& scene){}

private:
  ColorShader m_colorShader;
};

class GLContext : public ::render::RenderContext<GLTexture,GLCamera,VBO,GLDrawable>{
public:
  void create() override{
    ::render::RenderContext<GLTexture,GLCamera,VBO,GLDrawable>::create();
    m_renderer.create();
  }
  
  void render() override{
     
  };
private:
  GLDefaultRenderer m_renderer;
  
};

}
}


#endif
