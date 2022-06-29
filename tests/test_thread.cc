#include "sylar/sylar.h"
#include "sylar/config.h"

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

int count = 0;
// sylar::RWMutex s_mutex;
sylar::Mutex s_mutex;
void fun1(){
    SYLAR_LOG_INFO(g_logger) << "name: " << sylar::Thread::GetName()    
                            << "this.name:" << sylar::Thread::GetThis()->getName()
                            << "id: " << sylar::GetThreadId()
                            << "this.id" << sylar::Thread::GetThis()->getId();
    for(int i = 0; i < 100; i++){
        // sylar::RWMutex::WriteLock lock(s_mutex);
        sylar::Mutex::Lock lock(s_mutex);
        ++count;
    }
}

void fun2(){
    while(true){
        SYLAR_LOG_INFO(g_logger) << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";   
    }
}

void fun3(){
    while(true){
         SYLAR_LOG_INFO(g_logger) << "======================================";
    }
}

int main(int argc, char const *argv[])
{   
    SYLAR_LOG_INFO(g_logger) << "thread test begin";
    YAML::Node root = YAML::LoadFile("/Users/zhangli/dw/学习/程序/c:c++项目学习/cpp-framework/bin/config/log2.yml");
    sylar::Config::LoadFromYaml(root);

    std::vector<sylar::Thread::ptr> thrs;
#if 0    
    for (size_t i = 0; i < 5; i++) {
        sylar::Thread::ptr thr(new sylar::Thread(&fun1, "name_" + std::to_string(i)));
        thrs.push_back(thr);
    }
    for(int i = 0; i < 5; i++){
        thrs[i]->join();
    }
#endif

    for (size_t i = 0; i < 2; i++) {
        sylar::Thread::ptr thr(new sylar::Thread(&fun1, "name_" + std::to_string(i * 2)));
        sylar::Thread::ptr thr2(new sylar::Thread(&fun2, "name_" + std::to_string(i * 2 
        + 1)));

        thrs.push_back(thr);
        thrs.push_back(thr2);
    }
    for(int i = 0; i < 2; i++){
        thrs[i]->join();
    }

    SYLAR_LOG_INFO(g_logger) << "thread test end";
    SYLAR_LOG_INFO(g_logger) << "count" << count;

    sylar::Config::Visit([](sylar::ConfigVarBase::ptr var){
        SYLAR_LOG_INFO(g_logger) << "name = " << var->getName()
                    << " description = " << var->getDescription()
                    << " typename = " << var->getTypeName()
                    << " value =  " <<  var->toString();
    });
    return 0;
}
