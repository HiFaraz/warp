#ifndef WARP_NET_HTTP_STATUS_H
#define WARP_NET_HTTP_STATUS_H

#include <string> // char const *

namespace warp {

  namespace http {

    // 1xx - Informational
    char const * CONTINUE                           = "HTTP/1.1 100 Continue\r\n";
    char const * SWITCHING_PROTOCOLS                = "HTTP/1.1 101 Switching Protocols\r\n";

    // 2XX - Successful
    char const * OK                                 = "HTTP/1.1 200 OK\r\n";
    char const * CREATED                            = "HTTP/1.1 201 Created\r\n";
    char const * ACCEPTED                           = "HTTP/1.1 202 Accepted\r\n";
    char const * NON_AUTHORITATIVE_INFORMATION      = "HTTP/1.1 203 Non-Authoritative Information\r\n";
    char const * NO_CONTENT                         = "HTTP/1.1 204 No Content\r\n";
    char const * RESET_CONTENT                      = "HTTP/1.1 205 Reset Content\r\n";
    char const * PARTIAL_CONTENT                    = "HTTP/1.1 206 Partial Content\r\n";
    
    // 3xx - Redirection
    char const * MULTIPLE_CHOICES                   = "HTTP/1.1 300 Multiple Choices\r\n";
    char const * MOVED_PERMANENTLY                  = "HTTP/1.1 301 Moved Permanently\r\n";
    char const * FOUND                              = "HTTP/1.1 302 Found\r\n";
    char const * SEE_OTHER                          = "HTTP/1.1 303 See Other\r\n";
    char const * NOT_MODIFIED                       = "HTTP/1.1 304 Not Modified\r\n";
    char const * USE_PROXY                          = "HTTP/1.1 305 Use Proxy\r\n";
    char const * TEMPORARY_REDIRECT                 = "HTTP/1.1 307 Temporary Redirect\r\n";

    // 4xx - Client error
    char const * BAD_REQUEST                        = "HTTP/1.1 400 Bad Request\r\n";
    char const * UNAUTHORIZED                       = "HTTP/1.1 401 Unauthorized\r\n";
    char const * PAYMENT_REQUIRED                   = "HTTP/1.1 402 Payment Required\r\n";
    char const * FORBIDDEN                          = "HTTP/1.1 403 Forbidden\r\n";
    char const * NOT_FOUND                          = "HTTP/1.1 404 Not Found\r\n";
    char const * METHOD_NOT_ALLOWED                 = "HTTP/1.1 405 Method Not Allowed\r\n";
    char const * NOT_ACCEPTABLE                     = "HTTP/1.1 406 Not Acceptable\r\n";
    char const * PROXY_AUTHENTICATION_REQUIRED      = "HTTP/1.1 407 Proxy Authentication Required\r\n";
    char const * REQUEST_TIMEOUT                    = "HTTP/1.1 408 Request Timeout\r\n";
    char const * CONFLICT                           = "HTTP/1.1 409 Conflict\r\n";
    char const * GONE                               = "HTTP/1.1 410 Gone\r\n";
    char const * LENGTH_REQUIRED                    = "HTTP/1.1 411 Length Required\r\n";
    char const * PRECONDITION_FAILED                = "HTTP/1.1 412 Precondition Failed\r\n";
    char const * REQUEST_ENTITY_TOO_LARGE           = "HTTP/1.1 413 Request Entity Too Large\r\n";
    char const * REQUEST_URI_TOO_LONG               = "HTTP/1.1 414 Request-URI Too Long\r\n";
    char const * UNSUPPORTED_MEDIA_TYPE             = "HTTP/1.1 415 Unsupported Media Type\r\n";
    char const * REQUESTED_RANGE_NOT_SATISFIABLE    = "HTTP/1.1 416 Requested Range Not Satisfiable\r\n";
    char const * EXPECTATION_FAILED                 = "HTTP/1.1 417 Expectation Failed\r\n";

    // 5xx - Server error
    char const * INTERNAL_SERVER_ERROR              = "HTTP/1.1 500 Internal Server Error\r\n";
    char const * NOT_IMPLEMENTED                    = "HTTP/1.1 501 Not Implemented\r\n";
    char const * BAD_GATEWAY                        = "HTTP/1.1 502 Bad Gateway\r\n";
    char const * SERVICE_UNAVAILABLE                = "HTTP/1.1 503 Service Unavailable\r\n";
    char const * GATEWAY_TIMEOUT                    = "HTTP/1.1 504 Gateway Timeout\r\n";
    char const * HTTP_VERSION_NOT_SUPPORTED         = "HTTP/1.1 505 HTTP Version Not Supported\r\n";
  }

}

#endif // !WARP_NET_HTTP_STATUS_H