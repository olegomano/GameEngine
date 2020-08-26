#include <iostream>
#include <string>
#include "src/context.h"
#include <utils.h>
#include "src/types.h"
#include <thread>
#include <csignal>

bool Running = true;
Context context;    

void signalHandler(int sig){
  Running = false;
}

const char* str = "print a";
int main(int argc,char** argv){
  signal(SIGINT,signalHandler);

  
  if(argc > 1){
    std::string workdir = std::string(argv[0]);
    for(int i = 1; i < argc; i++){
      std::string filepath = std::string(argv[i]);
      std::cout << "Adding init script " << filepath << std::endl;
      context.addInitScript(filepath);
    }
  }  
    
  std::thread thread([&]{
    context.init();
    context.loopForever();   
    Running = false;
  });
  
  std::string line;
  core::Latch latch;
  while(Running){
    std::cin.clear();
    std::getline(std::cin,line);
    auto function = &Context::loadLuaBuffer;
    context.runAsync( function, latch,line );
    //latch.pass();
    //   context.loadLua((const uint8_t*)"print(a)",8);
  }
  std::cout << "Closing" << std::endl;
  return 0;

}
