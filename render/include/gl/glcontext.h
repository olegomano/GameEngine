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

class GLNullContext : public render::RenderContext<GLTexture,GLCamera,VBO,GLNullRenderer,GLDrawable>{
public:
  void create() override {
    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao); 
  }
};


class GLContext : 
 public render::RenderContext<GLTexture,GLCamera,VBO,GLDefaultRenderer,GLDrawable>
,public render::scene::SceneHook<GLCamera>
,public render::scene::SceneHook<GLDrawable>
{
public:
  void create() override{
    m_primitives.create();
    m_renderer.create();
 }

  void onCreated(GLCamera& camera) override {
  
  }

  void onCreated(GLDrawable& drawable) override {
  
  }

  void onDeleted(GLCamera& camera) override {
  
  }

  void onDeleted(GLDrawable& camera) override {
  
  }
};

}
}


#endif
