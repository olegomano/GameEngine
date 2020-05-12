#ifndef _RENDER_PRIMITIVES_H_
#define _RENDER_PRIMITIVES_H_
#include "buffer.h"
#include "drawable.h"

#include <unordered_map>
#include <cstring>

namespace render{
namespace primitive{

enum Type : uint8_t{
  FIRST = 0,
  Triangle,
  Cube,
  Sphere,
  Rect,
  LAST
};


template<typename _T_Context>
class Primitives {
public:
  static constexpr float Tris_Verts[] = {
    -0.5 , -0.5 , 0,
     0,5 , -0,5 , 0,
     0   ,  0.5 , 0 
  }; 


  struct Prim{
    const Type  name;
    const float* buffer;
    const uint32_t byteCount;
  };

  static constexpr Prim PrimDeclaration[] = {
    {Triangle,Tris_Verts,sizeof(Tris_Verts)}
  };

  Primitives(){}
  
  bool newPrimitive(Type t, typename _T_Context::Drawable& out){
    out.vertex = m_vertexBuffers[t];
    return true; 
  }

  void create() {
    constexpr int primCount = sizeof(PrimDeclaration) / sizeof(Prim);
    
    uint32_t totalSize = 0;
      for(int i = 0; i < primCount; i++){
      const Prim& p = PrimDeclaration[i];
      totalSize+=p.byteCount;
    }

    m_buffer.create(totalSize);
    float* tmpBuffer = new float[totalSize];
    uint32_t tmpBufferCount = 0;

    for(int i = 0; i < primCount; i++){
      const Prim& p = PrimDeclaration[i];
      std::memcpy(tmpBuffer + tmpBufferCount,p.buffer,p.byteCount);
      m_vertexBuffers[p.name] = m_buffer.createAttrib(p.byteCount / sizeof(float),0,0); //count, offset, stride
    }
    m_buffer.write((uint8_t*)tmpBuffer,tmpBufferCount);
    
    delete[] tmpBuffer;
  };
private:
  std::unordered_map<Type,typename _T_Context::Buffer::Attribute> m_vertexBuffers;
  typename _T_Context::Buffer m_buffer;
};

}
}
#endif
