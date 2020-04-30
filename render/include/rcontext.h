#ifndef _RENDER_RCONTEXT_H_
#define _RENDER_RCONTEXT_H_ 
#include "scene.h"
#include "primitives.h"

#include <initializer_list>

namespace render{


class IRenderContext{
  virtual void create() = 0;
  virtual void render() = 0;
  virtual scene::IAbstractScene& scene() = 0;
  virtual scene::Entity createCamera() = 0;
  //virtual void createPrimitive() = 0;
//virtual void createColladaAsset() = 0;
};

template<
  typename _T_Texture,
  typename _T_Camera,
  typename _T_Buffer,
  typename _T_Renderer,
  typename _T_Drawable
  >
class RenderContext : public IRenderContext{
public:
  typedef _T_Texture Texture;
  typedef _T_Camera Camera;
  typedef _T_Buffer Buffer;
  typedef _T_Renderer Renderer;
  typedef _T_Drawable Drawable;
  typedef RenderContext<_T_Texture,_T_Camera,_T_Buffer,_T_Renderer,_T_Drawable> Context;

public:
  RenderContext(){}

  virtual void create() = 0;

  scene::IAbstractScene& scene() override {
    return m_scene;
  }

  void render() override {
    m_renderer.draw(m_scene);
  }

  scene::Entity createCamera() override {
    std::initializer_list<scene::Component> components = {scene::Camera,scene::Transform};
    scene::Entity e = m_scene.createEntity(components);
    return e;
  }

protected:
  primitive::Primitives<Context> m_primitives;
  scene::Scene<Context> m_scene;
  _T_Renderer    m_renderer;
};

}
#endif
