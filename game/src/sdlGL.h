#include <iostream>
#include <GL/glew.h>
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <functional>
#include "log.h"

#define LOG_TAG "GLContext"

class GLWindow{
public:
  GLWindow(const std::string name, uint32_t w, uint32_t h)
    :m_name(name)
    ,m_width(w)
    ,m_height(h){}
  
  void init(){
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT,0);
    m_window    = SDL_CreateWindow( m_name.c_str(), 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        m_width, m_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN ); 
    m_glContext = SDL_GL_CreateContext( m_window );
    m_windowId  = SDL_GetWindowID(m_window); 
  }

  void init(GLWindow* w){
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT,1);
    m_window    = SDL_CreateWindow( m_name.c_str(), 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        m_width, m_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN ); 
    m_glContext = w->context();
    m_windowId  = SDL_GetWindowID(m_window);
  }


  void startDraw(){
    SDL_GL_MakeCurrent(m_window,m_glContext);
  }

  void clear(){
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void show(){
    SDL_GL_SwapWindow(m_window);
  }

  void close(){
  
  }

  SDL_GLContext context() const {
    return m_glContext;
  }

  SDL_Window* window() const {
    return m_window;
  }
private:
  const std::string m_name;
  const uint32_t    m_width;
  const uint32_t    m_height;
  uint32_t          m_windowId;
  SDL_Window*       m_window;
  SDL_GLContext     m_glContext;
};

class SdlGlContext{
public:
  typedef std::function<void()> GLContextInitCallback;

  SdlGlContext(){
  }

  void setContextCallback(GLContextInitCallback c){
    m_contextInitCallback = c;
  }

  GLWindow* createWindow(const std::string& name, uint32_t w, uint32_t h){
    GLWindow* window = new GLWindow(name,w,h); 
    if(m_windows.size() == 0){
      cprint_debug(LOG_TAG) << "Creating window with new context " << name << " (" << w << "," << h <<") " << std::endl;
      window->init();
      glewInit();
      m_contextInitCallback();
    }else{
      cprint_debug(LOG_TAG) << "Creating window with derived context " << name << " (" << w << "," << h <<") " << std::endl;
      window->init(m_windows[0]); 
    }
    m_windows.push_back(window);
    return window;
  }

  template<typename T>
  void checkInput(const T& handler){
        SDL_PumpEvents();
        int x;
        int y;
        SDL_GetMouseState(&x,&y);
        m_mousex = x;
        m_mousey = y;

        SDL_Event event;
        while(SDL_PollEvent(&event)){
            
        }
        handler(m_keys,m_mousex,m_mousey);
    } 

  void init(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 ); 
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
    glewExperimental = true;
    SDL_GL_SetSwapInterval(1);
    m_keys = SDL_GetKeyboardState(NULL);
  }
private: 
    double                 m_mousex = 0.5;
    double                 m_mousey = 0.5;
    const uint8_t*         m_keys;
    std::vector<GLWindow*> m_windows;
    GLContextInitCallback  m_contextInitCallback;
};

#undef LOG_TAG
