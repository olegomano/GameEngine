#include "../include/primitives.h"
#include <memory>
#include <cstring>


render::gl::Drawable render::Primitives::operator[](render::Primitives::Type t){
    return get(t);
}


render::gl::Drawable render::Primitives::get(render::Primitives::Type t){
    using render::Primitives;

    switch (t){
    case Type::Cube:
        return m_cube;
    case Type::Sphere:
        return m_sphere;
    case Type::Triangle:
        return m_triangle;
    case Type::Rect:
        return m_rect;
    };
}

static constexpr float Tris_Buffer[] = {
        //vertex         //uv       //normals
    -1.0, -1.0, 0, 1,     0,0,       0,0,1,0,
     1.0, -1.0, 0, 1,     0,1,       0,0,1,0,
     0,0,  1,0, 0, 1,     1,0,       0,0,1,0
};

static constexpr float Rect_Buffer[] = {
        //vertex         //uv       //normals
    -1.0, -1.0, 0, 1,     0,0,       0,0,1,0,
    -1.0,  1.0, 0, 1,     0,1,       0,0,1,0,
     1,0,  1,0, 0, 1,     1,0,       0,0,1,0,

     1,0,  1,0, 0, 1,     1,1,       0,0,1,0,
     1,0, -1,0, 0, 1,     1,0,       0,0,1,0,
    -1,0, -1,0, 0, 1,     0,0,       0,0,1,0
};

static constexpr float Cube_Buffer[] = {

};

struct Primitive{
    const  float* const   data;
    const  uint32_t       size;
    render::gl::Drawable& drawable;
    const  char*          name;
};
/**
static constexpr Primitive Primitive_List[] = {
    {Tris_Buffer,120,render::Primitives::Type::Triangle,"Triangle"},
    {Rect_Buffer,240,render::Primitives::Type::Rect,"Rectangle"}
};
**/

#define length(x) (sizeof(x) / sizeof((x)[0]))
void render::Primitives::init(){    
    Primitive Primitive_List[] = {
      {Tris_Buffer,120,m_triangle,"Triangle"},
      {Rect_Buffer,240,m_rect,"Rectangle"}
    };
    
    m_allPrimitivesBuffer.create(nullptr,(sizeof(Tris_Buffer)+sizeof(Rect_Buffer)));
    m_allPrimitivesBuffer.updateMap([=](void* buffer){
        int counter = 0;
        for(int i = 0; i < length(Primitive_List); i++){  
            const Primitive& prim = Primitive_List[i];
            std::cout << "Initializing Primitive " << prim.name << " size " << prim.size << " offset " << counter << std::endl;
            
            memcpy((float*)buffer + counter,prim.data,prim.size);
            render::gl::Drawable& d = prim.drawable;
            d.vertex                = m_allPrimitivesBuffer.createAttrib(prim.size/(10*sizeof(float)),counter                  ,40);
            d.normals               = m_allPrimitivesBuffer.createAttrib(prim.size/(10*sizeof(float)),counter + 6*sizeof(float),40);
            counter                 +=prim.size;
        }
    });
    m_allPrimitivesBuffer.create<std::initializer_list<float>>({-1,-1,0,1, 0,1,0,1, 1,-1,0,1});
    m_triangle.vertex = m_allPrimitivesBuffer.createAttrib(3,0,0);
};
#undef length
