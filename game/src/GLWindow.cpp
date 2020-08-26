#include "sdlwindow.h"


void GLWindow::create(SDLWindowManager& manager){
   if(manager.windows().size() == 0){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 ); 
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
    glewExperimental = true;
    SDL_GL_SetSwapInterval(1);
        
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT,0);
    m_window    = SDL_CreateWindow( m_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN ); 
    m_glContext = SDL_GL_CreateContext( m_window );
    m_id        = SDL_GetWindowID(m_window); 
    glewInit();
  }else{
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT,1);
    m_window    = SDL_CreateWindow( m_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN ); 
    m_glContext = ((GLWindow*)manager.windows()[0])->m_glContext;
    m_id        = SDL_GetWindowID(m_window); 
  }
}

void GLWindow::beginDraw(){
  glViewport(0,0,m_width,m_height);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(1,1,1,1);
}

void GLWindow::endDraw(){
  SDL_GL_SwapWindow(m_window);
}
