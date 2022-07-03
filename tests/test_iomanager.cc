#include "sylar/sylar.h"
#include "sylar/sylar.h"
#include "sylar/iomanager.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/epoll.h>

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();
int sock = 0;

void test_fiber(){
    SYLAR_LOG_INFO(g_logger) << "test_fiber" << sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sock, F_SETFL, O_NONBLOCK);

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    inet_pton(AF_INET, "115.239.210.27", &addr.sin_addr.s_addr);
    if(!connect(sock, (const sockaddr*)&addr, sizeof(addr))){

    }else if(errno == EINPROGRESS){
        SYLAR_LOG_INFO(g_logger) << "add event error=" << errno << " " << stderr(errno);
        sylar::IOManager::GetThis().addEvent(sock, sylar::IOManager::READ, [](){
                SYLAR_LOG_INFO(g_logger) << "read callback";
        });
        sylar::IOManager::GetThis().addEvent(sock, sylar::IOManager::WRITE, [](){
                SYLAR_LOG_INFO(g_logger) << "write callback";
                // close(sock);
                sylar::IOManager::GetThis()->cancelEvent(sock, sylar::IOManager::READ);
                close(sock);
        });
    }else{
        SYLAR_LOG_INFO(g_logger) << "else" << errno  << " " << strerror(errno);
     }


}

void test01(){

    std::cout << "EPOLLIN" << EPOLLIN
            << "EPOLLOUT" << EPOLLOUT
    sylar::IOManager iom;
    iom.schedule(&tet_fiber);

}

int main(int argc, char** argv[]){

    test01();
    return 0;
}