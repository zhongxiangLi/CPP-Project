#include <sys/mman.h>
#include <sys/stat.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <stdlib.h>

#include "HttpServer.h"

HttpServer::HttpServer(){

}

HttpServer::~HttpServer(){


}
int HttpServer::BadRequest(const char * path,const char *head, char* sbuffer){

	struct stat st;
	if(stat(path,&st) <0){
		return -1;

	}

	int length = 0;

	const char * status_line = head;
	//状态行 HTTP/1.0 404 Not Found\r\n	
	strcpy(sbuffer,status_line);

	length = strlen(status_line);
	//消息报头  Content-Type: text/html;charset=utf-8\r\n Content-Length:XXXX \r\n
	const char* content_type = "Content-Type: text/html;charset=utf-8\r\n";

	strcat(sbuffer+length,content_type);

	length += strlen(content_type);

	//Content-Length 
	length += sprintf(sbuffer+length,"Content-Length: %ld\r\n",st.st_size());

	//空行 \r\n
	strcat(sbuffer+length,"\r\n");
	
	length += 2;

	
	int fd = open(path,O_RDONLY);
	//mmap将文件从磁盘映射到内存，把fd从0处开始总共st.st_size那么大的文件以prot_read形式 MAP_SHARED的方式映射到内存中，返回内存的首地址
	char *fp = (char*)mmap(NULL,st.st_size,PORT_READ,MAP_SHARED,fd,0);
	//相应正文
	strcpy(buffer+length,fp);
	
	length +=st.st_size;

	close(fd);
	return length;

}
int HttpServer::SendError(char buffer*,int err_code){

	switch(err_code){

	case 404:{

		return BadRequest("www/404.html","HTTP/1.0 404 Not Found\r\n",buffer);
	}
	case 503:{

		return BadRequest("www/404.html","HTTP/1.0 503 Server Unavailable\r\n",buffer);
	}


	}

	return 0;

}
