#ifndef THREADPOOL_H 
#define THREADPOOL_H
#include "Common.h"
#include <pthread.h>
#include "config.h"
using namespace sqlinfo;

struct job {
	void *(*callback_function)(void *arg);
	void *arg;
	struct job *next;

};
static uint64_t get_tick_count() {
	struct timeval tval;
	uint64_t ret_tick;

	gettimeofday(&tval, NULL);

	ret_tick = tval.tv_sec * 1000L + tval.tv_usec / 1000L;
	return ret_tick;

}
static void *threadpoll_function(void *arg);


class Threadpool {

public:
	Threadpool() {}
	
		
	bool Init(int thread_num, int queue_max_num) {
		m_threadnum = thread_num;
		m_QueueMaxNum = queue_max_num;

		m_CurQueueJobNum = 0;

		m_PoolIsClose = false;
		m_head = m_tail = NULL;
		
		if (pthread_mutex_init(&m_mutex, NULL))
		{
			return false;
		}
		if (pthread_cond_init(&m_cond_QueueEmpty,NULL) || pthread_cond_init(&m_cond_QueueNotEmpty, NULL) || pthread_cond_init(&m_cond_QueueNotFull, NULL))
		{
			return false;
		}

		int i = 0;
		m_pthreads = (pthread_t*)malloc(sizeof(pthread_t)*m_threadnum);
		if(NULL == m_pthreads)
		{
			printf("malloc pthread fail\n");
			return false;
		}	

		for (i = 0; i < m_threadnum; i++) {
			
			pthread_create(&m_pthreads[i], NULL, threadpoll_function, (void*)this);
			
		}
		return true;
	}
	

	int ThreadPool_AddJob(void *(*callback_function)(void*arg),void *arg) {


		pthread_mutex_lock(&m_mutex);

		while ((m_CurQueueJobNum == m_QueueMaxNum) &&!(m_PoolIsClose) )
		{
			pthread_cond_wait(&m_cond_QueueNotFull, &m_mutex);
		}
		
		if ( m_PoolIsClose)
		{
			pthread_mutex_unlock(&m_mutex);
			return -1;
		}
		
		struct job *pjob = (struct job*)malloc(sizeof(struct job));

		if (!pjob)
		{
			pthread_mutex_unlock(&m_mutex);
			return -1;
		}
		
		pjob->callback_function = callback_function;
		pjob->arg = arg;
		pjob->next = NULL;
		
		if (m_head == NULL)
		{
			m_head = m_tail = pjob;
			pthread_cond_broadcast(&m_cond_QueueNotEmpty);
		}
		else
		{
			m_tail->next = pjob;
			m_tail = pjob;
		}
		m_CurQueueJobNum++;
		pthread_mutex_unlock(&m_mutex);
		return 0;
	}

	int ThreadPool_Destroy() {

		pthread_mutex_lock(&m_mutex);

		if (m_PoolIsClose)
		{
			pthread_mutex_unlock(&m_mutex);
			return 0;
		}
		printf("1 queue num = %d\n",m_CurQueueJobNum);

		while (m_CurQueueJobNum != 0)
		{
			pthread_cond_wait(&m_cond_QueueEmpty, &m_mutex);
		}
		printf(" 2queue num = %d\n",m_CurQueueJobNum);	
		m_PoolIsClose = true;
		pthread_mutex_unlock(&m_mutex);
		
		
		pthread_cond_broadcast(&m_cond_QueueNotEmpty);
		pthread_cond_broadcast(&m_cond_QueueNotFull);

		int  i ;
		for (i=0;i< m_threadnum;i++)
		{
			pthread_join(m_pthreads[i],NULL);
		}
		pthread_mutex_destroy(&m_mutex);
		pthread_cond_destroy(&m_cond_QueueEmpty);
		pthread_cond_destroy(&m_cond_QueueNotEmpty);
		pthread_cond_destroy(&m_cond_QueueNotFull);
		free(m_pthreads);

		struct job *mj;
		while (m_head != NULL) {
		
			mj = m_head;
			m_head = m_head->next;
			free(mj);
		}
		
		return 0;
	}
public:
	int m_threadnum;                    //线程池中开启线程的个数
	int m_QueueMaxNum;                //队列中最大job的个数
	struct job *m_head;                 //指向job的头指针
	struct job *m_tail;                 //指向job的尾指针
	pthread_t  *m_pthreads;       		 //线程池中所有线程的pthread_t
	pthread_mutex_t m_mutex;            //互斥信号量
	pthread_cond_t m_cond_QueueEmpty;       //队列为空的条件变量
	pthread_cond_t m_cond_QueueNotEmpty;   //队列不为空的条件变量
	pthread_cond_t m_cond_QueueNotFull;    //队列不为满的条件变量
	int m_CurQueueJobNum;                //队列当前的job个数

