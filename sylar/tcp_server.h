#ifndef __SYLAR_TCP_SERVER_H__
#define __SYLAR_TCP_SERVER_H__

#include <memory>
#include <functional>
#include "address.h"
#include "iomanager.h"
#include "socket.h"
#include "noncopyable.h"
#include "config.h"

namespace sylar{
class TcpServer : public std::enable_shared_from_this<TcpServer>{
public:
    typedef std::shared_ptr<TcpServer> ptr;
    /**
     * @brief 构造函数
     * @param[in] worker socket客户端工作的协程调度器
     * @param[in] accept_worker 服务器socket执行接收socket连接的协程调度器
     */   
    TcpServer(sylar::IOManager* worker = sylar::IOManager::GetThis()
            , sylar::IOManager* io_worker = sylar::IOManager::GetThis()
            , sylar::IOManager* accept_worker = sylar::IOManager::GetThis());

    /**
     * @brief 析构函数
     */
    virtual ~TcpServer();
private:
    std::vector<Socket::ptr> m_socks;
    IOManager* m_worker;
    uint64_t m_readTimeout;
    std::string m_name;
    bool m_isStop;
};
} /* end ns sylar */

#endif /* __SYLAR_TCP_SERVER_H__ */
