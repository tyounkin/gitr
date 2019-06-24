#ifndef _GITR_FILE_IO_
#define _GITR_FILE_IO_

#include <libconfig.h++>
#include <string>
#include <cstring>

int importLibConfig(libconfig::Config &cfg, const std::string filepath);

template <typename P>
P getVariable(libconfig::Config &cfg, const std::string s);

extern template int getVariable(libconfig::Config &cfg, const std::string s);
extern template float getVariable(libconfig::Config &cfg, const std::string s);
extern template double getVariable(libconfig::Config &cfg, const std::string s);
extern template const char* getVariable(libconfig::Config &cfg, const std::string s);
#endif
