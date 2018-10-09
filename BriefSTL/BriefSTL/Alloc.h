#ifndef _JDG_STL_ALLOC_H
#define _JDG_STL_ALLOC_H
#include <cstdlib>

namespace BriefSTL
{
	template<int __inst>
	class __first_alloc_template
	{
	public:
		static void* allocate(size_t __n)
		{
			void* __result = malloc(__n);
			return __result;
		}

		static void deallocate(void* __p, size_t)
		{
			free(__p);
		}

	private:

	};

	typedef __first_alloc_template<0> malloc_alloc;

	template<int __inst>
	class __second_alloc_template
	{
	private:
		//data area
		enum
		{
			_Align = 8 //小型区块的上调边界
		};
		enum
		{
			_Max_Bytes = 128 //小型区块的上限
		};
		enum
		{
			_NFreeLists = 16 // free-list个数(_Max_Bytes/_Align)
		};
	private:
		//function area
		static size_t _Round_up(size_t __bytes)
		{
			return (((__bytes)+_Align - 1)&~(_Align - 1));
		}

		union Obj
		{
			union Obj* _Free_list_next;
			char _Client_Data[1];
		};

		static Obj* volatile _Free_list[_NFreeLists];//16个队列

		static size_t _FreeList_Index(size_t __bytes)
		{
			return ((__bytes)+(_Align - 1)) / (_Align - 1);//确定当前bytes
			//所在16个队列中的哪一个
		}

		//返回一个大小为__n的对象，并重新增加结点到free-list
		static void* _Refill(size_t _n);
		//分配容纳_nobjs大小的空间，如果要求的大小不能满足，那么__nobjs会减少
		static char* _Chunk_alloc(size_t __size, int& __nobjs);

	private:
		//内存块分配状态
		static char* _Start_free;
		static char* _End_free;
		static size_t _Heap_size;

	public:
		// __n必须大于0
		static void* allocate(size_t __n)
		{
			void* __ret = 0;
			//分配的空间大小大于_Max_Bytes,直接调用malloc
			if (__n > _Max_Bytes)
			{
				__ret = malloc_alloc::allocate(__n);
			}
			else
			{
				//第一种写法
				Obj* volatile * __My_free_list = _Free_list + _FreeList_Index(__n);

				////第二种写法
				//	size_t n = _FreeList_Index(__n);
				//	Obj* __My_free_list = _Free_list[n];

				Obj* __result = *__My_free_list;
				if (0 == __result)//list 为空
				{
					__ret = _Refill(_Round_up(__n));
				}
				else
				{
					*__My_free_list = __result->_Free_list_next;
					__ret = __result;
				}
			}
			return __ret;
		}

		static void deallocate(void* __p, size_t __n)
		{
			if (__n > _Max_Bytes)
			{
				malloc_alloc::deallocate(__p, __n);
			}
			else
			{
				Obj* volatile * _My_free_list = _Free_list + _FreeList_Index(__n);

				Obj* __q = (Obj*)__p;

				__q->_Free_list_next = *_My_free_list;
				*_My_free_list = __q;
			}
		}

		static void* reallocate(void* __p, size_t __old_sz, size_t __new_sz)
		{
			if ((__old_sz > _Max_Bytes) && (__new_sz > _Max_Bytes))
			{
				return (realloc(__p, __new_sz));
			}

			deallocate(__p, __old_sz);
			void* __result = allocate(__p, __new_sz);

			return __result;
		}
	};

	typedef __second_alloc_template<0> alloc;

	template<int inst>
	char* __second_alloc_template<inst>::_Start_free = nullptr;
	template<int inst>
	char* __second_alloc_template<inst>::_End_free = nullptr;
	template<int inst>
	size_t __second_alloc_template<inst>::_Heap_size = 0;

	template<int inst>
	typename __second_alloc_template<inst>::Obj* volatile
		__second_alloc_template<inst>::_Free_list[__second_alloc_template<inst>::_NFreeLists]
		= { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, };

