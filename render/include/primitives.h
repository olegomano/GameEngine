#ifndef _RENDER_PRIMITIVES_H_
#define _RENDER_PRIMITIVES_H_
#include "renderer.h"
#include "vbo.h"

namespace render{

class Primitives{
public:
    enum Type{
        Triangle,
        Cube,
        Sphere,
        Rect
    };
    void init();
    gl::Drawable get(Type t);
    gl::Drawable operator[](Type t);
private:
    render::gl::VBO m_allPrimitivesBuffer;
    gl::Drawable    m_triangle;
    gl::Drawable    m_rect;
    gl::Drawable    m_cube;
    gl::Drawable    m_sphere;
};

}

#endif
