#ifndef EVENTDEMULTIPLEX_H
#define EVENTDEMULTIPLEX_H

#include "EventHandle.h"

class Reactor;
class EventDemultiplex{
public:	
	EventDemultiplex(){};
	virtual ~EventDemultiplex(){}
	
	virtual int regist(EventHandle* eventhandle,int event) = 0;
	virtual void remove(EventHandle* eventhandle) = 0;
	virtual  void wait_event(Reactor *r,int timeout = 0) = 0;
protected:

		
};
class EpollDemultiplex:public EventDemultiplex{
public:
	EpollDemultiplex();
	~EpollDemultiplex();
	virtual int regist(EventHandle* eventhandle,int event);
	virtual void remove(EventHandle* event);
	virtual  void wait_event(Reactor *r,int timeout = 0);
private:
	int m_epfd;
	int socketfd;
};



#endif