	template<int __inst>
	inline char * __second_alloc_template<__inst>::_Chunk_alloc(size_t __size, int & __nobjs)
	{
		char* __Result;
		size_t __Total_bytes = __size * __nobjs;//是大小
		size_t __Bytes_left = _End_free - _Start_free;

		//剩下的大小（pool的大小）大于要求分配的大小
		if (__Bytes_left >= __Total_bytes) // __Result
		{                                  //     \  _______
			__Result = _Start_free; //(1)  //      \|_______| <- _Start_free(1)
			_Start_free += __Total_bytes;//(2) //   |_______|
			return (__Result);             //       |_______| <- _Start_free(2)
		}                                  //       |_______| <- _End_free
		else if (__Bytes_left >= __size) //内存池空间不够，小于20
		{
			__nobjs = (int)(__Bytes_left / __size);
			__Total_bytes = __size * __nobjs;//是大小
			__Result = _Start_free;
			_Start_free += __Total_bytes;
			return (__Result);
		}
		else //__Bytes_left < __size
		{
			size_t __Bytes_to_get =
				2 * __Total_bytes + _Round_up(_Heap_size >> 4);

			//将剩余的__Bytes_left重新挂载在list上面，以备后面使用
			//充分利用pool中剩余空间
			if (__Bytes_left > 0)
			{
				Obj* volatile * __My_free_list =
					_Free_list + _FreeList_Index(__Bytes_left);

				((Obj*)_Start_free)->_Free_list_next = *__My_free_list;
				*__My_free_list = (Obj*)_Start_free;
			}

			//剩余空间不够的情况下，再次使用malloc分配新空间
			_Start_free = (char*)malloc(__Bytes_to_get);//分配大小为__Bytes_left
														//的空间

			//如果分配失败，则从现有的pool中的free-list里寻找可用的区块
			if (nullptr == _Start_free)
			{
				size_t __i;
				Obj* volatile* __My_free_list;
				Obj* __p;

				for (__i = __size; __i < _Max_Bytes; __i += (_Align))
				{
					__My_free_list = _Free_list + _FreeList_Index(__i);
					__p = *__My_free_list;
					//在free-list上找到了可供分配的空间
					if (nullptr != __p)
					{
						*__My_free_list = __p->_Free_list_next;

						_Start_free = (char*)__p;
						_End_free = _Start_free + (__i);
						return(_Chunk_alloc(__size, __nobjs));
					}
				}
				//如果在free-list 上没有找到可供分配的空间
				//使用第一级配置器分配空间
				_End_free = nullptr;
				_Start_free = (char*)malloc_alloc::allocate(__Bytes_to_get);
			}

			//如果分配成功的话
			_Heap_size += __Bytes_to_get;//累计总的分配空间量
			_End_free = _Start_free + __Bytes_to_get;
			//空间充足后，再次以递归的方式调用chunk_alloc,重新分配空间
			return(_Chunk_alloc(__size, __nobjs));
		}
	}

	// 缺省取20个新节点
	template<int __inst>
	inline void * __second_alloc_template<__inst>::_Refill(size_t __n)
	{
		// 缺省取20个新节点,放在__n对象的list下面，其实每次都是分配double
		// 留下20*__n大小空间，以作备用，即所谓的pool
		int __nobjs = 20;
		//调用_Chunk_alloc，尝试去的__nobjs个区块作为free-list的新节点
		//__nobjs 是pass by reference
		char* __chunk = _Chunk_alloc(__n, __nobjs);

		Obj* volatile * __My_free_list;
		Obj* __Result;
		Obj* __Current_Obj;
		Obj* __Next_Obj;
		int  __i;

		//若只需要一个区块，这个区块就分配给调用者用，free list无新结点
		if (1 == __nobjs)
		{
			return (__chunk);
		}

		//否则转动调用 free list ，纳入新节点
		__My_free_list = _Free_list + _FreeList_Index(__n);

		//以下是在chunk空间内建立free list，__chunk是pool的开始
		__Result = (Obj*)__chunk;

		//(__chunk + __n)
		*__My_free_list = __Next_Obj = (Obj*)(__chunk + __n);
		//将free list 的各个节点串起来
		for (__i = 1;; __i++)
		{
			__Current_Obj = __Next_Obj;
			__Next_Obj = (Obj*)((char*)__Next_Obj + __n);
			if (__nobjs - 1 == __i)
			{
				__Current_Obj->_Free_list_next = 0;
				break;
			}
			else
			{
				__Current_Obj->_Free_list_next = __Next_Obj;

			}
		}
		return __Result;
	}

}
#endif // !_JDG_STL_ALLOC_H

