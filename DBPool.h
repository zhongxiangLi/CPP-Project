
#ifndef DBPOOL_H
#define DBPOOL_H
#include "ThreadNotify.h"
#include "Common.h"


class DBConn;
class DBPool {

public:

	DBPool();
	DBPool(const char* pname, const char* serverip, unsigned short derverport, 
	const char* username, const char*passwd, const char*dbname, int max_conn);

	int Init();
	DBConn *GetDBConn();
	void RelConn(DBConn*);

	~DBPool();

	const char* GetPoolName() { return m_poolname.c_str(); }
	const char* GetDBServerHostIp() { return m_db_serverip.c_str(); }
	unsigned short GetDBServerPort() { return m_db_serverport; }

	const char* GetUserName() { return m_username.c_str(); }
	const char*GetUSerPasswd() { return m_passwd.c_str(); }
	const char*GetDBName() { return m_db_name.c_str(); }

	int GetCurConnCnt() { return m_db_cur_conn_cnt; }
	int GetMaxConnCnt() { return m_db_max_conn_cnt; }

private:
	
	string	m_poolname;
	string  m_db_serverip;
	unsigned short m_db_serverport;
	string m_username;
	string m_passwd;
	string m_db_name;

	int m_db_cur_conn_cnt;
	int m_db_max_conn_cnt;

	list<DBConn*> m_free_list;
	CThreadNotify m_free_notify;
};


#endif
