
#ifndef  RESULT_H
#define  RESULT_H


#include<map>
#include<mysql.h>
using namespace std;
class ResultSet {

public:
	ResultSet(MYSQL_RES*);
	virtual ~ResultSet();

	bool Next();
	int GetInt(const char *key);
	char *GetString(const char* key);

private:
	int GetIndex(const char* key);

	MYSQL_RES * m_res;
	MYSQL_ROW m_row;

	map<string, int> m_key_map;
};


#endif 

