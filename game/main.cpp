#include <iostream>
#include <string>
#include "src/context.h"
#include "src/config.h"
#include <utils.h>
#include "src/types.h"

int main(int argc,char** argv){
  std::string dir(argv[0]);
  std::string config(argv[1]);
    
  Context context;  
  context.init();
  context.loadScript(config);
  context.loopForever();
  return 0;
}
