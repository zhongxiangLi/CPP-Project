#ifndef DBCONN_H
#define DBCONN_H


#include "Common.h"


#define MAX_ESCAPE_STRING_LEN 10240
class ResultSet;
class DBPool;
class DBConn {
public:
	DBConn(DBPool *);
	virtual ~DBConn();

	int Init();
	bool ExecuteCreate(const char*sql_query);
	bool ExecuteDrop(const char*sql_query);

	bool ExecuteUpdate(const char*sql_query,bool care_affected_rows = true);

	unsigned int  GetInsertId();

	bool StartTransaction();
	bool Commit();
	bool Rollback();
	const char *GetPoolName();

	MYSQL* GetMysql() { return m_mysql; }
	ResultSet* ExecuteQuery(const char*sql_query);
private:

	DBPool *m_pDbPool;
	MYSQL* m_mysql;
	char m_escape_string[MAX_ESCAPE_STRING_LEN + 1];
};
#endif

