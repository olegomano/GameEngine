#include <iostream>
#include <GL/glew.h>
#include <SDL.h>
#include <stdio.h>
#include <string>

class SdlGlContext{
public:
    SdlGlContext(){}

    void clearFrame(){
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void showFrame(){
        SDL_GL_SwapWindow(m_window);
    }

    template<typename T>
    void checkInput(const T& handler){
        SDL_PumpEvents();
        int x;
        int y;
        SDL_GetMouseState(&x,&y);
        m_mousex = (double) x / (double) m_width;
        m_mousey = (double) y / (double) m_height;

        SDL_Event event;
        while(SDL_PollEvent(&event)){
            
        }
        handler(m_keys,m_mousex,m_mousey);
    }
    

    void init(uint32_t w, uint32_t h, const std::string& name = "Default Window"){
        m_width  = w;
        m_height = h;

        SDL_Init(SDL_INIT_VIDEO);
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 ); 
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
        m_window         = SDL_CreateWindow( name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN ); 
        m_glContext      = SDL_GL_CreateContext( m_window );
        glewExperimental = true;
        glewInit();
        SDL_GL_SetSwapInterval(1);
        m_keys = SDL_GetKeyboardState(NULL);
    }
    /**
    void printKeyState(){
      for(int i = 0; i < )
    }
    **/

private:
    SDL_Window*     m_window;
    SDL_GLContext   m_glContext;
    
    uint32_t        m_width;
    uint32_t        m_height;
    double          m_mousex = 0.5;
    double          m_mousey = 0.5;
    std::string     m_windowName;
    
    const uint8_t*  m_keys;
};
