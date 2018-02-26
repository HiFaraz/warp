#ifndef PHOTON_ENV_H
#define PHOTON_ENV_H

#include <cstdlib> // std::getenv
#include <string> // std::stoi

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

#endif // !PHOTON_ENV_H