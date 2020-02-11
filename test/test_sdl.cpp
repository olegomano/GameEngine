#include <shader.h>

#include <string>
#include <tuple>
#include <iostream>
#include <array>
#include <utility>
#include <functional>
#include "router.h"
#include "que.h"

#include "sdlGL.h"
#include "renderer.h"
#include "luascript.h"


struct Drawable{
    Transform             position;
    render::gl::VBOAttrib vertex;
    render::gl::VBOAttrib normals;    
};

class Params{
public:
    Params(const lua::LuaRef& root){
        root["width"].read(m_screenWidth);
        root["height"].read(m_screenHeight);
        m_onFrame = root["onFrame"];
        m_onKeyboard = root["onKey"];
    }

    int width() const {return m_screenWidth;}
    int height() const {return m_screenHeight;}
    const lua::LuaRef& onFrame() const {return m_onFrame;}
    const lua::LuaRef& onKey() const {return m_onKeyboard;}

    const std::vector<Transform>& objects() const {
        return m_objects;
    }

private:
    int                    m_screenWidth;
    int                    m_screenHeight;
    std::vector<Transform> m_objects;    
    lua::LuaRef            m_onFrame;
    lua::LuaRef            m_onKeyboard;
};  

static int callFromLua(lua_State* ls){
    std::cout << " Called From lua" << std::endl;
    return 0;
}

int main(int argc,char** argv){
    std::string path(argv[1]);
    lua::Script luaScript(path);
    luaScript.load();
    luaScript.registerFunction("cfunc",callFromLua);
    Params params(luaScript.root());
    
    std::map<std::string,lua::FlatTableItem> ctrisState;
    ctrisState["x"] = 0;
    ctrisState["y"] = 0;
    ctrisState["z"] = 0;
    lua::LuaRef trisState = luaScript.createTable(ctrisState);
    
    SdlGlContext glContext(params.width(),params.height());
    glContext.init();

    render::gl::Renderer<Drawable> renderer;
    renderer.init();


    std::vector<float> v = {0,0.5,0,1, 0.5,-0.5,0,1,  -0.5,-0.5,0,1};
    std::vector<float> n = {0,0,1,     0,0,1,          0,0,1};
    
    render::gl::VBO trisVBO;
    trisVBO.create(v);
    Drawable& d = renderer.createDrawable(Drawable{Transform(),trisVBO.createAttrib(3,0,0),trisVBO.createAttrib(3,0,0)});

   
    float scale = 1;
    luaScript["scale"].read(scale);

    d.position.scale(scale);
    d.position.setPosition(0,0,0);
    
    bool running = true;


    while(running){
        glContext.clearFrame();
        glContext.checkInput([&running,&params,&trisState,&d](const SDL_Event& e) {            
            switch(e.type){
                case SDL_QUIT:
                    running = false;
                break;
                case SDL_KEYDOWN:
                    //params.onKey().call('h');      
                break;
            }
        });

        params.onFrame().call(trisState);
        float x = 0;
        float y = 0;
        float z = 0;
        trisState["x"].read(x);
        trisState["y"].read(y);
        trisState["z"].read(z);
        d.position.setPosition(x,y,z);
        renderer.render(glm::mat4(1));
        glContext.showFrame();
    }
}