#ifndef _JDG_STL_ALLOCATOR_H
#define _JDG_STL_ALLOCATOR_H
#include "Construct.h"
#include "Alloc.h"

namespace BriefSTL
{
	//��ģ�� �������
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
		//û��ָ�������С������£�����Ĵ�СΪsizeof(_Tp)
		static _Tp* allocate()
		{
			return   static_cast<_Tp*>(_Alloc::allocate(sizeof(_Tp)));
		}

		static	_Tp* allocate(size_t __n, const void * = 0)
		{
			return __n == 0 ? 0 : static_cast<_Tp*>(_Alloc::allocate(__n * sizeof(_Tp)));
			//return  _Alloc::allocate(__n  ) ;
		}

		static	void deallocate(pointer __p)
		{
			_Alloc::deallocate(__p, sizeof(_Tp));
		}

		static	void deallocate(pointer __p, size_type __n)
		{
			_Alloc::deallocate(__p, __n * sizeof(_Tp));
		}

		//����д�����������
		static void construct(_Tp* ptr)
		{
			new(ptr) _Tp();
		}

		//�ڶ���д������ʵ��������typedef��Щ
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


	//��ģ�� �ػ����
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

	////��ģ�� ƫ�ػ�
	//template <typename _Tp>
	//class allocator<_Tp*>
	//{
	//public:

	//private:

	//};
}

#endif // !_JDG_STL_ALLOCATOR_H
