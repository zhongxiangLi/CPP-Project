#include "EventHandle.h"
#include "Reactor.h"

#define MAX_BUF_LEN 1024

void EventHandle::HandAccept(int fd,int events,void* arg){
	
	Reactor *reactor = (Reactor*)arg;
        if(NULL == reactor)
                return ;

        struct sockaddr_in sock_client;

        socklen_t len = sizeof(sock_client);

        int clientfd = accept(fd,(struct sockaddr*)&sock_client,&len);

        if(-1 == clientfd)
                return ;

        do{

			int flag = fcntl(clientfd,F_SETFL,O_NONBLOCK);
			if(flag <0)
					break;
			EventHandle *evHanlde =  new EventHandle(clientfd);

			if(NULL == evHanlde)
				return ;
			reactor->RegisterEventHandle(evHanlde,Event_Recv);
			

        }while(0);
	
}

void EventHandle::HandSend(int fd, int events, void *arg){
	
}

void EventHandle::HandRecv(int fd, int events, void *arg){

	Reactor *reactor = (Reactor*)arg;
	if(NULL == reactor)
		return ;

	EventHandle *ev = reactor->GetEventHandle(fd);
	if(ev == NULL)
		return ;

	char buff[MAX_BUF_LEN];
	memset(buff,0,MAX_BUF_LEN);
	int len = recv(fd,buff,MAX_BUF_LEN,0);

	if(len > 0 ){
		
		//cout <<"recv buf fd is:  "<<fd<<endl;	
		// recv  data  set other
	
	}else if(len ==0){
		close(fd);
		printf("close fd ...\n");
	
	}else{
		close(fd);
		
	}

	return ;
	
}