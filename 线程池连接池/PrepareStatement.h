#ifndef PREPARESTATEMENT_H
#define PREPARESTATEMENT_H
#include "Common.h"

class PrepareStatement {

public:
	PrepareStatement();
	virtual ~PrepareStatement();


	bool Init(MYSQL* mysql, string& sql);

	void setParam(uint32_t index,int &value);
	void setParam(uint32_t index, uint8_t &value);
	void setParam(uint32_t index, uint32_t& value);
	void setParam(uint32_t index, string& value);
	void setParam(uint32_t index, const string & value);

	bool ExecuteUpdate();
	uint32_t GetInsertId();
private:
	MYSQL_STMT*		m_stmt;
	MYSQL_BIND *	m_param_bind;
	uint32_t		m_param_cnt;
};
#endif
