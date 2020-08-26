#include "sdlwindow.h"
#include <log.h>

void SDLWindowManager::create(Backend b){
  m_backend = b; 
}

SDLWindow* SDLWindowManager::addWindow(const std::string& name, uint32_t w, uint32_t h){
  switch(m_backend){
    case VULCAN:
      return nullptr;
    case DX:
      return nullptr;
    case GL:
      cprint_debug("SDLWidowmanager") << "Creating Window " <<  name << "(" << w << "," << h << ")" << std::endl;      
      SDLWindow* window = new GLWindow(w,h,name,m_windows.size());
      window->create(*this);
      m_windows.push_back(window);
      return window;
  }
  return nullptr;
}
 
