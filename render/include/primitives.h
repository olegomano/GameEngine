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
  static constexpr std::array<float,18> Tris_Verts{
    -0.5f  , -0.5f , 0.0f, 1.0f,
     0.5f  , -0.5f , 0.0f, 1.0f,
     0.0f  ,  0.5f , 0.0f, 1.0f,
    //UV SECTION
     0,0,
     1,0,
     0,1
  };

  static constexpr std::array<float,36> Rect_Verts{
    -0.5f  , -0.5f , 0.0f, 1.0f,
     0.5f  , -0.5f , 0.0f, 1.0f,
     0.5f  ,  0.5f , 0.0f, 1.0f,

      0.5f,  0.5f, 0, 1.0f,
     -0.5f,  0.5f, 0, 1.0f,
     -0.5f, -0.5f, 0, 1.0f,
    //UV SECTION
     0,0,
     1,0,
     1,1,

     1,1,
     0,1,
     0,0
  };

  static constexpr std::array<float,36*6> Cube_Verts{
     -0.5f  , -0.5f , 0.5f, 1.0f,
     0.5f  , -0.5f , 0.5f, 1.0f,
     0.5f  ,  0.5f , 0.5f, 1.0f,

      0.5f,  0.5f, 0.5f, 1.0f,
     -0.5f,  0.5f, 0.5f, 1.0f,
     -0.5f, -0.5f, 0.5f, 1.0f,

    -0.5f  , -0.5f , -0.5f, 1.0f,
     0.5f  , -0.5f , -0.5f, 1.0f,
     0.5f  ,  0.5f , -0.5f, 1.0f,

      0.5f,  0.5f, -0.5f, 1.0f,
     -0.5f,  0.5f, -0.5f, 1.0f,
     -0.5f, -0.5f, -0.5f, 1.0f,


     -0.5f, -0.5f,  0.5f, 1,
     -0.5f, -0.5f, -0.5f, 1, 
     -0.5f, 0.5f,  -0.5f, 1,

     -0.5f,  0.5f,  -0.5f, 1,
     -0.5f,  -0.5f,   0.5f, 1,
     -0.5f,  0.5f,   0.5f, 1,

     0.5f, 0.5f,   0.5f, 1,
     0.5f, -0.5f,  0.5f, 1,
     0.5f, -0.5f, -0.5f, 1,
     
     0.5f,  0.5f,  -0.5f, 1,
     0.5f,  0.5f,   0.5f, 1,
     0.5f, -0.5f,  -0.5f, 1,


    -0.5f,0.5f,0.5f,1,
    0.5f,0.5f,0.5f,1,
    0.5f,0.5f,-0.5f,1,

    -0.5f,0.5f,-0.5f,1,
    -0.5f,0.5f, 0.5f,1,
     0.5f,0.5f, -0.5f,1,

    -0.5f,-0.5f,0.5f,1,
    0.5f,-0.5f,0.5f,1,
    0.5f,-0.5f,-0.5f,1,

    -0.5f,-0.5f,-0.5f,1,
    -0.5f,-0.5f, 0.5f,1,
     0.5f,-0.5f, -0.5f,1
  };

  struct Prim{
    const Type  name;
    const float* buffer;
    const uint32_t byteCount;
  };

  static constexpr Prim PrimDeclaration[3] = {
    {Triangle,&Tris_Verts[0],sizeof(Tris_Verts)},
    {Rect,&Rect_Verts[0],sizeof(Rect_Verts)},
    {Cube,&Cube_Verts[0],sizeof(Cube_Verts)}
  };

  Primitives(){}
  
  bool newPrimitive(Type t, typename _T_Context::Drawable& out){
    out.vertex = m_vertexBuffers[t];
    out.uv = m_uvBuffers[t];
    return true; 
  }

  void create() {

    constexpr int primCount = sizeof(PrimDeclaration) / sizeof(Prim);
    
    uint32_t totalSize = 0;
    for(int i = 0; i < primCount; i++){
      const Prim& p = PrimDeclaration[i];
      totalSize+=p.byteCount;
    }
    cprint_debug("Primitive") << "Create Called, Primitives take " << totalSize << " bytes " << primCount << std::endl;
    
    m_buffer.create(totalSize);
    float* tmpBuffer = new float[totalSize];

  
    uint32_t wroteBytes = 0;
    for(int i = 0; i < primCount; i++){
      const Prim& p = PrimDeclaration[i];
      std::memcpy((uint8_t*)tmpBuffer + wroteBytes,p.buffer,p.byteCount);
      

      uint32_t vertexCount  = (p.byteCount / sizeof(float)) / 6;
      uint32_t vertBytes = vertexCount*4;
      uint32_t uvbBytes =vertexCount*2;

      m_vertexBuffers[p.name] = m_buffer.createAttrib(vertexCount,wroteBytes,0); //count, offset, stride 
      m_uvBuffers[p.name] = m_buffer.createAttrib(vertexCount,wroteBytes + vertBytes,0); //count, offset, stride

      wroteBytes+=p.byteCount;
    }
    m_buffer.write((uint8_t*)tmpBuffer,totalSize);
    delete[] tmpBuffer;
  };
private:
  std::unordered_map<Type,typename _T_Context::Buffer::Attribute> m_vertexBuffers;
  std::unordered_map<Type,typename _T_Context::Buffer::Attribute> m_uvBuffers;
  typename _T_Context::Buffer m_buffer;
};

}
}
#endif
