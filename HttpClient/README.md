基于TCP实现的一个HTTP客户端程序，

http.c同步请求，获取第三方数据，将main函数中#if 0修改成#if 1即可
编译 


#gcc http.c -o http

#./http

#if 0 实现的是请求 HttpReactor启动的服务器程序，输出为获取的404 html信息

async_http.c 异步请求，获取第三方数据

#gcc async_http.c -o async_http -lpthread

#./async_http


