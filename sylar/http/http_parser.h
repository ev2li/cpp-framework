#ifndef __SYLAR__HTTP_PARSER_H__
#define __SYLAR__HTTP_PARSER_H__

#include "http.h"
#include "http11_parser.h"
#include "httpclient_parser.h"

namespace sylar {
namespace http {
class HttpRequsetParser{
public:
    typedef std::shared_ptr<HttpRequsetParser> ptr;
    HttpRequsetParser();

    size_t execute(const char* data, size_t len, size_t off);
    int isFinished() const;
    int hasError() const;

private:
    http_parser m_parser;
    HttpRequest::ptr m_request;
    int m_error;
};

class HttpResponseParser{
public:
    typedef std::shared_ptr<HttpResponseParser> ptr;
    HttpResponseParser();
    size_t execute(const char* data, size_t len, size_t off);
    int isFinished() const;
    int hasError() const;

private:
    /// httpclient_parser
    httpclient_parser m_parser;
    /// HttpResponse
    HttpResponse::ptr m_data;
    /// 错误码
    /// 1001: invalid version
    /// 1002: invalid field
    int m_error;
};

}/* end ns sylar*/
} /* end ns http*/
#endif /* __SYLAR__HTTP_PARSER_H__ */
