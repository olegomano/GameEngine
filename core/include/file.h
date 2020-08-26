#ifndef _CORE_FILE_H_
#define _CORE_FILE_H_
#include <string>
#include <ostream>


namespace core{
namespace file{

class IFile{
public:
  friend std::ostream& operator<<(std::ostream& out, const IFile& file); 
  enum Mode{
    RELOAD = 0,
    LAZY
  };

  IFile(const std::string& path);
  IFile(const IFile& other);
  IFile();

  inline const uint8_t*  data() const {return m_data;}
  
  /**
   * @param flag 
   *  RELOAD if file is not loaded it will be loaded. If file is loaded it will be loaded from disk again
   *  LAZY if file is not loaded it will be loaded. If file is loaded, nothing will happen
   */
  virtual void           load(Mode flag = LAZY);
  
  /*
   *@return returns true if this file points to a valid path
   */
  inline bool            null() const { return m_path.empty();}
  
  /*
   * Not thread safe
   *@return returns true if the file changes on disk since last poll 
   */
  virtual bool           pollChanges() = 0;

  inline const std::string& path() const{return m_path;}
  inline const std::string& name() const{return m_name;}
  inline const std::string& dir()  const{return m_dir;}
  
  /*
    @return returns the size of the file in bytes
   */
  inline const size_t size(){return m_size;}

  ~IFile(){}

private:
  const std::string m_path;
  const std::string m_dir;
  const std::string m_name;
protected:
  uint8_t*          m_data = nullptr;
  size_t            m_size = -1;
};

class UnixFile : public IFile{
public:
  using IFile::IFile;
  UnixFile(const UnixFile& other);
  bool pollChanges() override;
  ~UnixFile(){}
};

typedef UnixFile File;
};
};


#endif
