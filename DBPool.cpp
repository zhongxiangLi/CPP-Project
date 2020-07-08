

#include "DBPool.h"
#include "DBConn.h"
#include "Common.h"
DBPool::DBPool()
{

}

DBPool::DBPool(const char* pname, const char* serverip, unsigned short derverport, const char* username, const char*passwd, const char*dbname, int max_conn)
{
	m_poolname = pname;
	m_db_serverip = serverip;
	m_db_serverport = derverport;
	m_username = username;
	m_passwd = passwd;
	m_db_name = dbname;

	m_db_cur_conn_cnt = 1;
	m_db_max_conn_cnt = max_conn;

	 m_free_list.clear();
	m_free_notify.init();

}

int DBPool::Init()
{
	int i = 0;
	for (i=0;i<1;i++)
	{
		DBConn *pDbConn = new DBConn(this);
		if (NULL == pDbConn)
		{
			continue;
		}
		int ret = pDbConn->Init();
		if (ret)
		{
			delete pDbConn;
			return ret;
		}
		m_free_list.push_back(pDbConn);
	//	m_db_cur_conn_cnt ++;
	
	}
	return 0;
}

DBConn * DBPool::GetDBConn()
{
	m_free_notify.Lock();
	while(m_free_list.empty())
	{
		if (m_db_cur_conn_cnt >= m_db_max_conn_cnt)
		{
			m_free_notify.Wait();
		}
		else
		{
			DBConn * conn = new DBConn(this);
			if (conn->Init() != 0)
			{
				delete conn;
				m_free_notify.UnLock();
				return NULL;
			}

			m_free_list.push_back(conn);
			m_db_cur_conn_cnt++;
			printf("conn %d\n",m_db_cur_conn_cnt);
		}
	}

	DBConn *pconn = m_free_list.front();

	m_free_list.pop_front();
	
	m_free_notify.UnLock();
	return pconn;
}

void DBPool::RelConn(DBConn* conn)
{
	m_free_notify.Lock();
	
	list<DBConn*>::iterator it = m_free_list.begin();
	for (;it != m_free_list.end();it ++)
	{
		if (*it == conn)
		{
			break;
		}
	}

	if (it == m_free_list.end())
	{
		m_free_list.push_back(conn);
	}
	
	m_free_notify.Signal();
	m_free_notify.UnLock();
}

DBPool::~DBPool()
{
	list<DBConn*>::iterator it = m_free_list.begin();
	
	for (; it != m_free_list.end(); it++) {

		DBConn* pconn = *it;
		delete pconn;
	}
	m_free_list.clear();
}
