#ifndef _RENDER_TEXTURE_
#define _RENDER_TEXTURE_
#include <cinttypes>

namespace render{

class ITexture {
public:
  void create(uint32_t w, uint32_t h){
    m_width = w;
    m_height = h;
    createImpl();
  }

  virtual void write(uint8_t* pixels) = 0;
  virtual void destroy() = 0;
  
  uint32_t width() const{return m_width;}
  uint32_t height() const{return m_height;}
protected:
  virtual void createImpl() = 0;
  uint32_t m_width;
  uint32_t m_height;
};
}
#endif
