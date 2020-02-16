#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "core.h"

#include <vector>
#include <string>
#include <ostream>

class ContextConfig{
public:
  friend std::ostream& operator<<(std::ostream& out, const ContextConfig& config);
  void parse(lua::LuaRef root){
        root["width"].read(m_screenWidth);
        root["height"].read(m_screenHeight);
        root["scripts"].read(m_scripts);
  }

  inline const uint32_t width() const {return m_screenWidth;}
  inline const uint32_t height() const {return m_screenHeight;}
  inline const std::vector<std::string> scripts() const {return m_scripts;}
private:
  uint32_t                 m_screenWidth;
  uint32_t                 m_screenHeight;
  std::vector<std::string> m_scripts;
};

#endif
