

#include <iostream>
#include <stdio.h>
#include <stdlib.h>


enum { _ALIGN = 8 };  // 小型区块的上调边界
enum { _MAX_BYTES = 128 }; // 小区区块的上限
enum { _NFREELISTS = 16 }; // _MAX_BYTES/_ALIGN  free-list 的个数


static char* _S_start_free;  // 内存池起始位置。只在 _S_chunk_alloc() 中变化
static char* _S_end_free;    // 内存池结束位置。只在 _S_chunk_alloc() 中变化
static size_t _S_heap_size;

union _Obj {
	union _Obj* _M_free_list_link;  // 利用联合体特点
	char _M_client_data[1]; 
};

//空闲数组
static _Obj*  _S_free_list[_NFREELISTS];

//获取比此数大且是8的倍数的最小数，内存池存放内存按照8的倍数为一个块
// 比如申请7 byte 放在块为8的链表中，申请17byte 放在块为24的链表中
static size_t _S_round_up(size_t __bytes)
{
	return  (((__bytes)+(size_t)_ALIGN - 1) & ~((size_t)_ALIGN - 1));

}


//获取 申请对应大小字节 对应的 _S_free_list 链表下标
//_S_free_list 是一个大小为16的数组，每一个下标按照8的倍数存放相应的块
//下标为0 存放 8 byte的块链表；下标为1存放16 byte块大小.....
static  size_t _S_freelist_index(size_t __bytes) {

	return (((__bytes)+(size_t)_ALIGN - 1) / (size_t)_ALIGN - 1);
}

//此函数申请内存使用，调用后返回size的地址 和
/*
	_S_start_free 表示申请内存的起始地址
	_S_end_free   表示申请的内存的结束地址
	__result 使用的内存起始地址

*/
char*_S_chunk_alloc(size_t __size,int& __nobjs)
{
	char* __result;
	size_t __total_bytes = __size * __nobjs;  // 需要申请空间的大小 
	size_t __bytes_left = _S_end_free - _S_start_free;  // 计算内存池剩余空间

	if (__bytes_left >= __total_bytes) {  // 内存池剩余空间完全满足申请
		__result = _S_start_free;
		_S_start_free += __total_bytes;
		return(__result);
	}
	else if (__bytes_left >= __size) {  // 内存池剩余空间不能满足申请，提供一个以上的区块

		__nobjs = (int)(__bytes_left / __size);

		__total_bytes = __size * __nobjs;

		__result = _S_start_free;

		_S_start_free += __total_bytes;

		return(__result);
	}
	else {       
		// 内存池剩余空间连一个区块的大小都无法提供                      
		size_t alloc_bytes =2 * __total_bytes + _S_round_up(_S_heap_size >> 4);

		// 内存池的剩余空间分给合适的空闲链表
		if (__bytes_left > 0) {

			_Obj* * __my_free_list =_S_free_list + _S_freelist_index(__bytes_left);

			((_Obj*)_S_start_free)->_M_free_list_link = *__my_free_list;
			*__my_free_list = (_Obj*)_S_start_free;
		}
		_S_start_free = (char*)malloc(alloc_bytes);  // 配置 heap 空间，用来补充内存池
		
		if (0 == _S_start_free) {  // heap 空间不足，malloc() 失败
			size_t __i;
			_Obj* * __my_free_list;
			_Obj* __p;

			for (__i = __size;__i <= (size_t)_MAX_BYTES;__i += (size_t)_ALIGN) 
			{
				__my_free_list = _S_free_list + _S_freelist_index(__i);
				__p = *__my_free_list;
				if (0 != __p) {

					*__my_free_list = __p->_M_free_list_link;
					_S_start_free = (char*)__p;
					_S_end_free = _S_start_free + __i;

				}
			}
			_S_end_free = 0;	
			_S_start_free = (char*)malloc(alloc_bytes);  // 调用第一级配置器

		}


		_S_heap_size += alloc_bytes;
		_S_end_free = _S_start_free + alloc_bytes;

		return(_S_chunk_alloc(__size, __nobjs));  // 递归调用自己
	}
}


void*_S_refill(size_t __n)
{
 	int __nobjs = 20;
	// 调用 _S_chunk_alloc()，缺省取 20 个区块作为 free list 的新节点
	char* __chunk = _S_chunk_alloc(__n, __nobjs);




	//获取分配的块之后，将对应块 放进相应下边数组中  链表串起来
	_Obj* * __my_free_list;
	_Obj* __result;
	_Obj* __current_obj;
	_Obj* __next_obj;
	int __i;

	// 如果只获得一个数据块，那么这个数据块就直接分给调用者，空闲链表中不会增加新节点
	if (1 == __nobjs) 
		return(__chunk);

	__my_free_list = _S_free_list + _S_freelist_index(__n);  // 否则根据申请数据块的大小找到相应空闲链表  

	__result = (_Obj*)__chunk;
	*__my_free_list = __next_obj = (_Obj*)(__chunk + __n);  // 第0个数据块给调用者，地址访问即chunk~chunk + n - 1 

	for (__i = 1; ; __i++) 
	{
		__current_obj = __next_obj;

		__next_obj = (_Obj*)((char*)__next_obj + __n);

		if (__nobjs - 1 == __i) {
			__current_obj->_M_free_list_link = 0;
			break;
		}
		else {
			__current_obj->_M_free_list_link = __next_obj;
		}
	}
	return(__result);
}


static void* allocate(size_t __n)
{
	void* __ret = 0;

	 {
		// 根据申请空间的大小寻找相应的空闲链表（16个空闲链表中的一个）
		_Obj* * __my_free_list= _S_free_list + _S_freelist_index(__n);

		_Obj*  __result = *__my_free_list;

		// 空闲链表没有可用数据块，就将区块大小先调整至 8 倍数边界，然后调用 _S_refill() 重新填充
		if (__result == 0)
			__ret = _S_refill(_S_round_up(__n));
		else {
			// 如果空闲链表中有空闲数据块，则取出一个，并把空闲链表的指针指向下一个数据块  
			*__my_free_list = __result->_M_free_list_link;
			__ret = __result;
		}
	}

	return __ret;
};
//================= 以上是malloc 使用第二级分配内存流程;省去了第一级==========
//================= 下面是dellocate归还内存流程；省去了第一级=================
static void deallocate(void* __p, size_t __n)
{

		_Obj* *  __my_free_list= _S_free_list + _S_freelist_index(__n);
		_Obj* __q = (_Obj*)__p;


		__q->_M_free_list_link = *__my_free_list;
		*__my_free_list = __q;

}
//=============主函数测试
int main(int argc, char** argv) {

	
	_S_start_free = 0;
	_S_start_free = 0;
	_S_heap_size = 0;
	_S_free_list[_NFREELISTS] = {0};


	int size_1 = 20;

	void* p1 = allocate(7);

	void* p2 = allocate(17);

	void* p3 = allocate(47);

	void* p4 = allocate(47);

	void* p5 = allocate(6);

	deallocate(p1,7);
	deallocate(p3, 47);
	deallocate(p4, 47);


	return 0;
}
