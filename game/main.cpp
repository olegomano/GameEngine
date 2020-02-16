#include <iostream>
#include <string>
#include "src/context.h"
#include "src/config.h"
#include <utils.h>

int main(int argc,char** argv){
    std::string dir(argv[0]);
    std::string config(argv[1]);

    core::file::File configFile(config);
    configFile.load();

    lua::Script configScript(configFile);
    configScript.load();
    
    std::cout << dir << std::endl;

    ContextConfig rootConfig;
    rootConfig.parse(configScript.root());
    
    std::cout << rootConfig << std::endl;

    Context context;
    std::string workingDir = core::utils::dirFromPath(config);
    for(auto& str : rootConfig.scripts()){
        std::string fullPath = core::utils::joinPath(workingDir,str);
        context.addScript(fullPath);
    }
    context.init();
    context.loopForever();
    return 0;
}