	bool m_PoolIsClose;                   //线程池是否已经关闭


};

static void *threadpoll_function(void *arg) {
	
	
	Threadpool* pool = (Threadpool*)arg;
	struct job *pjob = NULL;
	if (NULL == pool)
	{
		return NULL;
	}

	uint64_t start_time = get_tick_count();
	uint64_t end_time = get_tick_count();
	while (1) {

		pthread_mutex_lock(&(pool->m_mutex));

		while ((pool->m_CurQueueJobNum == 0) && !(pool->m_PoolIsClose)) {

			
			end_time = get_tick_count();
			pthread_cond_wait(&(pool->m_cond_QueueNotEmpty), &(pool->m_mutex));//wait new job  into queue or pool is close
		}
		
		if (pool->m_PoolIsClose) {

			printf("threadpool need time %lums\n",end_time - start_time);
			pthread_mutex_unlock(&(pool->m_mutex));
			pthread_exit(NULL);
		}

		pool->m_CurQueueJobNum--;
		
		pjob = pool->m_head;
		if (pool->m_CurQueueJobNum == 0)
		{
			pool->m_head = pool->m_tail = NULL;
		}
		else
		{
			pool->m_head = pjob->next;
		}
		if (pool->m_CurQueueJobNum == 0)
		{
			end_time = get_tick_count();
			pthread_cond_signal(&(pool->m_cond_QueueEmpty));
			printf("signal num is %d\n",pool->m_CurQueueJobNum);
		}
		
		if (pool->m_CurQueueJobNum == pool->m_QueueMaxNum - 1)
		{
			pthread_cond_broadcast(&(pool->m_cond_QueueNotFull));
		}
		pthread_mutex_unlock(&(pool->m_mutex));

		(*(pjob->callback_function))(pjob->arg);

		free(pjob);
		pjob = NULL;
	}

}


static void *workUsePool(void *arg) {

	DBPool *pdbpool = (DBPool*)arg;
	if(pdbpool == NULL)
		return NULL;
	DBConn *pDbConn = pdbpool->GetDBConn();

	if (pDbConn)
	{
		UserTable *table = new UserTable(pDbConn);
		if (NULL == table)
		{
			return NULL;
		}
		bool ret = table->insertUser();
		if (!ret)
		{
			printf("insert User fail\n");
		}
		delete table;
	}
	else
	{
		printf("GetConn failed\n");
	}
	
	pdbpool->RelConn(pDbConn);
	return NULL;

}

static void*WorkNoPool(void *) {

	//int a = *(int*)connnum;
	DBPool *pdbpool = new DBPool(db_pool_name, db_host, db_port, db_username, db_password, db_dbname, 1);
	if (pdbpool->Init())
	{
		printf("pool init fail\n");
		return NULL;
	}
	DBConn *pDbConn = pdbpool->GetDBConn();
	if (pDbConn)
	{
		UserTable *table = new UserTable(pDbConn);
		if (NULL == table)
		{
			return NULL;
		}
		bool ret = table->insertUser();
		if (!ret)
		{
			printf("insert User fail\n");
		}
		delete table;
	}
	else
	{
		printf("GetConn failed\n");
	}
	
	pdbpool->RelConn(pDbConn);

	delete pdbpool;
	return NULL;
}
#endif // THREADPOOL_H 
