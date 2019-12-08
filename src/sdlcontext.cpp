#include "context.h"
#include <iostream>
#include <GL/glew.h>

template<typename... T>
void SDLContext<T...>::init(uint32_t w, uint32_t h){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 ); 
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
    m_window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN ); 
    m_glContext = SDL_GL_CreateContext( m_window );
    glewExperimental = true;
    glewInit();
    SDL_GL_SetSwapInterval(1);
}

template<typename... T>
uint32_t SDLContext<T...>::loop(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch (event.type)
        {
            case SDL_QUIT:
                return 1;
            case SDL_KEYDOWN:
                std::cout << "Key Down " << event.key.keysym.sym << std::endl;
                break;
            case SDL_KEYUP:
                std::cout << "Key Up " << event.key.keysym.sym << std::endl;
        }
    }
    return 0;
}

template<typename... T>
void SDLContext<T...>::destroy(){
    std::cout << "Destroy " << std::endl;
}