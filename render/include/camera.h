#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "transform.h"

namespace render{

class ICamera{
public:
  virtual void create() = 0;
  
  inline Transform& transform() {return m_transform;}
  inline float fov() const {return m_fov;}
  inline void setFov(float f){m_fov=f;}

  inline const glm::mat4& projectionMatrix() {return m_projetion;}
  inline const glm::mat4& modelMatrix() {return m_transform.transform();}
private:
  glm::mat4 m_projetion;
  Transform m_transform;
  float     m_fov = 90;
};


};

#endif
