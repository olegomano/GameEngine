#ifndef _CORE_UTILS_H_
#define _CORE_UTILS_H_
#include <string>

namespace core{
namespace utils{

std::string dirFromPath(const std::string& path);
std::string fileFromPath(const std::string& path);
std::string joinPath(const std::string& dir, const std::string& name);
};
};


#endif
