#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>       /* close */
#include <netdb.h> 
 
#include <sys/epoll.h>
#include <pthread.h>


char *host_to_ip(const char * hostname){

	struct hostent * host_entry = gethostbyname(hostname);

	if(host_entry)
	 return  inet_ntoa(*(struct in_addr*)*host_entry->h_addr_list);

	return NULL;
}

int create_socket(const char *ip){


   int sockfd = socket(AF_INET,SOCK_STREAM,0);
  
   struct sockaddr_in sin;
  
  sin.sin_addr.s_addr = inet_addr(ip);
  sin.sin_port = htons(80);

  sin.sin_family = AF_INET;

 if(-1==connect(sockfd,(struct sockaddr*)&sin,sizeof(struct sockaddr_in))){

	return -1;
 }
 fcntl(sockfd,F_SETFL,O_NONBLOCK);

return sockfd;

}
char * send_request(int sockfd,const char * hostname,const char* resource){

	char buffer[1024] = {0};
	int len = 0;
	 len = sprintf(buffer,"GET %s HTTP/1.1\r\nHost:%s \r\nConnection: close \r\n\r\n\r\n",resource,hostname);

	printf("buffer is %d\n",buffer);

	send(sockfd,buffer,strlen(buffer),0);

	 struct timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        fd_set fdread;

        FD_ZERO(&fdread);
        FD_SET(sockfd,&fdread);


	char *result = malloc(sizeof(int));
	result[0] = '\0';
	
	while(1){
	
	    int selection = select(sockfd+1,&fdread,NULL,NULL,&tv);

	   if(!selection || !(FD_ISSET(sockfd,&fdread)))

		break;
          len = recv(sockfd,buffer,1024,0);
	  if(len ==0)
		break;
		
	  result = realloc(result,(strlen(result)+len+1)*sizeof(char));


	 strncat(result,buffer,len);


	}

	return result;	
}

int client_commit(const char *hostname,const char* resource){

	char *ip = host_to_ip(hostname);
	if(NULL == ip){

		printf("error host to ip is NULL \n");
		return -1;
	}
	
 	int fd = create_socket(ip);

	char *content = send_request(fd,hostname,resource);			
	if(content == NULL)
		printf("have no data\n");

	puts(content);

	close(fd);
	free(content);


	return 0;
}
struct http_request{

	char * hostname;
	char *resource;
};

struct http_request reqs[] = {

    {"api.seniverse.com", "/v3/weather/now.json?key=0pyd8z7jouficcil&location=beijing&language=zh-Hans&unit=c" },
    {"api.seniverse.com", "/v3/weather/now.json?key=0pyd8z7jouficcil&location=changsha&language=zh-Hans&unit=c" },
    {"api.seniverse.com", "/v3/weather/now.json?key=0pyd8z7jouficcil&location=shenzhen&language=zh-Hans&unit=c" },
    {"api.seniverse.com", "/v3/weather/now.json?key=0pyd8z7jouficcil&location=shanghai&language=zh-Hans&unit=c" },

};
int main(){


	int count = sizeof(reqs) /sizeof(reqs[0]);
	int i = 0;
#if 0
	for(i =0;i<count;i++){
	
		client_commit(reqs[i].hostname,reqs[i].resource);

	}
#else
	
	int fd = create_socket("172.16.236.185");

        char *content = send_request(fd,"172.16.236.185","hello");
        if(content == NULL)
                printf("have no data\n");

        puts(content);

        close(fd);
        free(content);
	
#endif
	return 0;
}
