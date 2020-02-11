#include "../include/transform.h"

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

void Transform::setPosition(float x, float y, float z){
   m_transform[3][0] = x;
   m_transform[3][1] = y;
   m_transform[3][2] = z;
}

void Transform::scale(float xyz){
    for(int x = 0; x < 3; x++){
        for(int y = 0; y < 3; y++){
            m_transform[x][y]*=xyz;
        }    
    }
}
