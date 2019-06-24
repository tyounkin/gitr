#include "file_io.hpp"
#include <iostream>

int importLibConfig(libconfig::Config &cfg, const std::string filepath) {
    try {
      cfg.readFile(filepath.c_str());
    } catch (const libconfig::FileIOException &fioex) {
      std::cerr << "Error while opening " << filepath << std::endl;
      std::cerr << "It is possible this file does not exist" << std::endl;
      exit (EXIT_FAILURE);
    } catch (const libconfig::ParseException &pex) {
      std::cerr << "Parse error of " << filepath << " at " << pex.getFile() << ":" << pex.getLine()
                << " - " << pex.getError() << std::endl;
      exit (EXIT_FAILURE);
    }
    std::cout << "Finished libconfig import  " << filepath.c_str() << std::endl;
    return (EXIT_SUCCESS);
}

template <typename P>
P getVariable(libconfig::Config &cfg, const std::string s) {
  P tmp;
  if (cfg.lookupValue(s, tmp)) {
    std::cout << s << " = " << tmp << std::endl;
  } else {
    std::cout << "ERROR: Failed importing " << s << std::endl;
    exit(EXIT_FAILURE);
  }
  return tmp;
}

template int getVariable(libconfig::Config &cfg, const std::string s);
template float getVariable(libconfig::Config &cfg, const std::string s);
template double getVariable(libconfig::Config &cfg, const std::string s);
template const char* getVariable(libconfig::Config &cfg, const std::string s);
