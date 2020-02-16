#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <atomic>

#include "renderer.h"
#include "primitives.h"
#include "core.h"
#include "sdlGL.h"
#include "types.h"



class Context{
public:
    Context();
    void addScripts(const std::vector<std::string>& scripts);
    void addScript(const std::string& script);
    void init();
    void setWindowParams(uint32_t w, uint32_t h, const std::string& name);
    void loopForever();
    void stopLooping();

    inline render::gl::Renderer<Drawable>& renderer()    {return m_renderer;}
    inline render::Primitives&             renderPrims() { return m_primitives;}
private:
    SdlGlContext                   m_glContext;
    std::vector<std::string>       m_luaScriptPaths;
    std::vector<lua::Script*>      m_scripts;

    std::vector<lua::LuaRef>       m_luaInitFunctions;
    std::vector<lua::LuaRef>       m_luaFrameFunctions;
    std::vector<lua::LuaRef>       m_luaKeyFunctions;
    std::vector<lua::LuaRef>       m_luaMouseFunctions;
    
    render::gl::Renderer<Drawable> m_renderer;
    render::Primitives             m_primitives;

    bool m_running = true;

    uint32_t    m_screenWidth  = 800;
    uint32_t    m_screenHeight = 600;
    std::string m_windowName   = "Game";
};
#endif
