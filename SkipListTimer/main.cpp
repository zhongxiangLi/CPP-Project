#include "skiplist.h"
#include <iostream>
#include <unistd.h>
using namespace std;


void expire_timer(SkipList *zsl) {
    SkipListNode *x;
    uint32_t now = getcurrent_time();
    for (;;) {
        x = zsl->GetMinNode();
        if (!x) break;
        if (x->GetScore() > now) break;
        
		cout <<"touch timer expire time " <<x->GetScore()<<"now "<<now<<endl;
        x->m_handle(x);
		zsl->Del_HeadNode();

    }
}

int main()
{

	SkipList *sl = new SkipList();
	if(NULL == sl)
	{
		cout <<"sl is NULL\n";
		return 0;
	}
	sl->Add_timer(10100,SkipList::Printf_hello);
	sl->Add_timer(21040,SkipList::Printf_hello);
	sl->Add_timer(32050,SkipList::Printf_hello);
	SkipListNode * node = sl->Add_timer(22050,SkipList::Printf_hello);
	sl->Del_timer(node);
	sl->Add_timer(32850,SkipList::Printf_hello);
	 for (;;) {
        expire_timer(sl);
        sleep(1);
    }
	
	return 0;
}
