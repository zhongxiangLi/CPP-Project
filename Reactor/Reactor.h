#ifndef REACTOR_H
#define REACTOR_H
#include <map>
#include "EventDemultiplex.h"

using namespace std;

typedef map<int,EventHandle *> REVENT_MAP;

class Reactor{
public:
	Reactor(EventDemultiplex*);
	~Reactor();

	
	int  RegisterEventHandle(EventHandle* eventhandle,int event);
	void RemoveEventHandle(EventHandle* eventhandle,int event);
	int Run();

	EventHandle *GetEventHandle(int fd);
	
	void test_loginfo();
	
private:
   EventDemultiplex * m_Eventdemu;
   REVENT_MAP  ep_events;
};

#endif

