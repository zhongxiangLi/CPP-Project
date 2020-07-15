#include "EventDemultiplex.h"
#include "EventHandle.h"
#include "Reactor.h"
#include<sys/epoll.h>

EpollDemultiplex::EpollDemultiplex(){
	m_epfd = epoll_create(1);
	socketfd = 0;
}

EpollDemultiplex::~EpollDemultiplex(){
	close(m_epfd);
}

int EpollDemultiplex::regist(EventHandle* event,int events){
	
	if(NULL == event)
		return -1;
	

	struct epoll_event ep_ev={0,{0}};

	int op;
	ep_ev.data.ptr = event;
	ep_ev.data.fd = event->GetFd();
	
	if(events & Event_Accept){
		ep_ev.events |= EPOLLIN;
		socketfd = event->GetFd();
		printf("listen socketfd is %d\n",socketfd);
	}
	
	if(events & Event_Recv)
		ep_ev.events |= EPOLLIN;

	if(event->GetStatus() == 1){
			op = EPOLL_CTL_MOD;
	}else
	{
			op = EPOLL_CTL_ADD;
			event->SetStatus(1);
	}

	if(epoll_ctl(m_epfd,op,event->GetFd(),&ep_ev) <0){

		printf("event add fail %d,ep %d \n",event->GetFd(),op);
		return -1;
	}

	return 0;
}

void EpollDemultiplex::remove(EventHandle* event){
	if(NULL == event)
		return ;
	
	if(event->GetStatus() != 1)
		return ;
	event->SetStatus(0);
	
	struct epoll_event ep_ev={0,{0}};
	ep_ev.data.ptr = event;

	
	if(epoll_ctl(m_epfd,EPOLL_CTL_DEL,event->GetFd(),&ep_ev) <0){

		printf("event del fail %d\n",event->GetFd());
		return ;
	}
	
	return ;
}
#define MAX_EPOLL_ITEM 10240

 void EpollDemultiplex::wait_event(Reactor *r,int timeout){
	 
	 struct epoll_event events[MAX_EPOLL_ITEM];

	int checkpos = 0,i;


	int ready = epoll_wait(m_epfd,events,MAX_EPOLL_ITEM,1000);

	for(i = 0;i<ready;i++){

		EventHandle *event = r->GetEventHandle(events[i].data.fd);
		
		if(event == NULL)
			continue;
		
		if(events[i].events & EPOLLIN){

			if(socketfd == events[i].data.fd){	
				
				event->HandAccept(event->GetFd(),events[i].events,(void*)r);
				r->test_loginfo();

			}
			else{
				event->HandRecv(event->GetFd(),events[i].events,(void*)r);
			}

		}			
		if(events[i].events & EPOLLOUT){
			   event->HandSend(event->GetFd(),events[i].events,(void*)r);
		}	
	}
		
	 
	 
	 
 }