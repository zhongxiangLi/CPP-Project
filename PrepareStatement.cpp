#include "PrepareStatement.h"

PrepareStatement::PrepareStatement()
{
	m_stmt = NULL;
	m_param_bind = NULL;
	m_param_cnt = 0;
}

PrepareStatement::~PrepareStatement()
{
	if (m_stmt)
	{
		mysql_stmt_close(m_stmt);
		m_stmt = NULL;
	}
	if (m_param_bind)
	{
		delete[] m_param_bind;
		m_param_bind = NULL;
	}
}

bool PrepareStatement::Init(MYSQL* mysql, string& sql)
{
	mysql_ping(mysql);
	m_stmt = mysql_stmt_init(mysql);
	if (!m_stmt)
	{
		return false;
	}

	if (mysql_stmt_prepare(m_stmt, sql.c_str(), sql.size()))
	{
		return false;
	}
	m_param_cnt = mysql_stmt_param_count(m_stmt);
	if (m_param_cnt>0)
	{
		m_param_bind = new MYSQL_BIND[m_param_cnt];
		if (!m_param_bind)
		{
			return false;
		}
		memset(m_param_bind,0,sizeof(MYSQL_BIND)*m_param_cnt);
	}
	return true;

}

void PrepareStatement::setParam(uint32_t index, int &value)
{
	if (index > m_param_cnt)
	{
		return;
	}
	m_param_bind[index].buffer_type = MYSQL_TYPE_LONG;
	m_param_bind[index].buffer = &value;
}

void PrepareStatement::setParam(uint32_t index, uint32_t& value)
{
	if (index > m_param_cnt)
	{
		return;
	}
	m_param_bind[index].buffer_type = MYSQL_TYPE_LONG;
	m_param_bind[index].buffer = &value;
}

void PrepareStatement::setParam(uint32_t index, string& value)
{
	if (index > m_param_cnt)
	{
		return;
	}
	m_param_bind[index].buffer_type = MYSQL_TYPE_STRING;
	m_param_bind[index].buffer = (char*)value.c_str();
	m_param_bind[index].buffer_length = value.size();

}

void PrepareStatement::setParam(uint32_t index, const string & value)
{
	if (index > m_param_cnt)
	{
		return;
	}
	m_param_bind[index].buffer_type = MYSQL_TYPE_STRING;
	m_param_bind[index].buffer = (char*)value.c_str();
	m_param_bind[index].buffer_length = value.size();
}

void PrepareStatement::setParam(uint32_t index, uint8_t &value)
{
	if (index > m_param_cnt)
	{
		return;
	}
	m_param_bind[index].buffer_type = MYSQL_TYPE_STRING;
	m_param_bind[index].buffer = (char*)value;

}

bool PrepareStatement::ExecuteUpdate()
{
	if (!m_stmt)
	{
		return false;
	}
	if (mysql_stmt_bind_param(m_stmt,m_param_bind))
	{
		cout << "stmt bind error" << mysql_stmt_errno(m_stmt) << endl;
		return false;
	}
	if (int i =mysql_stmt_execute(m_stmt))
	{
		cout << "stmt execute error :"<<i<<" " << mysql_stmt_errno(m_stmt) << endl;
		return false;
	}
	if (mysql_stmt_affected_rows(m_stmt) == 0)
	{
		cout << "stmt affected error" << mysql_stmt_errno(m_stmt) << endl;
		return false;
	}
	return true;
}

uint32_t PrepareStatement::GetInsertId()
{
	return mysql_stmt_insert_id(m_stmt);
}

