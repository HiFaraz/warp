#ifndef WARP_NET_HTTP_REQUEST_H
#define WARP_NET_HTTP_REQUEST_H

// #include <cstring> // std::size_t
// #include <string> // std::string

#include "picohttpparser.h" // phr_header, phr_parse_request

#include "warp/buffer.h" // slice_buffer, source_buffer

#define WARP_NET_HTTP_REQUEST_PARSER_NUM_HEADERS 100

namespace warp {

  namespace http {

    using header = struct phr_header;

    class request {

      public:
        header headers[WARP_NET_HTTP_REQUEST_PARSER_NUM_HEADERS];
        std::string method;
        std::string url;
        int minor_version;
        std::size_t num_headers;

      private:
        friend class server;
        auto parse_(source_buffer& buffer);
        slice_buffer method_;
        slice_buffer url_;
    };

    auto request::parse_(source_buffer& buffer) {
      num_headers = WARP_NET_HTTP_REQUEST_PARSER_NUM_HEADERS;
      auto parse_result = phr_parse_request(buffer.begin(), buffer.size(), &method_.data_, &method_.size_, &url_.data_, &url_.size_, &minor_version, headers, &num_headers, 0);
      // TODO if (parse successful)
      method = method_.to_string();
      url = url_.to_string();
    }

  }

}

#endif // !WARP_NET_HTTP_REQUEST_H