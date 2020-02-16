#include "../include/log.h"
#include <chrono>
#include <ctime>
#include <iomanip>

static core::log::Console* s_Console = new core::log::Console();

static void timestamp(std::ostream& stream){
  stream << "[";
  auto now      = std::chrono::system_clock::now();
  std::time_t t = std::chrono::system_clock::to_time_t(now);
  auto gmt      = std::localtime(&t);
  auto ts       = std::put_time(gmt, "%H:%M:%S");
  
  stream << ts;
  stream << "] ";
}

void core::log::Console::init(){

}

core::log::Console& core::log::Console::instance(){
  return *s_Console;
}

core::log::Console::Console()
:m_out(std::cout)
,m_error(std::cerr)
,m_debug(std::cout)
{}

std::ostream& core::log::Console::debug(){
  timestamp(m_out);
  return m_out;
}

std::ostream& core::log::Console::error(){
  timestamp(m_error);
  return m_error;
}


std::ostream& cprint_error(const std::string& id){
  std::ostream& errorStream = core::log::Console::instance().error();
  errorStream << "{" << id << "}: ";
  return errorStream;
}

std::ostream& cprint_error(const std::string&& id){
  std::ostream& errorStream = core::log::Console::instance().error();
  errorStream << "{" << id << "}: ";
  return errorStream;
}

std::ostream& cprint_debug(const std::string& id){
  std::ostream& debugStream = core::log::Console::instance().debug();
  debugStream << "{" << id << "}: "; 
  return debugStream;
}

std::ostream& cprint_debug(const std::string&& id){
  std::ostream& debugStream = core::log::Console::instance().debug();
  debugStream << "{" << id << "}: "; 
  return debugStream;
}
