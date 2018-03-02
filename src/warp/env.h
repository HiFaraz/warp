#ifndef WARP_ENV_H
#define WARP_ENV_H

#include <cstdlib> // std::getenv
#include <string> // std::stoi

namespace warp {

  namespace env {

    int get_int(char const* name, int fallback) {
      auto value = std::getenv(name);
      if (value != nullptr) {
        return std::stoi(value);
      } else {
        return fallback;
      }
    }

  }

}

#endif // !WARP_ENV_H