#ifndef _SDL_CONTEXT_
#define _SDL_CONTEXT_

#include <SDL2/SDL.h>
#include <cstdint>
#include <vector>
#include <tuple>

template<typename... T>
class Context{

public:
    virtual void     init(uint32_t w, uint32_t h) = 0;
    virtual void     loopForever(){
        while(!loop()){}
        destroy();
    }
    virtual void     destroy() = 0;
protected:
    virtual uint32_t loop() = 0;
protected:
    std::tuple<T...> m_processes;
};


template<typename... T>
class SDLContext : public Context<T...>{
public:
    void     init(uint32_t w, uint32_t h) override;
    void     destroy() override;
protected:
    uint32_t loop() override;
private:
    SDL_Window*   m_window;
    SDL_GLContext m_glContext;
};

#endif