#include "../include/vbo.h"


std::ostream& render::gl::operator << (std::ostream& out, const render::gl::VBOAttrib& attrib){
  out << "Attribute(count,offset,stride): " << attrib.glHandle() << " " << attrib.count() << " " << attrib.offset() << " " << attrib.stride();
  return out;
}


GLuint render::gl::VBOAttrib::glHandle() const {
  return m_owner->handle();
}

void render::gl::VBO::update(uint8_t* data, uint32_t size){
  std::string error = "";
  glBindBuffer(GL_ARRAY_BUFFER, m_handle);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  _check_gl_error("Update Buffer",error);
  if(error.size() > 0){
       std::cout << " error updating VBO " << error << std::endl;
  }
   
}
