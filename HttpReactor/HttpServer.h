#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

class HttpServer{

public:
	HttpServer();
	~HttpServer();

	int BadRequest(const char * path,const char *head,char* sbuffer);
	int SendError(char buffer*,int err_code);
	int HttpHandler(void *arg);

};

#endif
