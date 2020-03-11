#ifndef _TYPES_H_
#define _TYPES_H_
#include "core.h"
#include "renderer.h"
#include <vector>
#include <variant>


//core should define some kind of standard pointer type so i can have an array of it here
//IPtr -> Ptr<T>
class Drawable {
public:
  static constexpr char* LuaX = "x";
  static constexpr char* LuaY = "y";
  static constexpr char* LuaZ = "z";
  Transform             position;
  render::gl::VBOAttrib normals;
  render::gl::VBOAttrib vertex;
private:
  lua::LuaRef m_luaState;
};

class EntityScript{
public:
  void load(lua::LuaRef ref){
    m_onInit = ref["init"];
    m_onFrame = ref["frame"];
  }

  void onInit(){
    m_onInit.call();
  }
  void onFrame(){
    m_onFrame.call();
  }
private:
  lua::LuaRef m_onInit;
  lua::LuaRef m_onFrame; 
};

class ILuaComponent{
public:
  virtual void readLua() = 0;
  virtual void create()  = 0;
  virtual lua::LuaRef luaRef() const = 0;
};

template<typename ...T>
class LuaComponent : public ILuaComponent{
public:
  template<typename A>
  LuaComponent(A& data, lua::Script* s) 
  :m_data(&data)
  ,m_script(s)
  {
  
  }

  void readLua(){
    std::visit([this](auto& component){
        assert(component != nullptr);
        m_ref.read(*component);
        lua_pop(m_script->luaState(),1);    
    },m_data);
  };

  void create(){
    std::visit([this](const auto& component){
        m_ref= create_table(m_script,*component);
    },m_data);   
  };

  lua::LuaRef luaRef() const {
    return m_ref;
  }

  lua::Script* owner() const {
    return m_script;
  }
private:
  lua::LuaRef        m_ref;
  lua::Script*       m_script = nullptr;
  std::variant<T...> m_data;
};

typedef LuaComponent<Drawable*> GameComponent;

#endif
