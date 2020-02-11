#include <iostream>
#include <string>
#include "src/context.h"
#include "src/config.h"

int main(int argc,char** argv){
    std::string config(argv[1]);
    lua::Script configScript(config);
    configScript.load();
    
    ContextConfig rootConfig;
    rootConfig.parse(configScript.root());
    
    std::cout << rootConfig << std::endl;

    Context context;
    context.addScripts(rootConfig.scripts());
    context.init();
    for(int i =0; i < 2;i++){
      auto& tris = context.renderer().createDrawable(context.renderPrims()[render::Primitives::Triangle]);    
      tris.position.setPosition(i*0.125,0,0);
      tris.position.scale(0.125f);
    }
    context.loopForever();
    return 0;
}
