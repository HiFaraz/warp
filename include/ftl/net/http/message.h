#ifndef FTL_NET_HTTP_MESSAGE_H
#define FTL_NET_HTTP_MESSAGE_H

#include <cstddef> // std::size_t

namespace http {

  const char* new_ln() { return "\r\n"; }

  const std::size_t HTTP_MAX_HEADER_SIZE() { return 8 * 1024; } // 8 KiB

}

#endif // !FTL_NET_HTTP_MESSAGE_H