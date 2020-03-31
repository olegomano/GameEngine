#ifndef _RENDER_RCONTEXT_H_
#define _RENDER_RCONTEXT_H_ 
#include "scene.h"
#include "primitives.h"

namespace render{


template<typename _T_Drawable>
class IRenderContext{
  virtual void init() = 0;
  virtual void render() = 0;
  virtual IScene& scene() = 0;
  virtual IPrimitives<_T_Drawable>& primitives() = 0;
};

template<
  typename _T_Texture,
  typename _T_Camera,
  typename _T_Buffer,
  typename _T_Renderer,
  typename _T_Drawable
  >
class RenderContext : public IRenderContext<_T_Drawable>{
  typedef _T_Texture  Texture;
  typedef _T_Camera   Camera;
  typedef _T_Buffer   Buffer;
  typedef _T_Renderer Renderer;
  typedef RenderContext<_T_Texture,_T_Camera,_T_Buffer,_T_Renderer,_T_Drawable> Context;

public:
  virtual void init() override{
    m_primitives.init();
    m_renerer.init();
  }

  IPrimitives<_T_Drawable> primitives() override{
    return m_primitives;
  }

  IScene<_T_Drawable>& scene() override {
    return m_scene;
  }

  void render() override {
    m_renderer.draw(m_scene);
  }

private:
  render::Primitives<Context> m_primitives;
  render::Scene<Context> m_scene;
  _T_Renderer            m_renderer;
};

}
#endif
