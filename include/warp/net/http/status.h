#ifndef WARP_NET_HTTP_STATUS_H
#define WARP_NET_HTTP_STATUS_H

#include <string> // std::string

namespace http {

  struct Status {
    int code;
    std::string description;
  };

  // 1xx - Informational
  struct Status CONTINUE = { 100, "Continue" };
  struct Status SWITCHING_PROTOCOLS = { 101, "Switching Protocols" };

  // 2XX - Successful
  struct Status OK = { 200, "OK" };
  struct Status CREATED = { 201, "Created" };
  struct Status ACCEPTED = { 202, "Accepted" };
  struct Status NON_AUTHORITATIVE_INFORMATION = { 203, "Non-Authoritative Information" };
  struct Status NO_CONTENT = { 204, "No Content" };
  struct Status RESET_CONTENT = { 205, "Reset Content" };
  struct Status PARTIAL_CONTENT = { 206, "Partial Content" };
  
  // 3xx - Redirection
  struct Status MULTIPLE_CHOICES = { 300, "Multiple Choices" };
  struct Status MOVED_PERMANENTLY = { 301, "Moved Permanently" };
  struct Status FOUND = { 302, "Found" };
  struct Status SEE_OTHER = { 303, "See Other" };
  struct Status NOT_MODIFIED = { 304, "Not Modified" };
  struct Status USE_PROXY = { 305, "Use Proxy" };
  struct Status TEMPORARY_REDIRECT = { 307, "Temporary Redirect" };

  // 4xx - Client error
  struct Status BAD_REQUEST = { 400, "Bad Request" };
  struct Status UNAUTHORIZED = { 401, "Unauthorized" };
  struct Status PAYMENT_REQUIRED = { 402, "Payment Required" };
  struct Status FORBIDDEN = { 403, "Forbidden" };
  struct Status NOT_FOUND = { 404, "Not Found" };
  struct Status METHOD_NOT_ALLOWED = { 405, "Method Not Allowed" };
  struct Status NOT_ACCEPTABLE = { 406, "Not Acceptable" };
  struct Status PROXY_AUTHENTICATION_REQUIRED = { 407, "Proxy Authentication Required" };
  struct Status REQUEST_TIMEOUT = { 408, "Request Timeout" };
  struct Status CONFLICT = { 409, "Conflict" };
  struct Status GONE = { 410, "Gone" };
  struct Status LENGTH_REQUIRED = { 411, "Length Required" };
  struct Status PRECONDITION_FAILED = { 412, "Precondition Failed" };
  struct Status REQUEST_ENTITY_TOO_LARGE = { 413, "Request Entity Too Large" };
  struct Status REQUEST_URI_TOO_LONG = { 414, "Request-URI Too Long" };
  struct Status UNSUPPORTED_MEDIA_TYPE = { 415, "Unsupported Media Type" };
  struct Status REQUESTED_RANGE_NOT_SATISFIABLE = { 416, "Requested Range Not Satisfiable" };
  struct Status EXPECTATION_FAILED = { 417, "Expectation Failed" };

  // 5xx - Server error
  struct Status INTERNAL_SERVER_ERROR = { 500, "Internal Server Error" };
  struct Status NOT_IMPLEMENTED = { 501, "Not Implemented" };
  struct Status BAD_GATEWAY = { 502, "Bad Gateway" };
  struct Status SERVICE_UNAVAILABLE = { 503, "Service Unavailable" };
  struct Status GATEWAY_TIMEOUT = { 504, "Gateway Timeout" };
  struct Status HTTP_VERSION_NOT_SUPPORTED = { 505, "HTTP Version Not Supported" };
}

#endif // !WARP_NET_HTTP_STATUS_H