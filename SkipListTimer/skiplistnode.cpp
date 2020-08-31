#include "skiplistnode.h"

static void defaultHandler(SkipListNode *node){
	
	
}
SkipListNode::SkipListNode(int score,int level,handler_pt handler){

	m_score = score;
	m_handle = 	handler;
		
	int i = 0;
	for(i=0;i<level;i++){
		
		m_level[i] = new SkipListLevel();
	}
}


SkipListNode::~SkipListNode(){

	int i ;
	for(i=0;i<SKIPLIST_MAXLEVEL;i++){
		if(NULL != m_level[i]){
			delete m_level[i];
			m_level[i] = NULL;
		}
	}

}
