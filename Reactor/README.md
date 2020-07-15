C++ reactor模型与epoll 的源码实现框架

Reactor类，负责事件EventHandle的存储，多路复用器EventDemultiplex类；

Reactor类实现了事件的注册删除函数，EventHandle类主要实现事件的处理函数，EventDemultiplex实现注册等待事件等功能。

#运行方式

#make

生成可执行文件reactorepoll

运行

#./reactorepoll 

默认端口为7710 可以手动配置端口

#./reactorepoll 8989

此框架可以测试高并发连接，客户端暂时未上传。

