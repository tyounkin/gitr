#ifndef _GITR_FILE_IO_
#define _GITR_FILE_IO_

#include <libconfig.h++>
#include <string>
#include <cstring>
#include "Field.hpp"

int importLibConfig(libconfig::Config &cfg, const std::string filepath);

template <typename P>
P getVariable(libconfig::Config &cfg, const std::string s);

extern template int getVariable(libconfig::Config &cfg, const std::string s);
extern template float getVariable(libconfig::Config &cfg, const std::string s);
extern template double getVariable(libconfig::Config &cfg, const std::string s);
extern template const char* getVariable(libconfig::Config &cfg, const std::string s);

//template <typename F>
//F importField(libconfig::Config &cfg,std::string s);
//extern template Field2d_rz importField(libconfig::Config &cfg,std::string s);
//extern template Field2d_xz importField(libconfig::Config &cfg,std::string s);
#endif
