#include <iostream>
#include <string>
#include "src/context.h"
#include <utils.h>
#include "src/types.h"
#include <thread>
#include <csignal>

bool Running = true;

void signalHandler(int sig){
  Running = false;
}

const char* str = "print a";
int main(int argc,char** argv){
  signal(SIGINT,signalHandler);
  
  std::string dir(argv[0]);
  std::string config(argv[1]);
      
  Context context;  


  
  std::thread thread([&]{
    context.init();
    context.loopForever();   
  });
  
  std::string line;
  while(Running){
    std::cin.clear();
    std::getline(std::cin,line);
    context.loadLua((const uint8_t*)line.c_str(),line.size()); 
    std::this_thread::sleep_for(std::chrono::seconds(10));
    //   context.loadLua((const uint8_t*)"print(a)",8);
  } 
 
  return 0;

}
