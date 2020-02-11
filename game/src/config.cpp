#include "config.h"

std::ostream& operator << (std::ostream& out, const ContextConfig& config){
  out << "Window(w,h): " << config.m_screenWidth << "," << config.m_screenHeight << std::endl;
  out << "Scripts: [";
  for(int i = 0; i < config.m_scripts.size(); i++){
    out << config.m_scripts[i] << " ";
  }
  out << "]";
  return out;
}


