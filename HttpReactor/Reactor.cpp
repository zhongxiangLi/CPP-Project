#include"Reactor.h"

#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

#include<iostream>
using namespace std;


Reactor::Reactor(EventDemultiplex* demu):m_Eventdemu(demu){
	
   ep_events.clear();
   srand( (unsigned)time( NULL ) );
}


Reactor::~Reactor(){
	if(NULL !=m_Eventdemu )
		delete m_Eventdemu;
	
	REVENT_MAP::iterator it = ep_events.begin();
	for(;it != ep_events.end();it++){

		EventHandle* ev = it->second;
		delete ev;
	}
	
	ep_events.clear();
	
}

int  Reactor::RegisterEventHandle(EventHandle* eventhandle,int event){
	
	if(m_Eventdemu == NULL || eventhandle == NULL)
		return -1;
	if(eventhandle->GetFd() %1998 == 0)
	printf("register event handle begin fd is %d \n",eventhandle->GetFd());
	map<int,EventHandle*>::iterator it = ep_events.find(eventhandle->GetFd());
	if(it == ep_events.end())
	{
		ep_events.insert(make_pair(eventhandle->GetFd(),eventhandle));
	}
	

	return m_Eventdemu->regist(eventhandle,event);
}

void Reactor::RemoveEventHandle(EventHandle* event,int events){
	
	if(m_Eventdemu == NULL || NULL == event)
		return ;

	m_Eventdemu->remove(event);
	
	int fd = event->GetFd();

	map<int,EventHandle*>::iterator it = ep_events.find(fd);

	if(it == ep_events.end())
	{
		return ;
	}
	EventHandle* ev = it->second;

	ep_events.erase(it);

	delete ev;
	
}
int Reactor::Run(){

	if(m_Eventdemu == NULL )
		return -1;
	
	while(1){
		m_Eventdemu->wait_event(this);
	}
	
	return 0;
}
EventHandle *Reactor::GetEventHandle(int fd)
{

	map<int,EventHandle*>::iterator it = ep_events.find(fd);

	if(it != ep_events.end())
	{
		return it->second;
	}

	return NULL;
		
}

void Reactor::test_loginfo(){
	
	
	int n = rand()%1000;
	if(n == 50){
		printf("current connect is %d \n",ep_events.size());
		
	}
}
	
