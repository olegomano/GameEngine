#ifndef _TYPES_H_
#define _TYPES_H_
#include "core.h"
#include "renderer.h"
#include <vector>

struct Drawable{
    Transform             position;
    render::gl::VBOAttrib normals;
    render::gl::VBOAttrib vertex;
};


class Component{
  void* owner;
  void* object;
};

 
class IFile{
public:
  virtual void     load()      = 0;
  virtual bool     hasUpdate() = 0;
  virtual void     free()      = 0;
  virtual uint8_t* data()      = 0;
  virtual std::string const path() = 0;
};

class LuaEntity{
public:
  void parse(lua::LuaRef ref);
  void onInit();
  void onFrame();
  void onMouse(double x, double y);
  void onKey();
  void onEvent();
  void onDestroy();
private:
  lua::LuaRef   m_initFunction;
  lua::LuaRef   m_frameFunction;
  lua::LuaRef   m_keyboardFunction;
  lua::LuaRef   m_mouseFunction;
  lua::LuaRef   m_eventFunction;
  lua::LuaRef   m_destroyFunction;
  //should have reference to all components this script initializes
  //
};


#endif
