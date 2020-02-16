#include "../include/file.h"
#include "../include/utils.h"
#include "../include/log.h"
#include <fstream>
#include <limits>


template<uint32_t _T_blockSize>
static uint8_t* loadFile(const std::string& path, size_t& size){ 
  std::ifstream file;
  file.open(path,std::ios::in|std::ios::binary);
  
  if(!file){
    cprint_error("File") << path << " failed to load " << std::endl;
    size = -1;
    return nullptr;
  }
  
  file.ignore( std::numeric_limits<std::streamsize>::max() );
  std::streamsize length = file.gcount();
  file.clear();   //  Since ignore will have set eof.
  file.seekg( 0, std::ios_base::beg );
 
  uint8_t* fileBuffer = new uint8_t[length]; 
  uint8_t* writePtr   = fileBuffer;

  while(file.good()){
    file.read((char*)writePtr,_T_blockSize);
    writePtr+=_T_blockSize;
  }
  
  cprint_debug("File") <<  path << " succesfully loaded, size " << length << std::endl;

  size = length;
  return fileBuffer;
} 
  

std::ostream& operator<<(std::ostream& out, const core::file::IFile& file){
  return out;
}

core::file::IFile::IFile()
  :m_path("")
  ,m_dir("")
  ,m_name("")
  ,m_data(nullptr)
  ,m_size(-1)
{}

core::file::IFile::IFile(const std::string& fp)
  :m_path(fp)
  ,m_dir(core::utils::dirFromPath(fp))
  ,m_name(core::utils::fileFromPath(fp))
  ,m_data(nullptr)
  ,m_size(-1)
  
{}

core::file::IFile::IFile(const IFile& other)
  :m_path(other.m_path)
  ,m_dir(other.m_dir)
  ,m_name(other.m_name)
  ,m_data(other.m_data)
  ,m_size(other.m_size)
{}


void core::file::IFile::load(Mode m){
 if(m_data == nullptr || m == RELOAD){
    cprint_debug("File") <<  m_path << " loading from disk, load() called in mode " << m << std::endl;
    m_data = loadFile<2048>(m_path,m_size);   
  }else{
    cprint_debug("File") << m_path << " using loaded copy, load() called in mode " << m <<  std::endl; 
  }
}
