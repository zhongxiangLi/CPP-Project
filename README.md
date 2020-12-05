线程池连接池：
    是C++实现封装线程以及连接myhsql连接池，具体 参考项目内的README.md

Reactor目录：
    是 C++封装Reactor模型与Epoll的框架，实现了socket监听，连接等；具体 参考项目内的README.md

HttpServer目录：
    基于Reactor实现的一个Http服务器，完成功能：接收到HTTP连接默认返回一个404页面，框架已成，可以修改添加其他页面
    
HttpClient目录：
    基于TCP实现的Http客户端，运行后默认连接HttpServer目录下的http服务器，获取的输出信息是个404 html

SkipListTimer目录：
    基于跳表实现的一个定时器 demo
    
STL目录：
    对于STL内存池，二级内存配置的实现代码
  
MakeGuid.c:
    游戏服务器生成唯一ID的源码+详细注释

