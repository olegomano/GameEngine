#ifndef _RENDER_DRAWABLE_H_
#define _RENDER_DRAWABLE_H_
#include "texture.h"
#include "buffer.h"


namespace render{


class IDrawable{
  inline IAttribute* vertex(){
    return m_vertexAttribute;
  }
  
  virtual void create() = 0;
  
protected:
  IAttribute* m_vertexAttribute = nullptr;

  void* m_allComponents[1] =  {&m_vertexAttribute};

};


};
#endif
