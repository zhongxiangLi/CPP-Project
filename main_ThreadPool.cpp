#include "Common.h"

#include "DBPool.h"
#include "DBConn.h"
#include "TableOPerator.h"
#include "ThreadPool.h"
#include <time.h>
#include <unistd.h>
#include "config.h"
using namespace std;
using namespace sqlinfo;
#define MAX_LEN 1024*32


int main(int argc,char *argv[])
{

	int thread_num = 1;
	int db_maxconn = 1;
	int use_pool = 1;
	int task_num = 100;

	if (argc != 5)
	{
		printf("usage ./test_ThreadPool thread_num db_maxconn use_pool task_num\n");
		return 1;
	}
	thread_num = atoi(argv[1]);
	db_maxconn = atoi(argv[2]);
	use_pool = atoi(argv[3]);
	task_num = atoi(argv[4]);
	
	printf("thread num %d dbmaxconn %d usepool %d tasknum %d",thread_num,db_maxconn,use_pool,task_num);	

	DBPool *pool = new DBPool(db_pool_name, db_host, db_port, db_username, db_password, db_dbname, db_maxconn);
	if (NULL == pool || pool->Init())
	{
		printf("pool init...");
		return 1;
	}
	/*获取连接之后 如果表存在 先删除 在创建*/
	DBConn *conn = pool->GetDBConn();
	if (conn)
	{
		bool ret = conn->ExecuteDrop(DROP_IMUSER_TABLE);
		if (ret)
		{
			printf("drop table ok\n");
		}
		ret = conn->ExecuteCreate(CREATE_IMUSER_TABLE);
		if (ret)
		{
			printf("create table ok\n");
		}
	}
	pool->RelConn(conn);

	printf("thread_num = %d\n",thread_num);


	Threadpool *pthreadpool = new Threadpool();

	if (!pthreadpool->Init(thread_num, task_num)) {
		
		printf("threadpool init fail\n");

		delete pthreadpool;
		return 0;
	}

	int i;

	for (i =0;i<task_num;i++)
	{
		if (use_pool)
		{
			pthreadpool->ThreadPool_AddJob(workUsePool, (void*)pool);
		}
		else
		{
		
		 	pthreadpool->ThreadPool_AddJob(WorkNoPool, (void*)&thread_num);
		}
		
	}
	sleep(thread_num+db_maxconn);

	pthreadpool->ThreadPool_Destroy();

	delete pthreadpool;
	delete pool;
	return 0;
}
