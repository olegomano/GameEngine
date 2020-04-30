#ifndef _RENDER_PRIMITIVES_H_
#define _RENDER_PRIMITIVES_H_
#include "buffer.h"
#include "drawable.h"

#include <cstring>

namespace render{
namespace primitive{

enum Type{
  Triangle,
  Cube,
  Sphere,
  Rect
};


template<typename _T_Context>
class Primitives {
public:
  typename _T_Context::Drawable& get(Type t){
    return m_triangle;
  }
  
  
  static constexpr float Tris_Verts[] = {
    -0.5 , -0.5 , 0,
     0,5 , -0,5 , 0,
     0   ,  0.5 , 0 
  }; 


  struct Prim{
    const char*  name;
    const float* buffer;
    const uint32_t byteCount;
    typename _T_Context::Drawable& drawable;
  };

  Primitives(){}

  void create() {

    const Prim PrimDeclaration[] = {
      {"Tris",Tris_Verts,sizeof(Tris_Verts),this->m_triangle}
    };
    constexpr int primCount = sizeof(PrimDeclaration) / sizeof(Prim);
    
    uint32_t totalSize = 0;
      for(int i = 0; i < primCount; i++){
      const Prim& p = PrimDeclaration[i];
      totalSize+=p.byteCount;
    }

    m_vertexBuffer.create(totalSize);
    float* tmpBuffer = new float[totalSize];
    uint32_t tmpBufferCount = 0;

    for(int i = 0; i < primCount; i++){
      const Prim& p = PrimDeclaration[i];
      std::memcpy(tmpBuffer + tmpBufferCount,p.buffer,p.byteCount);
      p.drawable.vertex = m_vertexBuffer.createAttrib(p.byteCount / sizeof(float),0,0); //count, offset, stride
    }
    m_vertexBuffer.write((uint8_t*)tmpBuffer,tmpBufferCount);
    
    delete[] tmpBuffer;
  };
private:
  typename _T_Context::Buffer m_vertexBuffer;
  typename _T_Context::Drawable m_triangle;
};

}
}
#endif
