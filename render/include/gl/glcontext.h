#ifndef _RENDER_GL_CONTEXT_
#define _RENDER_GL_CONTEXT_
#include "buffer.h"
#include "camera.h"
#include "texture.h"
#include "../rcontext.h"


namespace render{
namespace gl{

template<typename _T_Drawable>
class GLContext : public render::RenderContext<GLTexture,GLCamera,VBOBuffer,GLRenderer,_T_Drawable>{};
public:
  void init() override{
    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao); 
  }
}
}


#endif
