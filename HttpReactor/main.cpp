#include "Reactor.h"
#include "EventHandle.h"
#include "EventDemultiplex.h"

#include <sys/socket.h>


#include <fcntl.h>
#include <unistd.h>
#include <error.h>
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#define SERVER_PORT 7710
#define SERVER_NUM 1


int main(int argc,char *argv[]){


	unsigned short port = SERVER_PORT;

	if(argc ==2)
		port  = atoi(argv[1]);

	EventDemultiplex* epollde = new EpollDemultiplex();
	
	Reactor *m_reactor = new Reactor(epollde);

	if(NULL == m_reactor)
		return 0;


	int i =0;

	int lisfd = socket(AF_INET,SOCK_STREAM,0);

	fcntl(lisfd,F_SETFL,O_NONBLOCK);

	struct sockaddr_in sock;
	memset(&sock,0,sizeof(sock));

	sock.sin_addr.s_addr =htonl(INADDR_ANY);
	sock.sin_family = AF_INET;
	sock.sin_port = htons(port+i); 

	bind(lisfd,(struct sockaddr*)&sock,sizeof(sock));

	if(listen(lisfd,20)<0){

		cout <<"listem error"<<endl;

	}

	printf(" listen port %d \n",port+i);
	EventHandle *evHanlde =  new EventHandle(lisfd);
	if(NULL == evHanlde){
		printf("even Handle error \n");
		return 0;
	}
		
	//evHanlde->SetCallback(EventHandle::HandAccept_cb);
	
	m_reactor->RegisterEventHandle(evHanlde,Event_Accept);
	
	m_reactor->Run();


	close(lisfd);

	delete m_reactor;

	return 0;
}
