#ifndef FTL_CONSOLE_H
#define FTL_CONSOLE_H

#include <iostream>

namespace console {
  
  template <typename T>
  void error(T message) {
    std::cerr << message << std::endl;
  }

  template <typename T>
  void log(T message) {
    std::cout << message << std::endl;
  }

}

#endif // !FTL_CONSOLE_H