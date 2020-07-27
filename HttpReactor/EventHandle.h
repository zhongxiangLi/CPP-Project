#ifndef EVENTHANDLE_H
#define EVENTHANDLE_H
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <memory.h>
#include <arpa/inet.h>
#include<iostream>
using namespace std;

enum EventMask{
	
	Event_Accept = 	0X01,
	Event_Recv = 	0X02,
	Event_Send = 	0X04,
	
};

#define MAX_BUFFER 1024

class EventHandle{
public:	
	EventHandle(){}
	EventHandle(int _fd){
	
	  memset(sbuffer,0,sizeof(char)*MAX_BUFFER);
	  len = 0;
	  fd = _fd;
          status=0;

	};
	~EventHandle(){};
	
	 void HandAccept(int fd,int events,void* arg);
	 void HandSend(int fd, int events, void *arg);
	 void HandRecv(int fd, int events, void *arg);
	
	int GetFd()const{return fd;}
	
	int GetStatus(){return status;}
	void SetStatus(int s ){status = s;}
private:	
	int fd;
	int status;
	
	char* sbuffer[MAX_BUFFER];
	int len;

};




#endif
