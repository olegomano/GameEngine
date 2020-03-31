#ifndef _GL_RENDERER_H_
#define _GL_RENDERER_H_
#include <inttypes.h>
#include "core.h"
#include "utils.h"
#include "transform.h"
#include "camera.h"
#include "gl/shader.h"
#include "gl/framebuffer.h"
#include "gl/buffer.h"

#include <glm/glm.hpp>
#include <vector>
#include <tuple>
#include <experimental/type_traits>
#include <log.h>


namespace render{

namespace gl{

class Renderer{    
public:
  void init();
  
  template<typename _T_Scene>
  void renderScene(_T_Scene& scene){
    
  }
  
private:
  ColorShader m_shader;
};


}
}
#include "../src/glRenderer.cpp"
#endif 
