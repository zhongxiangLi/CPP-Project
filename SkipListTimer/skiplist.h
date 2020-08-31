#ifndef SKIPLIST_H_
#define SKIPLIST_H_

#include "skiplistnode.h"
#include "common.h"

class SkipList{

public:
	SkipList();
	~SkipList();
	static void Printf_hello(SkipListNode* zn){
		cout <<"zn "<<zn->GetScore()<<endl;
	}
	
	SkipListNode *Add_timer(uint32_t mesc, handler_pt handler);
	void Del_timer(SkipListNode *snode);
	
	SkipListNode* GetMinNode();
public:
	SkipListNode *Insert_node(uint32_t mesc, handler_pt handler);
	void Del_node(SkipListNode *snode);
	void Del_HeadNode();
	int RandomLevel();
private:

	SkipListNode *m_header;
	int m_length;
	int m_level;	


};

#endif
