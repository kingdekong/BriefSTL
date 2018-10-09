#ifndef _JDG_STL_ALLOCATOR_H
#define _JDG_STL_ALLOCATOR_H
#include "Construct.h"
#include "Alloc.h"

namespace BriefSTL
{
	//类模板 泛化设计
	template<typename _Tp>
	class allocator
	{
	private:
		typedef alloc _Alloc;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef _Tp* pointer;
		typedef const _Tp* const_pointer;
		typedef _Tp& reference;
		typedef const _Tp& const_reference;
		typedef _Tp value_type;

	public:
		//没有指定分配大小的情况下，分配的大小为sizeof(_Tp)
		static _Tp* allocate()
		{
			return   static_cast<_Tp*>(_Alloc::allocate(sizeof(_Tp)));
		}

		static	_Tp* allocate(size_t __n, const void * = 0)
		{
			return __n == 0 ? 0 : static_cast<_Tp*>(_Alloc::allocate(__n * sizeof(_Tp)));
		}

		static	void deallocate(pointer __p)
		{
			_Alloc::deallocate(__p, sizeof(_Tp));
		}

		static	void deallocate(pointer __p, size_type __n)
		{
			_Alloc::deallocate(__p, __n * sizeof(_Tp));
		}

		//这种写法，容易理解
		static void construct(_Tp* ptr)
		{
			new(ptr) _Tp();
		}

		//第二种写法，其实就是上面typedef那些
		//void construct(pointer ptr)
		//{
		//	new(ptr) value_type();
		//}


		static	void construct(_Tp* ptr, const _Tp& value)
		{
			new(ptr) _Tp(value);
		}


		static	void destroy(_Tp* ptr)
		{
			ptr->~_Tp();
		}
		static	void destroy(_Tp* first, _Tp* last)
		{
			for (; first != last; ++first)
			{
				first->~_Tp();
			}
		}
	};


	//类模板 特化设计
	template<>
	class allocator<void>
	{
	private:
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef void* pointer;
		typedef const void* const_pointer;
		typedef void value_type;
	public:

	};

	////类模板 偏特化
	//template <typename _Tp>
	//class allocator<_Tp*>
	//{
	//public:

	//private:

	//};
}

#endif // !_JDG_STL_ALLOCATOR_H
