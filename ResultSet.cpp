#include "ResultSet.h"
#include "DBPool.h"

#define log_err printf

ResultSet::ResultSet(MYSQL_RES* res)
{
	m_res = res;

	int num_fields = mysql_num_fields(m_res);
	MYSQL_FIELD *fileds = mysql_fetch_fields(m_res);
	int i;
	for ( i =0;i< num_fields;i++)
	{
		m_key_map.insert(make_pair(fileds[i].name, i));
	}
}

ResultSet::~ResultSet()
{
	if (m_res)
	{
		mysql_free_result(m_res);
		m_res = NULL;
	}
}

bool ResultSet::Next()
{
	m_row = mysql_fetch_row(m_res);
	if (m_row)
	{
		return true;
	}
	return false;
}

int ResultSet::GetInt(const char *key)
{
	int idx = GetIndex(key);
	if (idx != -1)
	{
		return atoi(m_row[idx]);
	}
	return -1;
}

char * ResultSet::GetString(const char* key)
{
	int idx = GetIndex(key);
	if (idx != -1)
	{
		return m_row[idx];
	}
	return NULL;
}

int ResultSet::GetIndex(const char* key)
{
	map<string, int>::iterator it = m_key_map.find(key);
	if (it != m_key_map.end())
	{
		return it->second;
	}
	return -1;
}

