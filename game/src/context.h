#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <atomic>
#include <functional>

#include "renderer.h"
#include "primitives.h"
#include "core.h"
#include "sdlGL.h"
#include "types.h"



class Context{
public:
  Context();
  void init();
  void loopForever();
  void stopLooping();
  void addComponent(GameComponent& c);
  lua::Script* getScriptById(uint32_t id);
      
  void createWindow(const std::string& name, uint32_t w, uint32_t h);
  void loadScript(const std::string& name); //every script must have init function

  inline SdlGlContext&                   graphicsContext(){return m_glContext;}
  inline render::gl::Renderer<Drawable>& renderer()    {return m_renderer;}
  inline render::Primitives&             renderPrims() { return m_primitives;}
private:
  void onGLContextInit();

  SdlGlContext                   m_glContext;
  std::vector<lua::Script*>      m_scripts;
  std::vector<EntityScript>      m_entities;

    //core::block_array::BlockArray<GameComponent> m_gameComponents;

  std::vector<GameComponent> m_gameComponents;
  render::gl::Renderer<Drawable> m_renderer;
  render::Primitives             m_primitives;
  std::vector<GLWindow*>         m_windows;

  bool m_running = true;
};
#endif
