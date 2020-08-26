#include "../include/transform.h"
#include <glm/gtx/string_cast.hpp>

std::ostream& operator<<(std::ostream& out, const Transform& trans){
  for(int x = 0; x < 4; x++){
    out << "[ ";
    for(int y = 0; y < 4; y++){
      out << trans.m_transform[x][y] << " ";
    } 
    out << "]" << std::endl;
  }
  return out;
}

Transform& Transform::operator=(const Transform& other){
  m_transform = other.m_transform;
  return *this;
}

void Transform::setPosition(float x, float y, float z){
   m_transform[3][0] = x;
   m_transform[3][1] = y;
   m_transform[3][2] = z;
}

void Transform::setPositionX(float x){
  m_transform[3][0] = x;
}

void Transform::setPositionY(float y){
  m_transform[3][1] = y;
}

void Transform::setPositionZ(float z){
  m_transform[3][2] = z;
}

float Transform::positionX() const{
  return m_transform[3][0];
}

float Transform::positionY() const{
  return m_transform[3][1];
}

float Transform::positionZ() const{
  return m_transform[3][2];
}

void Transform::scale(float xyz){
    for(int x = 0; x < 3; x++){
        for(int y = 0; y < 3; y++){
            m_transform[x][y]*=xyz;
        }    
    }
}


std::ostream& Transform::operator<<(std::ostream& out){
  out << glm::to_string(m_transform);
  return out;
}

