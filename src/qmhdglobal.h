#ifndef HSGLOBAL_H
#define HSGLOBAL_H

#include <QHash>
#include <QString>

class QDateTime;

/* Request methods taken from http-parser */
#define QMHD_METHOD_MAP(XX)     \
  XX(DELETE,      DELETE)       \
  XX(GET,         GET)          \
  XX(HEAD,        HEAD)         \
  XX(POST,        POST)         \
  XX(PUT,         PUT)          \
  /* pathological */            \
  XX(CONNECT,     CONNECT)      \
  XX(OPTIONS,     OPTIONS)      \
  XX(TRACE,       TRACE)        \
  /* webdav */                  \
  XX(COPY,        COPY)         \
  XX(LOCK,        LOCK)         \
  XX(MKCOL,       MKCOL)        \
  XX(MOVE,        MOVE)         \
  XX(PROPFIND,    PROPFIND)     \
  XX(PROPPATCH,   PROPPATCH)    \
  XX(SEARCH,      SEARCH)       \
  XX(UNLOCK,      UNLOCK)       \
  /* subversion */              \
  XX(REPORT,      REPORT)       \
  XX(MKACTIVITY,  MKACTIVITY)   \
  XX(CHECKOUT,    CHECKOUT)     \
  XX(MERGE,       MERGE)        \
  /* upnp */                    \
  XX(MSEARCH,     M-SEARCH)     \
  XX(NOTIFY,      NOTIFY)       \
  XX(SUBSCRIBE,   SUBSCRIBE)    \
  XX(UNSUBSCRIBE, UNSUBSCRIBE)  \
  /* RFC-5789 */                \
  XX(PATCH,       PATCH)        \
  XX(PURGE,       PURGE)        \
  /* CalDAV */                  \
  XX(MKCALENDAR,  MKCALENDAR)   \

enum class QMHDMethod
{
    Unknown = -1,
#define XX(name, string) name,
    QMHD_METHOD_MAP(XX)
#undef XX
};

QMHDMethod qmhd_method_from_string(const char* method);
const char* qmhd_method_to_string(QMHDMethod method);

enum class QMHDHttpVersion
{
    Unknown  = -1,
    Http_1_0 = 0x0100,
    Http_1_1 = 0x0101,
};

QMHDHttpVersion qmhd_http_version_from_string(const char* http_version);

#define HS_HTTP_STATUS_MAP(XX)                                              \
    XX(100, Continue, Continue)                                             \
    XX(101, SwitchingProtocols, Switching Protocols)                        \
    XX(200, Ok, Ok)                                                         \
    XX(201, Created, Created)                                               \
    XX(202, Accepted, Accepted)                                             \
    XX(203, NonAuthoritativeInformation, Non-Authoritative Information)     \
    XX(204, NoContent, No Content)                                          \
    XX(205, ResetContent, Reset Content)                                    \
    XX(206, PartialContent, Partial Content)                                \
    XX(300, MultipleChoices, Multiple Choices)                              \
    XX(301, MovedPermanently, Moved Permanently)                            \
    XX(302, MovedTemporarily, MovedTemporarily)                             \
    XX(303, SeeOther, See Other)                                            \
    XX(304, NotModified, Not Modified)                                      \
    XX(305, UseProxy, Use Proxy)                                            \
    XX(307, TemporaryRedirect, Temporary Redirect)                          \
    XX(308, PermanentRedirect, Permanent Redirect)                          \
    XX(400, BadRequest, Bad Request)                                        \
    XX(401, Unauthorized, Unauthorized)                                     \
    XX(402, PaymentRequired, Payment Required)                              \
    XX(403, Forbidden, Forbidden)                                           \
    XX(404, NotFound, NotFound)                                             \
    XX(405, MethodNotAllowed, Method Not Allowed)                           \
    XX(406, NotAcceptable, Not Acceptable)                                  \
    XX(407, ProxyAuthenticationRequired, Proxy Authentication Required)     \
    XX(408, RequestTimeout, Request Time-out)                               \
    XX(409, Conflict, Conflict)                                             \
    XX(410, Gone, Gone)                                                     \
    XX(411, LengthRequired, Length Required)                                \
    XX(412, PreconditionFailed, Precondition Failed)                        \
    XX(413, RequestEntityTooLarge, Request Entity Too Large)                \
    XX(414, RequestUriTooLong, Request-URI Too Long)                        \
    XX(415, UnsupportedMediaType, Unsupported Media Type)                   \
    XX(416, RequestedRangeNotSatisfiable, Requested Range Not Satisfiable)  \
    XX(417, ExpectationFailed, Expectation Failed)                          \
    XX(500, InternalServerError, Internal Server Error)                     \
    XX(501, NotImplemented, Not Implemented)                                \
    XX(502, BadGateway, Bad Gateway)                                        \
    XX(503, ServiceUnavailable, Service Unavailable)                        \
    XX(504, GatewayTimeout, Gateway Timeout)                                \
    XX(505, HttpVersionNotSupported, HTTP Version Not Supported)            \

enum class QMHDHttpStatus
{
#define XX(num, name, string) name = num,
    HS_HTTP_STATUS_MAP(XX)
#undef XX
};

typedef QHash<QString,QString> QStringHash;

QString qmhd_header_date(const QDateTime& dt);

#endif // HSGLOBAL_H

