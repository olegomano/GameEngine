#ifndef _CORE_LOG_H_
#define _CORE_LOG_H_
#include <iostream>
#include <string>

namespace core{
namespace log{



class Console{
public:
  static void     init();
  static Console& instance();
  Console();
  Console(const std::string& path);

  std::ostream& error();
  std::ostream& debug();
  std::ostream& out();
private:
  std::ostream& m_error;
  std::ostream& m_out;
  std::ostream& m_debug;
};

};
};



typedef core::log::Console cprint;

std::ostream& cprint_error(const std::string&& id );
std::ostream& cprint_debug(const std::string&& id );
std::ostream& cprint_error(const std::string&  id = "ERROR");
std::ostream& cprint_debug(const std::string&  id = "DEBUG");

#endif
