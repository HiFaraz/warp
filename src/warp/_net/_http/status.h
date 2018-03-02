#ifndef WARP_NET_HTTP_STATUS_H
#define WARP_NET_HTTP_STATUS_H

#include <string> // std::string

namespace warp {

  namespace http {

    struct status {
      int code;
      std::string description;
    };

    // 1xx - Informational
    struct status CONTINUE = { 100, "Continue" };
    struct status SWITCHING_PROTOCOLS = { 101, "Switching Protocols" };

    // 2XX - Successful
    struct status OK = { 200, "OK" };
    struct status CREATED = { 201, "Created" };
    struct status ACCEPTED = { 202, "Accepted" };
    struct status NON_AUTHORITATIVE_INFORMATION = { 203, "Non-Authoritative Information" };
    struct status NO_CONTENT = { 204, "No Content" };
    struct status RESET_CONTENT = { 205, "Reset Content" };
    struct status PARTIAL_CONTENT = { 206, "Partial Content" };
    
    // 3xx - Redirection
    struct status MULTIPLE_CHOICES = { 300, "Multiple Choices" };
    struct status MOVED_PERMANENTLY = { 301, "Moved Permanently" };
    struct status FOUND = { 302, "Found" };
    struct status SEE_OTHER = { 303, "See Other" };
    struct status NOT_MODIFIED = { 304, "Not Modified" };
    struct status USE_PROXY = { 305, "Use Proxy" };
    struct status TEMPORARY_REDIRECT = { 307, "Temporary Redirect" };

    // 4xx - Client error
    struct status BAD_REQUEST = { 400, "Bad Request" };
    struct status UNAUTHORIZED = { 401, "Unauthorized" };
    struct status PAYMENT_REQUIRED = { 402, "Payment Required" };
    struct status FORBIDDEN = { 403, "Forbidden" };
    struct status NOT_FOUND = { 404, "Not Found" };
    struct status METHOD_NOT_ALLOWED = { 405, "Method Not Allowed" };
    struct status NOT_ACCEPTABLE = { 406, "Not Acceptable" };
    struct status PROXY_AUTHENTICATION_REQUIRED = { 407, "Proxy Authentication Required" };
    struct status REQUEST_TIMEOUT = { 408, "Request Timeout" };
    struct status CONFLICT = { 409, "Conflict" };
    struct status GONE = { 410, "Gone" };
    struct status LENGTH_REQUIRED = { 411, "Length Required" };
    struct status PRECONDITION_FAILED = { 412, "Precondition Failed" };
    struct status REQUEST_ENTITY_TOO_LARGE = { 413, "Request Entity Too Large" };
    struct status REQUEST_URI_TOO_LONG = { 414, "Request-URI Too Long" };
    struct status UNSUPPORTED_MEDIA_TYPE = { 415, "Unsupported Media Type" };
    struct status REQUESTED_RANGE_NOT_SATISFIABLE = { 416, "Requested Range Not Satisfiable" };
    struct status EXPECTATION_FAILED = { 417, "Expectation Failed" };

    // 5xx - Server error
    struct status INTERNAL_SERVER_ERROR = { 500, "Internal Server Error" };
    struct status NOT_IMPLEMENTED = { 501, "Not Implemented" };
    struct status BAD_GATEWAY = { 502, "Bad Gateway" };
    struct status SERVICE_UNAVAILABLE = { 503, "Service Unavailable" };
    struct status GATEWAY_TIMEOUT = { 504, "Gateway Timeout" };
    struct status HTTP_VERSION_NOT_SUPPORTED = { 505, "HTTP Version Not Supported" };
  }

}

#endif // !WARP_NET_HTTP_STATUS_H