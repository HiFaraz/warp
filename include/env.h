#ifndef ENV_H
#define ENV_H

#include <cstdlib> // import std::getenv
#include <string> // import std::stoi

namespace env {

  int get_int(char const* name, int fallback) {
    char const* value = std::getenv(name);
    if (value != nullptr) {
      return std::stoi(value);
    } else {
      return fallback;
    }
  }

}

#endif // !ENV_H