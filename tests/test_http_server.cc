#include "sylar/http/http_server.h"
#include "sylar/log.h"

#define XX(...) #__VA_ARGS__
static sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();
sylar::IOManager::ptr worker;
 
void run(){
    sylar::http::HttpServer::ptr server(new sylar::http::HttpServer);
    sylar::Address::ptr addr = sylar::Address::LookupAnyIPAddress("0.0.0.0:8082");
    while (!server->bind(addr)) {
        sleep(2);
    }
    auto sd = server->getServletDispatch();
    sd->addServlet("/sylar/xx", [](sylar::http::HttpRequest::ptr req
                ,sylar::http::HttpResponse::ptr rsp
                ,sylar::http::HttpSession::ptr session) {
            rsp->setBody(req->toString());
            return 0;
    });

    sd->addGlobServlet("/sylar/*", [](sylar::http::HttpRequest::ptr req
                ,sylar::http::HttpResponse::ptr rsp
                ,sylar::http::HttpSession::ptr session) {
            rsp->setBody("Glob:\r\n" + req->toString());
            return 0;
    });

    sd->addGlobServlet("/sylarx/*", [](sylar::http::HttpRequest::ptr req
                ,sylar::http::HttpResponse::ptr rsp
                ,sylar::http::HttpSession::ptr session) {
            rsp->setBody(XX(<html>
<head><title>404 Not Found</title></head>
<body>
<center><h1>404 Not Found</h1></center>
<hr><center>nginx/1.16.0</center>
</body>
</html>
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
));
            return 0;
    });

    server->start();
}

int main(int argc, char const *argv[]){
    sylar::IOManager iom(2);
    iom.schedule(run);
    return 0;
}
