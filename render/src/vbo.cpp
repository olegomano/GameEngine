#include "../include/vbo.h"


std::ostream& render::gl::operator << (std::ostream& out, const render::gl::VBOAttrib& attrib){
  out << "Attribute(count,offset,stride): " << attrib.glHandle() << " " << attrib.count() << " " << attrib.offset() << " " << attrib.stride();
  return out;
}


GLuint render::gl::VBOAttrib::glHandle() const {
  return m_owner->handle();
}
