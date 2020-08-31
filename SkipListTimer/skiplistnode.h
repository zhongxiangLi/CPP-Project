#ifndef SKIPLISTNODE_H_
#define SKIPLISTNODE_H_

#include "common.h"

class SkipListNode;

typedef void(*handler_pt) (SkipListNode *node);





class SkipListLevel{
public:
	SkipListLevel(){
		
		m_next = NULL;
	}
	~SkipListLevel(){
		m_next = NULL;
	}
	SkipListNode* GetNextNode(){return m_next;}
	
	void SetNextNode(SkipListNode* tmp){
		m_next = tmp;
	}
private:
	SkipListNode *m_next;	

};


class SkipListNode{

public:
	SkipListNode(int score,int level,handler_pt handler);
	~SkipListNode();
	
	unsigned long GetScore(){return m_score;}
	void SetScore(unsigned long score){
		m_score = score;
	}
	
	SkipListNode * GetNextNodeOfLevel(int level){
		return m_level[level]->GetNextNode();
	}
	
	void SetNextNodeOfLevel(int level,SkipListNode* tmp){
		m_level[level]->SetNextNode(tmp);
	}
	handler_pt m_handle;	
private:

	unsigned long m_score;
	
	SkipListLevel *m_level[SKIPLIST_MAXLEVEL];

};



#endif

