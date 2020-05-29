#ifndef _RENDER_PRIMITIVES_H_
#define _RENDER_PRIMITIVES_H_
#include "buffer.h"
#include "drawable.h"

#include <log.h>
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
  static constexpr std::array<float,12> Tris_Verts{
    -0.5f  , -0.5f , 0.0f, 1.0f,
     0.5f  , -0.5f , 0.0f, 1.0f,
     0.0f  ,  0.5f , 0.0f, 1.0f
  };

  static constexpr std::array<float,12> Rect_Verts{
    -0.5f  , -0.5f , 0.0f, 1.0f,
     0.5f  , -0.5f , 0.0f, 1.0f,
     0.0f  ,  0.5f , 0.0f, 1.0f
  };

  struct Prim{
    const Type  name;
    const float* buffer;
    const uint32_t byteCount;
  };

  static constexpr Prim PrimDeclaration[] = {
    {Triangle,&Tris_Verts[0],Tris_Verts.size()*sizeof(float)}
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
    cprint_debug("Primitive") << "Create Called, Primitives take " << totalSize << " bytes" << std::endl;
    
    m_buffer.create(totalSize);
    float* tmpBuffer = new float[totalSize];

    uint32_t tmpBufferCount = 0;
  
    uint32_t wroteBytes = 0;
    for(int i = 0; i < primCount; i++){
      const Prim& p = PrimDeclaration[i];
      std::memcpy(tmpBuffer + tmpBufferCount,p.buffer,p.byteCount);
      uint32_t elementCount = p.byteCount / sizeof(float);
      m_vertexBuffers[p.name] = m_buffer.createAttrib(elementCount,wroteBytes,0); //count, offset, stride
      wroteBytes+=p.byteCount;
    }
    //m_buffer.write((uint8_t*)&Tris_Verts[0],sizeof(Tris_Verts));
    m_buffer.write((uint8_t*)tmpBuffer,totalSize);
    delete[] tmpBuffer;
  };
private:
  std::unordered_map<Type,typename _T_Context::Buffer::Attribute> m_vertexBuffers;
  typename _T_Context::Buffer m_buffer;
};

}
}
#endif
