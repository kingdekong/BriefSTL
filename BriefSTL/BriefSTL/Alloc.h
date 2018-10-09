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
			_Align = 8 //С��������ϵ��߽�
		};
		enum
		{
			_Max_Bytes = 128 //С�����������
		};
		enum
		{
			_NFreeLists = 16 // free-list����(_Max_Bytes/_Align)
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

		static Obj* volatile _Free_list[_NFreeLists];//16������

		static size_t _FreeList_Index(size_t __bytes)
		{
			return ((__bytes)+(_Align - 1)) / (_Align - 1);//ȷ����ǰbytes
			//����16�������е���һ��
		}

		//����һ����СΪ__n�Ķ��󣬲��������ӽ�㵽free-list
		static void* _Refill(size_t _n);
		//��������_nobjs��С�Ŀռ䣬���Ҫ��Ĵ�С�������㣬��ô__nobjs�����
		static char* _Chunk_alloc(size_t __size, int& __nobjs);

	private:
		//�ڴ�����״̬
		static char* _Start_free;
		static char* _End_free;
		static size_t _Heap_size;

	public:
		// __n�������0
		static void* allocate(size_t __n)
		{
			void* __ret = 0;
			//����Ŀռ��С����_Max_Bytes,ֱ�ӵ���malloc
			if (__n > _Max_Bytes)
			{
				__ret = malloc_alloc::allocate(__n);
			}
			else
			{
				//��һ��д��
				Obj* volatile * __My_free_list = _Free_list + _FreeList_Index(__n);

				////�ڶ���д��
				//	size_t n = _FreeList_Index(__n);
				//	Obj* __My_free_list = _Free_list[n];

				Obj* __result = *__My_free_list;
				if (0 == __result)//list Ϊ��
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
		size_t __Total_bytes = __size * __nobjs;//�Ǵ�С
		size_t __Bytes_left = _End_free - _Start_free;

		//ʣ�µĴ�С��pool�Ĵ�С������Ҫ�����Ĵ�С
		if (__Bytes_left >= __Total_bytes) // __Result
		{                                  //     \  _______
			__Result = _Start_free; //(1)  //      \|_______| <- _Start_free(1)
			_Start_free += __Total_bytes;//(2) //   |_______|
			return (__Result);             //       |_______| <- _Start_free(2)
		}                                  //       |_______| <- _End_free
		else if (__Bytes_left >= __size) //�ڴ�ؿռ䲻����С��20
		{
			__nobjs = (int)(__Bytes_left / __size);
			__Total_bytes = __size * __nobjs;//�Ǵ�С
			__Result = _Start_free;
			_Start_free += __Total_bytes;
			return (__Result);
		}
		else //__Bytes_left < __size
		{
			size_t __Bytes_to_get =
				2 * __Total_bytes + _Round_up(_Heap_size >> 4);

			//��ʣ���__Bytes_left���¹�����list���棬�Ա�����ʹ��
			//�������pool��ʣ��ռ�
			if (__Bytes_left > 0)
			{
				Obj* volatile * __My_free_list =
					_Free_list + _FreeList_Index(__Bytes_left);

				((Obj*)_Start_free)->_Free_list_next = *__My_free_list;
				*__My_free_list = (Obj*)_Start_free;
			}

			//ʣ��ռ䲻��������£��ٴ�ʹ��malloc�����¿ռ�
			_Start_free = (char*)malloc(__Bytes_to_get);//�����СΪ__Bytes_left
														//�Ŀռ�

			//�������ʧ�ܣ�������е�pool�е�free-list��Ѱ�ҿ��õ�����
			if (nullptr == _Start_free)
			{
				size_t __i;
				Obj* volatile* __My_free_list;
				Obj* __p;

				for (__i = __size; __i < _Max_Bytes; __i += (_Align))
				{
					__My_free_list = _Free_list + _FreeList_Index(__i);
					__p = *__My_free_list;
					//��free-list���ҵ��˿ɹ�����Ŀռ�
					if (nullptr != __p)
					{
						*__My_free_list = __p->_Free_list_next;

						_Start_free = (char*)__p;
						_End_free = _Start_free + (__i);
						return(_Chunk_alloc(__size, __nobjs));
					}
				}
				//�����free-list ��û���ҵ��ɹ�����Ŀռ�
				//ʹ�õ�һ������������ռ�
				_End_free = nullptr;
				_Start_free = (char*)malloc_alloc::allocate(__Bytes_to_get);
			}

			//�������ɹ��Ļ�
			_Heap_size += __Bytes_to_get;//�ۼ��ܵķ���ռ���
			_End_free = _Start_free + __Bytes_to_get;
			//�ռ������ٴ��Եݹ�ķ�ʽ����chunk_alloc,���·���ռ�
			return(_Chunk_alloc(__size, __nobjs));
		}
	}

	// ȱʡȡ20���½ڵ�
	template<int __inst>
	inline void * __second_alloc_template<__inst>::_Refill(size_t __n)
	{
		// ȱʡȡ20���½ڵ�,����__n�����list���棬��ʵÿ�ζ��Ƿ���double
		// ����20*__n��С�ռ䣬�������ã�����ν��pool
		int __nobjs = 20;
		//����_Chunk_alloc������ȥ��__nobjs��������Ϊfree-list���½ڵ�
		//__nobjs ��pass by reference
		char* __chunk = _Chunk_alloc(__n, __nobjs);

		Obj* volatile * __My_free_list;
		Obj* __Result;
		Obj* __Current_Obj;
		Obj* __Next_Obj;
		int  __i;

		//��ֻ��Ҫһ�����飬�������ͷ�����������ã�free list���½��
		if (1 == __nobjs)
		{
			return (__chunk);
		}

		//����ת������ free list �������½ڵ�
		__My_free_list = _Free_list + _FreeList_Index(__n);

		//��������chunk�ռ��ڽ���free list��__chunk��pool�Ŀ�ʼ
		__Result = (Obj*)__chunk;

		//(__chunk + __n)
		*__My_free_list = __Next_Obj = (Obj*)(__chunk + __n);
		//��free list �ĸ����ڵ㴮����
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

