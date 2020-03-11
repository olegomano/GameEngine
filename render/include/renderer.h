#ifndef _GL_RENDERER_H_
#define _GL_RENDERER_H_
#include <inttypes.h>
#include "core.h"
#include "utils.h"
#include "shader.h"
#include "transform.h"
#include "vbo.h"

#include <glm/glm.hpp>
#include <vector>
#include <tuple>
#include <experimental/type_traits>
#include <log.h>

namespace render{

namespace gl{

struct Drawable{
    Transform position;
    VBOAttrib vertex;
    VBOAttrib normals;   

    template<typename T>
    using normal_t = decltype(std::declval<T>().normals);

    template<typename T>
    using has_normals = std::experimental::is_detected<normal_t,T>;
        
    template<typename T>
    using vertex_t = decltype(std::declval<T>().vertex);

    template<typename T>
    using has_vertex = std::experimental::is_detected<vertex_t,T>;    
};


template<typename _T_drawable>
class Renderer{    
public:
    void init();
    void render(const glm::mat4& camera);

    template<typename T>
    _T_drawable& createDrawable(const T& d){
        _T_drawable draw;
        
        draw.position = d.position;
        draw.vertex   = d.vertex;
        draw.normals  = d.normals;
        int indx      = m_drawables.size();
        m_drawables.push_back(draw);
        
        //cprint_debug() << "Created Drawable " << m_drawables.size() << std::endl;
        
        return m_drawables[indx];
    }

    template<typename T>
    uint32_t createCamera(const T& c){
        return 0;
    }

    template<typename T>
    void updateDrawable(uint32_t index, const T& t){
        Drawable& d = m_drawables[index];
    }

    void deleteDrawable(uint32_t index){
        
    }

    template<typename T>
    uint32_t createLight(const T& l){
        return -1;
    }
private:
    ColorShader                                 m_shader;
    core::block_array::BlockArray<_T_drawable>  m_drawables; 
};


}
}
#include "../src/glRenderer.cpp"
#endif 
