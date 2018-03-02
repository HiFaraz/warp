#ifndef WARP_NET_HTTP_REQUEST_H
#define WARP_NET_HTTP_REQUEST_H

// #include <cstring> // strncmp, std::size_t
// #include <stdexcept> // std::runtime_error
// #include <string> // std::string

// #include "picohttpparser.h" // phr_header, phr_parse_request

#include "warp/buffer.h" // source_buffer
// #include "warp/console.h" // console

// #define WARP_NET_HTTP_REQUEST_PARSER_NUM_HEADERS 100

namespace warp {

  namespace http {

    // using Header = struct phr_header;

    // namespace RequestParser {

    //     auto parse_request_line(Buffer& buffer);
    //     auto seek_next_crlf(const Buffer& buffer, const int& from = 0);

    // };

    class request {

      public:
        // Header headers[WARP_NET_HTTP_REQUEST_PARSER_NUM_HEADERS];
        // const char* method;
        // const char* path;
        // int minor_version;
        // std::size_t num_headers;
        // std::size_t method_len;
        // std::size_t path_len;
        // // // Buffer         body
        // // Buffer*        buffer
        // // // unordered_map  headers
        // // // ???            ip
        // // std::string         method
        // // // unordered_map  query
        // // std::string         url

        // auto parse(const Buffer& buffer);

      private:
        // Buffer* buffer;
        // friend class Server;
    };

    // auto Request::parse(const Buffer& buffer) {
    //   num_headers = WARP_NET_HTTP_REQUEST_PARSER_NUM_HEADERS;
    //   return phr_parse_request(buffer.data, buffer.length, &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, 0);
    // }

    // // TODO generalize for any target any length and move to Buffer
    // auto RequestParser::seek_next_crlf(const Buffer& buffer, const int& from) {
    //   // TODO validate from (not negative, not bigger than buffer)
    //   auto found = false;
    //   auto pos = from - 1;
    //   auto seek_pos = 0;

    //   // TODO remove log statements
    //   // console::log("START from: " + std::to_string(from));
    //   // console::log("- buffer size: " + std::to_string(buffer.length));

    //   while (
    //     buffer.length - pos > 2 &&
    //     !found
    //   ) {
    //     // console::log("- loop at: " + std::to_string(pos+1));
    //     seek_pos = buffer.seek_next("\r", pos + 1);
    //     // console::log("- next \\r: " + std::to_string(seek_pos));
    //     if (seek_pos == -1) {
    //       break;
    //     }
    //     found = strncmp(buffer.data + seek_pos + 1, "\n", 1) == 0;
    //     // console::log("- test for \\n: " + std::to_string(found));
    //     pos = seek_pos;
    //   }
    //   return found ? pos : -1;
    // }

    // auto RequestParser::parse_request_line(Buffer& buffer) {
    //   /**
    //    * Validate HTTP/1.1
    //    * Validate only three words
    //    * Validate method
    //    * Extract URL
    //    * Extract method
    //    */

    //   auto versionErr = strncmp(buffer.data, "HTTP/1.1 ", 9) != 0;
    //   // TODO if error stop

    //   auto crlf_pos = seek_next_crlf(buffer);
    //   if (crlf_pos < 14) {
    //     // invalid, the minimum request line is "HTTP/1.1 / GET"
    //     // TODO handle invalid first line (too short)
    //   }

    //   // going to use s<X>_pos as variable names for space positions
    //   // auto s1_pos = seek_next
    //   return 1;
    // }

  }

}

#endif // !WARP_NET_HTTP_REQUEST_H

    //  *   - Buffer         body
    //  *   - Buffer*        buffer
    //  *   - unordered_map  headers
    //  *   - ???            ip
    //  *   - string         method
    //  *   - unordered_map  query
    //  *   - string         url
    //  *  
    //  *  Methods:
    //  *   - parse_headers()
    //  *   - parse_ip()
    //  *   - parse_query()