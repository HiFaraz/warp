#ifndef WARP_NET_HTTP_STATUS_H
#define WARP_NET_HTTP_STATUS_H

#include "warp/buffer.h" // struct char_string

namespace warp {

  namespace http {

    // 1xx - Informational
    struct char_string CONTINUE                           = { "100 Continue\r\n", 14};
    struct char_string SWITCHING_PROTOCOLS                = { "101 Switching Protocols\r\n", 25};

    // 2XX - Successful
    struct char_string OK                                 = { "200 OK\r\n", 8};
    struct char_string CREATED                            = { "201 Created\r\n", 13};
    struct char_string ACCEPTED                           = { "202 Accepted\r\n", 14};
    struct char_string NON_AUTHORITATIVE_INFORMATION      = { "203 Non-Authoritative Information\r\n", 35};
    struct char_string NO_CONTENT                         = { "204 No Content\r\n", 16};
    struct char_string RESET_CONTENT                      = { "205 Reset Content\r\n", 19};
    struct char_string PARTIAL_CONTENT                    = { "206 Partial Content\r\n", 21};
    
    // 3xx - Redirection
    struct char_string MULTIPLE_CHOICES                   = { "300 Multiple Choices\r\n", 22};
    struct char_string MOVED_PERMANENTLY                  = { "301 Moved Permanently\r\n", 23};
    struct char_string FOUND                              = { "302 Found\r\n", 11}; // TODO fix remaining sizes
    struct char_string SEE_OTHER                          = { "303 See Other\r\n", 15};
    struct char_string NOT_MODIFIED                       = { "304 Not Modified\r\n", 18};
    struct char_string USE_PROXY                          = { "305 Use Proxy\r\n", 15};
    struct char_string TEMPORARY_REDIRECT                 = { "307 Temporary Redirect\r\n", 24};

    // 4xx - Client error
    struct char_string BAD_REQUEST                        = { "400 Bad Request\r\n", 17};
    struct char_string UNAUTHORIZED                       = { "401 Unauthorized\r\n", 18};
    struct char_string PAYMENT_REQUIRED                   = { "402 Payment Required\r\n", 22};
    struct char_string FORBIDDEN                          = { "403 Forbidden\r\n", 15};
    struct char_string NOT_FOUND                          = { "404 Not Found\r\n", 15};
    struct char_string METHOD_NOT_ALLOWED                 = { "405 Method Not Allowed\r\n", 24};
    struct char_string NOT_ACCEPTABLE                     = { "406 Not Acceptable\r\n", 20};
    struct char_string PROXY_AUTHENTICATION_REQUIRED      = { "407 Proxy Authentication Required\r\n", 35};
    struct char_string REQUEST_TIMEOUT                    = { "408 Request Timeout\r\n", 21};
    struct char_string CONFLICT                           = { "409 Conflict\r\n", 14};
    struct char_string GONE                               = { "410 Gone\r\n", 10};
    struct char_string LENGTH_REQUIRED                    = { "411 Length Required\r\n", 21};
    struct char_string PRECONDITION_FAILED                = { "412 Precondition Failed\r\n", 25};
    struct char_string REQUEST_ENTITY_TOO_LARGE           = { "413 Request Entity Too Large\r\n", 30};
    struct char_string REQUEST_URI_TOO_LONG               = { "414 Request-URI Too Long\r\n", 26};
    struct char_string UNSUPPORTED_MEDIA_TYPE             = { "415 Unsupported Media Type\r\n", 28};
    struct char_string REQUESTED_RANGE_NOT_SATISFIABLE    = { "416 Requested Range Not Satisfiable\r\n", 37};
    struct char_string EXPECTATION_FAILED                 = { "417 Expectation Failed\r\n", 24};

    // 5xx - Server error
    struct char_string INTERNAL_SERVER_ERROR              = { "500 Internal Server Error\r\n", 27};
    struct char_string NOT_IMPLEMENTED                    = { "501 Not Implemented\r\n", 21};
    struct char_string BAD_GATEWAY                        = { "502 Bad Gateway\r\n", 17};
    struct char_string SERVICE_UNAVAILABLE                = { "503 Service Unavailable\r\n", 25};
    struct char_string GATEWAY_TIMEOUT                    = { "504 Gateway Timeout\r\n", 21};
    struct char_string HTTP_VERSION_NOT_SUPPORTED         = { "505 HTTP Version Not Supported\r\n", 32};
  }

}

#endif // !WARP_NET_HTTP_STATUS_H