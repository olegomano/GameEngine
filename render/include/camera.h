#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "transform.h"

namespace render{

class ICamera{
public:
  virtual void create(uint32_t w, uint32_t h){
    m_width = w;
    m_height = h;
    buildProjection();
  };
  /**
   * Writes the output of the camera to the sceen
   * goes to the rect, l,t,r,b in pixel coordintes in the screen
   * (0,0) is top left
   */
  virtual void blitToScreen(uint32_t l, uint32_t t, uint32_t r, uint32_t b) = 0;

  inline float fov() const {return m_focus;}
  
  inline void setFov(float f, float aspectRatio = 1){
    m_focus=f; //TODO: focus = 1/tan(f)
    m_aspectRatio = aspectRatio;
    buildProjection();  
  } 
  
  inline uint32_t height() const {return m_height;}
  inline uint32_t width() const {return m_width;}


  inline const glm::mat4& projection() const {
    return m_projetion;
  }

private:
  inline void buildProjection(){
    m_projetion = glm::mat4(m_focus);
    m_projetion[1][1]*=m_aspectRatio;
  }

  glm::mat4 m_projetion;
  float     m_near        = 0.01;
  float     m_far         = 100;
  float     m_focus       = 1;
  float     m_aspectRatio = 1;
  uint32_t  m_width       = 0;
  uint32_t  m_height      = 0;
};


};

#endif
