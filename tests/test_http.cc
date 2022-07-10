#include "sylar/http/http.h"
#include "sylar/log.h"

void test(){
    sylar::http::HttpRequest::ptr req(new sylar::http::HttpRequest);
    req->setHeader("host", "www.baidu.com");
    req->setBody("heelo sylar");

    req->dump(std::cout) << std::endl;
}

int main(int argc, char const *argv[])
{
    test();
    return 0;
}
