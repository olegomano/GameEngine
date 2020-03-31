#ifndef _RENDER_PRIMITIVES_H_
#define _RENDER_PRIMITIVES_H_
#include "renderer.h"
#include "buffer.h"

namespace render{
namespace primitive{

enum Type{
  Triangle,
  Cube,
  Sphere,
  Rect
};

template<typename _T_Drawable>
class IPrimitives{
public:
  virtual void init() = 0;
  _T_Drawable get(Type t){
    switch(t){
      case Triangle:
        return m_triangle;
      case Cube:
        return m_cube;
      case Sphere:
        return m_sphere;
      case Rect:
        return m_rect;
    }
  }

  _T_Drawable operator[](Type t){
    return get(t);
  }
protected:
  _T_Drawable m_triangle;
  _T_Drawable m_cube;
  _T_Drawable m_sphere;
  _T_Drawable m_rect;
};
  
template<typename _T_Context>
class Primitives : typename IPrimitives<_T_Context::_T_Drawable>{
public:
  void init() override {};
private:
  typedef _T_Context::Buffer m_vertexBuffer;
};

}
#endif
