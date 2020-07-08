#include "Common.h"

#include "DBPool.h"
#include "DBConn.h"
#include "TableOPerator.h"
#include <time.h>
#include "config.h"
using namespace std;
using namespace sqlinfo;
#define MAX_LEN 1024*32

void testOneConnect() {

	

	DBPool *pool = new DBPool(db_pool_name,db_host,db_port,db_username,db_password,db_dbname,db_maxconncnt);
	if (pool->Init())
	{
		return;
	}
	DBConn *conn = pool->GetDBConn();

	if (conn)
	{
		bool ret = conn->ExecuteDrop(DROP_IMUSER_TABLE);
		if (ret)
		{
			printf("drop table ok");
		}
		ret = conn->ExecuteCreate(CREATE_IMUSER_TABLE);
		if (ret )
		{
			printf("create table ok");
		}
		pool->RelConn(conn);
	}
	delete pool;




}
void testCURD() {
	DBPool *pool = new DBPool(db_pool_name, db_host, db_port, db_username, db_password, db_dbname, db_maxconncnt);
	if (pool->Init())
	{
		return;
	}
	DBConn *conn = pool->GetDBConn();
	if (conn)
	{
		bool ret = conn->ExecuteDrop(DROP_IMUSER_TABLE);
		if (ret)
		{
			printf("Drop Table ok \n");
		}
		ret = conn->ExecuteCreate(CREATE_IMUSER_TABLE);
		if (ret)
		{
			printf("create Table ok \n");
		}
		UserTable *table = new UserTable(conn);
		if (NULL == table)
		{
			return;
		}
		ret = table->insertUser();
		if (ret)
		{
			printf("Insert User Table ok \n");
		}
		ret = table->queryUser(1);
		if (ret)
		{
			printf("query User Table ok \n");
		}
		ret = table->updateInfo(1);
		if (ret)
		{
			printf("update User Table ok \n");
		}
		ret = table->queryUser(1);
		if (ret)
		{
			printf("query User Table ok \n");
		}
		delete table;
	}
}

int main()
{
	//testOneConnect();
	testCURD();
	return 0;
}
