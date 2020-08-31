#include "skiplist.h"
#include "common.h"

SkipList::SkipList(){

	m_level = 1;
	m_length = 0;
	m_header = new SkipListNode(0,SKIPLIST_MAXLEVEL,SkipList::Printf_hello);

}
SkipListNode *SkipList::Add_timer(uint32_t mesc, handler_pt handler)
{

	mesc += getcurrent_time();
	
	cout <<"add time expire at mesc = "<<mesc<<endl;
	
	return Insert_node(mesc,handler);

}
int SkipList::RandomLevel(){
	
	srand(time(0));
	int level = 1;
	while( (random()&0xFFFF) < (ZSKIPLIST_P *0xFFFF))
		level +=1;
	return (level<SKIPLIST_MAXLEVEL)?level:SKIPLIST_MAXLEVEL;
}

//插入节点 在每一level进行遍历找到比插入节点稍微小的那个节点 temp[level]进行保存
//随机新的level ，如果比现有的m_level 大，则新temp指向头节点
//创建新节点，按照level 将节点插入到相关位置  链表长度+1
SkipListNode *SkipList::Insert_node(uint32_t score, handler_pt handler)
{
	SkipListNode *temp[SKIPLIST_MAXLEVEL];
	
	int i,newlevel;
	
	SkipListNode *x = m_header;
	
	for( i = m_level-1;i>=0;i--){
		
		while(x->GetNextNodeOfLevel(i) && x->GetNextNodeOfLevel(i)->GetScore() < score){
			
			x = x->GetNextNodeOfLevel(i);
		}
		temp[i] = x;
		
	}
	
	newlevel = RandomLevel();
	cout  <<"node add  newlevel is "<<newlevel<<endl;
	if(newlevel > m_level ){
		
		for(i = m_level;i< newlevel ;i++)
		{
			temp[i] = m_header;
		}
		 m_level = newlevel;
	}
	
	x = new SkipListNode(score,newlevel,handler);
	
	for(i =0;i< newlevel;i++){
		x->SetNextNodeOfLevel(i,temp[i]->GetNextNodeOfLevel(i));
		temp[i]->SetNextNodeOfLevel(i,x);
	}
	 
	 m_length ++;
	 return x;
}
/*
	删除节点
	对于每一层，遍历找到 比删除节点 稍微小的那个节点 temp[level]保存节点信息
	
	获取当前节点的下一个节点（理论上是要删除的节点），level= 0时确定是要删除的节点，>0时 则 可能不存在这个节点
		
	删除节点（前一个节点next指针指向删除节点的下一个节点节） 释放 删除节点的 内存
		
*/
void SkipList::Del_node(SkipListNode *snode){
	
	SkipListNode * x = m_header;
	SkipListNode *temp[SKIPLIST_MAXLEVEL];
	
	int i = 0;
	
	for(i = m_level-1;i>=0;i--){
		
		while(x->GetNextNodeOfLevel(i) && x->GetNextNodeOfLevel(i)->GetScore() < snode->GetScore()){
				x = x->GetNextNodeOfLevel(i);
		}
		temp[i] = x;	
	}
	//level = 0 存放所有数据，理论上 x->level[0].forward 这个节点是被删除的节点。
	
	SkipListNode *delNode = x->GetNextNodeOfLevel(0);
	if(delNode && delNode->GetScore() == snode->GetScore()){
		
		
		for(i =0 ;i< m_level;i++){
			
			if(temp[i]->GetNextNodeOfLevel(i) == delNode){
				temp[i]->SetNextNodeOfLevel(i,delNode->GetNextNodeOfLevel(i));
				
			}
		}
		
		while(m_level > 1 && m_header->GetNextNodeOfLevel(m_level-1) == NULL)
			m_level --;
		
		m_length --;
		
		delete delNode;
		
	}
}
void SkipList::Del_timer(SkipListNode *snode){
	
	Del_node(snode);
	
}
void SkipList::Del_HeadNode()
{
	SkipListNode* header = GetMinNode();
	if(NULL == header)
		return ;
	Del_node(header);
	/*
	int i;
	for(i = m_level -1;i >=0;i--){
		
		if(m_header->level[i].m_next == header)
			m_header->level[i].m_next = header->level[i].m_next;
		
	}
	while(m_level > 1 && m_hreade->m_level[m_level-1].m_next == NULL)
		m_level --;
	
	m_length --;*/
}
SkipListNode* SkipList::GetMinNode()
{
	
	return m_header->GetNextNodeOfLevel(0);
}
SkipList::~SkipList(){

	if(NULL != m_header){
		
		delete m_header;
		m_header = NULL;
	}

}
