#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/perpendicular.hpp>
#include <iostream>


class Transform{
friend std::ostream& operator<<(std::ostream& out, const Transform& trans);
public:
  inline void rotate(const glm::vec3& axis, float angle){
    /*
    glm::vec3 forward = glm::perp(axis,glm::vec3(0,0,1));
    glm::vec3 right   = glm::cross(axis,forward);
    
    glm::mat4 rotation;
    rotation[0] = glm::vec4(right,0);
    rotation[1] = glm::vec4(forward,0);
    rotation[2] = glm::vec4(axis,0);
    rotation[3] = glm::vec4(0,0,0,1);
    
    rotation = glm::transpose(rotation);

    rotation = glm::rotate(rotation,angle,glm::vec3(0,0,1));
    m_transform = rotation*m_transform;
    */
    glm::vec4 position = m_transform[3]; 
    m_transform[3] = glm::vec4(0,0,0,1);
    m_transform = glm::rotate(m_transform,angle,axis);
    m_transform[3] = position;
    //setPosition(position[0],position[1],position[2]);
  }
  
  inline void rotate(float x, float y, float z, float angle){
    rotate(glm::vec3(x,y,z),angle);
  }
    
  inline void set(const glm::mat4& m);
  inline void setPosition(const glm::vec4& v);
  inline void setPosition(const glm::vec3& v);
  void setPosition(float x, float y, float z);
  void setPositionX(float x);
  void setPositionY(float y);
  void setPositionZ(float z);
    
  float positionX() const;
  float positionY() const;
  float positionZ() const;
  inline glm::vec4 position() const {
    return m_transform[3];
  }

  void scale(float xyz);
  inline void scale(float x, float y, float z);

  inline void lookAt(const glm::vec3& v);
  inline void lookAt(const glm::vec4& v);
  inline void lookAt(float x, float y, float z);
    
  inline void apply(const glm::mat4& in, glm::mat4& out);
  inline void undo(glm::mat4& out);

  inline glm::mat4 transform() const {return m_transform;}
  inline glm::mat4 invTransform() const {return glm::inverse(m_transform);}
  Transform& operator = (const glm::mat4 mat);
  Transform& operator = (const Transform& other);
  std::ostream& operator<<(std::ostream& out);

private:
    glm::mat4 m_transform = glm::mat4(1);
};

#endif
