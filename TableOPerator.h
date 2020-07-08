#ifndef TABLEOPERATOR
#define TABLEOPERATOR
#include "Common.h"
#include "DBConn.h"
#include "PrepareStatement.h"
#include "ResultSet.h"
#include <time.h>
#include <sstream>
#include <string>
class Table {
public:
	Table(DBConn *con) { conn = con; }
	virtual ~Table() {}


	virtual bool insertUser() = 0;
	virtual bool updateInfo(uint32_t id =2) = 0;
	virtual bool queryUser(uint32_t id = 1) = 0;


	DBConn *GetConn() { return conn; }
private:
	DBConn *conn;

};

class UserTable :public Table
{
public:
	UserTable(DBConn *c) ;
	~UserTable() {}

	bool insertUser();
	bool updateInfo(uint32_t id = 2);
	bool queryUser(uint32_t id = 1);
private:
	uint32_t nId;
	uint8_t nSex;
	uint8_t nStatus;
	uint32_t nValidateMethod; 
	uint32_t nDeptId;
	string strNick;
	string strDomain;
	string strName;
	string strTel;
	string strEmail;
	string strAvatar;
	string sign_info;
	string strPass; 
	string strCompany; 
	string strAddress; 

};

UserTable::UserTable(DBConn *c):Table(c) 
{
	 nId =0;
	 nSex=1;
	 nStatus=1; 
	 nValidateMethod =0; 
	 nDeptId=0;
	 strNick="test";
	 strDomain = "test";
	 strName = "test";
	 strTel = "test";
	 strEmail = "test";
	 strAvatar = "test";
	 sign_info = "test";
	 strPass = "test"; 
	 strCompany = "test"; 
	 strAddress = "test"; 
}

bool UserTable::insertUser()
{
	bool bRet = false;
	string strSql;
	strSql = "insert into IMUser(`salt`,`sex`,`nick`,`password`,`domain`,`name`,`phone`,`email`,`company`,`address`,`avatar`,`sign_info`,`departId`,`status`,`created`,`updated`) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";

	PrepareStatement* stmt = new PrepareStatement();
	MYSQL *mysql = this->GetConn()->GetMysql();


	if (stmt->Init(mysql, strSql)) {
	
		uint32_t nNow = (uint32_t)time(NULL);
		uint32_t index = 0;
		string strOutPass = "987654321";
		string strSalt = "abcd";

	
		stmt->setParam(index++, strSalt);
		stmt->setParam(index++, this->nSex);
		stmt->setParam(index++, this->strNick);
		stmt->setParam(index++, strOutPass);
		stmt->setParam(index++, this->strDomain);
		stmt->setParam(index++, this->strName);
		stmt->setParam(index++, this->strTel);
		stmt->setParam(index++, this->strEmail);
		stmt->setParam(index++, this->strCompany);
		stmt->setParam(index++, this->strAddress);
		stmt->setParam(index++, this->strAvatar);
		stmt->setParam(index++, this->sign_info);
		stmt->setParam(index++, this->nDeptId);
		stmt->setParam(index++, this->nStatus);
		stmt->setParam(index++, nNow);
		stmt->setParam(index++, nNow);

		
		bRet = stmt->ExecuteUpdate();
		if (bRet)
		{
			uint32_t id = stmt->GetInsertId();
			if(id %300 == 0)
				printf("register then get User_id%d\n", id);
		}
	}
	delete stmt;
	return true;
}
static string int2string(uint32_t user_id)
{
	stringstream ss;
	ss << user_id;
	return ss.str();
}

bool UserTable::updateInfo(uint32_t id) {

	bool bRet = false;

	strDomain = "naihan";

	uint32_t nNow = (uint32_t)time(NULL);
	string strSql = "update IMUser set `sex`=" + int2string(nSex) + ", `nick`='" + strNick
		+ "', `domain`='" + strDomain + "', `name`='" + strName + "', `phone`='" + strTel
		+ "', `email`='" + strEmail + "', `avatar`='" + strAvatar + "', `sign_info`='" + sign_info
		+ "', `departId`='" + int2string(nDeptId) + "', `status`=" + int2string(nStatus) + ", `updated`=" + int2string(nNow)
		+ ", `company`='" + strCompany + "', `address`='" + strAddress + "' where id=" + int2string(id);

	bRet = GetConn()->ExecuteUpdate(strSql.c_str());
	if (!bRet)
	{
		printf("update user fail %s\n",strSql.c_str());
	}
	return bRet;
}

bool UserTable::queryUser(uint32_t id)
{

	string strsql = "select * from IMUser where id=" + int2string(id);

	ResultSet *pResultSet = GetConn()->ExecuteQuery(strsql.c_str());

	bool bRet = false;
	if (pResultSet)
	{
		if (pResultSet->Next())
		{
			nId = pResultSet->GetInt("id");
			nSex = pResultSet->GetInt("sex");
			strNick = pResultSet->GetString("nick");
			strDomain = pResultSet->GetString("domain");
			strName = pResultSet->GetString("name");
			strTel = pResultSet->GetString("phone");
			strEmail = pResultSet->GetString("email");
			strAvatar = pResultSet->GetString("avatar");
			sign_info = pResultSet->GetString("sign_info");
			nDeptId = pResultSet->GetInt("departId");
			nValidateMethod = pResultSet->GetInt("validateMethod");
			nStatus = pResultSet->GetInt("status");

			printf("nId:%u\n", nId);
			printf("nSex:%u\n", nSex);
			printf("strNick:%s\n", strNick.c_str());
			printf("strDomain:%s\n", strDomain.c_str());
			printf("strName:%s\n", strName.c_str());
			printf("strTel:%s\n", strTel.c_str());
			printf("strEmail:%s\n", strEmail.c_str());
			printf("sign_info:%s\n", sign_info.c_str());

			bRet = true;

		}
		delete pResultSet;
	}
	else {
		printf("no result for sql %s\n",strsql.c_str());
	}
	return bRet;
}

#endif 
