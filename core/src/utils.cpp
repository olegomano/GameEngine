#include "../include/utils.h"


std::string core::utils::dirFromPath(const std::string& path){
  int divider = path.find_last_of("/");
  return path.substr(0,divider);
}


std::string core::utils::fileFromPath(const std::string& path){
  int divider = path.find_last_of("/");
  return path.substr(divider+1);
}

std::string core::utils::joinPath(const std::string& dir, const std::string& file){
  std::string result = "";
  if(dir[dir.size() - 1] == '/'){
    result+=dir;
  }else{
    result+=dir;
    result+="/";
  }
  result+=file;
  return result;
}

