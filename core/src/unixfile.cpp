#include "../include/file.h"

core::file::UnixFile::UnixFile(const core::file::UnixFile& f) : core::file::IFile(f){
  
}

bool core::file::UnixFile::pollChanges(){
  return false;
}
