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

#define HTTP_VERSION    "HTTP/1.1"
#define USER_AGENT      "User-Agent: Mozilla/5.0 (Windows NT 5.1; rv:10.0.2) Gecko/20100101 Firefox/10.0.2\r\n"
#define ENCODE_TYPE     "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
#define CONNECTION_TYPE "Connection: close\r\n"

typedef void (*async_result_cb)(const char*hostname,const char*resource);
struct async_context{

 	pthread_t thread_id;
	int epfd;
};
struct ep_arg {

  int sockfd;
  char hostname[1024];
  async_result_cb cb;
};
struct http_request {
    char *hostname;
    char *resource;
};
 
#define BUFFER_SIZE     4096

static void *thread_client_callback(void *arg){
	struct async_context *ctx = (struct async_context*)arg;

	int epfd = ctx->epfd;
	while (1) {
 
        struct epoll_event events[1024] = {0};
 
        int nready = epoll_wait(epfd, events,1024, -1);
        if (nready < 0) {
            if (errno == EINTR || errno == EAGAIN) {
                continue;
            } else {
                break;
            }
        } else if (nready == 0) {
            continue;
        }
 
        printf("nready:%d\n", nready);
        int i = 0;
        for (i = 0;i < nready;i ++) {
 
            struct ep_arg *data = (struct ep_arg*)events[i].data.ptr;
            int sockfd = data->sockfd;
             
            char buffer[BUFFER_SIZE] = {0};
            struct sockaddr_in addr;
            size_t addr_len = sizeof(struct sockaddr_in);
            int n = recv(sockfd, buffer, BUFFER_SIZE, 0);
 
            data->cb(data->hostname, buffer); //call cb
             
            int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
           
             
                    close(sockfd); /////
                          
                    free(data);
                                       
                 }
                                                  
             }

}
//
typedef void (*async_result_cb)(const char*hostname,const char*resource);

static void http_result_callback(const char*hostname,const char*resource){


}
char *host_to_ip(const char *hostname) {
 
    struct hostent *host_entry = gethostbyname(hostname);
    if (host_entry) {
        return inet_ntoa(*(struct in_addr*)*host_entry->h_addr_list);
    } else {
        return NULL;
    }
}

int async_http_client_request(struct async_context *ctx,const char* hostname,const char* source,async_result_cb cb){

	 char *ip = host_to_ip(hostname);
    if (ip == NULL) return -1;
     
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
 
    struct sockaddr_in sin = {0};
    sin.sin_addr.s_addr = inet_addr(ip);
    sin.sin_port = htons(80);
    sin.sin_family = AF_INET;
 
    if (-1 == connect(sockfd, (struct sockaddr*)&sin, sizeof(struct sockaddr_in))) {
        return -1;
    }
 
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
 
    char buffer[BUFFER_SIZE] = {0};
     
    int len = sprintf(buffer,"GET %s HTTP/1.1\r\nHost:%s \r\nConnection: close \r\n\r\n\r\n",source,hostname); 
    printf("request buffer\n");
    int slen = send(sockfd, buffer, strlen(buffer), 0);
   
   struct ep_arg* eparg = (struct ep_arg*)malloc(sizeof(struct ep_arg));
   if(eparg == NULL)
	return -1;
  eparg->sockfd = sockfd;
  eparg->cb = cb;

   struct epoll_event ev;
  ev.data.ptr = eparg;
  ev.events = EPOLLIN; 

   int ret = epoll_ctl(ctx->epfd,EPOLL_CTL_ADD,sockfd,&ev); 
   return ret;
}
//
struct async_context *async_client_init(void){

	int epfd = epoll_create(1);
	if(epfd < 0)
		return NULL;

	struct async_context* cnt = (struct async_context*)malloc(sizeof(struct async_context));
	if(!cnt)
		return NULL;
	cnt->epfd = epfd;

	int ret = pthread_create(&cnt->thread_id,NULL,thread_client_callback,cnt);
	if(ret){
		return NULL;
	}
	usleep(1);


	return cnt;
}

void async_client_uninit(struct async_context * ctx){

	if(ctx){
		close(ctx->epfd);
		pthread_cancel(ctx->thread_id);
	}
	
}
struct http_request reqs[] = {
    {"api.seniverse.com", "/v3/weather/now.json?key=0pyd8z7jouficcil&location=beijing&language=zh-Hans&unit=c" },
    {"api.seniverse.com", "/v3/weather/now.json?key=0pyd8z7jouficcil&location=changsha&language=zh-Hans&unit=c" },
    {"api.seniverse.com", "/v3/weather/now.json?key=0pyd8z7jouficcil&location=shenzhen&language=zh-Hans&unit=c" },
    {"api.seniverse.com", "/v3/weather/now.json?key=0pyd8z7jouficcil&location=shanghai&language=zh-Hans&unit=c" },
    {"api.seniverse.com", "/v3/weather/now.json?key=0pyd8z7jouficcil&location=tianjin&language=zh-Hans&unit=c" },
    {"api.seniverse.com", "/v3/weather/now.json?key=0pyd8z7jouficcil&location=wuhan&language=zh-Hans&unit=c" },
    {"api.seniverse.com", "/v3/weather/now.json?key=0pyd8z7jouficcil&location=hefei&language=zh-Hans&unit=c" },
    {"api.seniverse.com", "/v3/weather/now.json?key=0pyd8z7jouficcil&location=hangzhou&language=zh-Hans&unit=c" },
    {"api.seniverse.com", "/v3/weather/now.json?key=0pyd8z7jouficcil&location=nanjing&language=zh-Hans&unit=c" },
    {"api.seniverse.com", "/v3/weather/now.json?key=0pyd8z7jouficcil&location=jinan&language=zh-Hans&unit=c" },
    {"api.seniverse.com", "/v3/weather/now.json?key=0pyd8z7jouficcil&location=taiyuan&language=zh-Hans&unit=c" },
    {"api.seniverse.com", "/v3/weather/now.json?key=0pyd8z7jouficcil&location=wuxi&language=zh-Hans&unit=c" },
    {"api.seniverse.com", "/v3/weather/now.json?key=0pyd8z7jouficcil&location=suzhou&language=zh-Hans&unit=c" },
};
 
 
static void http_async_client_result_callback(const char *hostname, const char *result) {
     
    printf("hostname: result%s \n\n\n\n",result);

}
 
 
 
int main(int argc, char *argv[]) {
 
 
    struct async_context *ctx = async_client_init();
    if (ctx == NULL) return -1;
 
    int count = sizeof(reqs) / sizeof(reqs[0]);
    int i = 0;
    for (i = 0;i < count;i ++) {
        async_http_client_request(ctx, reqs[i].hostname, reqs[i].resource, http_async_client_result_callback);
    }
 
 
    getchar();
 
 
}

