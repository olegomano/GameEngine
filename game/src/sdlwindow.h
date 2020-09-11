#ifndef _GAME_SDL_WINDOW_H_
#define _GAME_SDL_WINDOW_H_

#include <iostream>
#include <iostream>
#include <GL/glew.h>
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <functional>
#include "log.h"

#define LOG_TAG "SDLWindow"


inline std::ostream& operator<<(std::ostream& out, const SDL_KeyboardEvent& key){
  /* Is it a release or a press? */
  if( key.type == SDL_KEYUP ){
    out << "Release:- ";
  }
  else{
    out << "Press:- ";
  }
  /* Print the hardware scancode first */
  out <<  "Scancode: " <<  key.keysym.scancode;
  /* Print the name of the key */
  out << ", Name: " <<  SDL_GetKeyName( key.keysym.sym );
  /* We want to print the unicode info, but we need to make */
  /* sure its a press event first (remember, release events */
  /* don't have unicode info                                */
  return out;
}


class SDLWindow;
class SDLWindowManager{
public:
friend SDLWindow;
enum Backend{
  GL,
  VULCAN,
  DX
};
public:
  

  void create(Backend b);
  SDLWindow* addWindow(const std::string& name, uint32_t w, uint32_t h);
  std::vector<SDLWindow*>& windows(){return m_windows;}
  
  template<typename T>
  void handleInput(T&& handler){
     SDL_Event event;
     while(SDL_PollEvent(&event)){
        handler(event);
     }
  } 
private:
  std::vector<SDLWindow*> m_windows;
  Backend m_backend;
};


class SDLWindow{
public:
  SDLWindow(uint32_t w, uint32_t h, const std::string& name,uint32_t id) : m_width(w),m_height(h),m_name(name),m_id(id){}
  virtual void create(SDLWindowManager& manager) = 0;
  virtual void beginDraw() = 0;
  virtual void endDraw() = 0;
  void close();

  inline uint32_t width() const {return m_width;}
  inline uint32_t height() const {return m_height;}
  inline uint32_t id() const {return m_id;}
  inline const std::string& name()const {return m_name;}
protected:
  const uint32_t    m_width;
  const uint32_t    m_height;
  const std::string m_name;
  SDL_Window*       m_window;
  uint32_t          m_id;
};

class GLWindow : public SDLWindow{
public:
  using SDLWindow::SDLWindow;
  void create(SDLWindowManager& manager) override;
  void beginDraw() override;
  void endDraw() override;
private:
  SDL_GLContext m_glContext;
};




#endif
