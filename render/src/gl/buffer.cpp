#include "../../include/gl/buffer.h"


void render::gl::VBO::create(uint32_t size) {
  std::string error = "";          
  if(m_handle != -1) destroy();
  glGenBuffers(1,&m_handle);
  _check_gl_error("Gen Buffers",error);
  if(error.size() > 0){
    std::cout << " error creating VBO " << error << std::endl;
    return;
  }
  write(nullptr,size);
}
  
void render::gl::VBO::write(uint8_t* buffer, uint32_t count){
  std::string error = "";          
  glBindBuffer(GL_ARRAY_BUFFER,m_handle);
  _check_gl_error("glBindBuffer",error);
  glBufferData(GL_ARRAY_BUFFER,count,buffer,GL_STATIC_DRAW);
  _check_gl_error("glBufferData",error);
  if(error.size() > 0){
    std::cout << " error writing VBO " << error << std::endl;
    return;
  }
  
}
  
void render::gl::VBO::write(uint8_t* buffer, uint32_t offset,uint32_t count){
  std::cout << "Warning calling unimplemented function write(buffer,offset,count) " << std::endl;
}

void* render::gl::VBO::map(){
  glBindBuffer(GL_ARRAY_BUFFER, m_handle);
  return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void render::gl::VBO::unmap(){
  glUnmapBuffer(GL_ARRAY_BUFFER);
}

void  render::gl::VBO::destroy(){
  std::cout << "Destroying VBO" << std::endl;
}

render::gl::VBOAttrib render::gl::VBO::createAttrib(uint32_t count,uint32_t offset, uint32_t stride){
  return render::gl::VBOAttrib(this,offset,stride,count);

}
std::ostream& render::gl::operator<<(std::ostream& o, const render::gl::VBOAttrib& vbo){
  o << "VBOAttrib: " << vbo.m_owner->m_handle << " count " << vbo.m_count << " offset: " << vbo.m_offset << " stride " << vbo.m_stride;
  return o;
}

