#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <stdint.h>
#include <pthread.h>

class CThreadNotify {

public:
	CThreadNotify();

	void Lock(); 
	void UnLock(); 
	void Wait(); 
	void init();
	void Signal() {
		pthread_cond_signal(&m_cond);
	}
	int WaitTime(int ms);

	~CThreadNotify();
	
private:

	pthread_mutex_t m_mutex;		//ª•≥‚À¯
	pthread_mutexattr_t m_mutexattr;//ª•≥‚À¯ Ù–‘
	pthread_cond_t m_cond;	//
};

#endif



