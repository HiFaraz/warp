#ifndef FTL_NET_HTTP_MESSAGE_H
#define FTL_NET_HTTP_MESSAGE_H

#include <cstddef> // std::size_t

namespace http {

  auto new_ln() {
    return "\r\n";
  }

  const auto HTTP_MAX_HEADER_SIZE() {
    return std::size_t{8 * 1024};// 8 KiB
  }

}

#endif // !FTL_NET_HTTP_MESSAGE_H