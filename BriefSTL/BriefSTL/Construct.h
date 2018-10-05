#ifndef _JDG_STL_CONSTRUCT_H
#define _JDG_STL_CONSTRUCT_H

#include <new.h>
#include "type_traits.h"

namespace BriefSTL
{
	template<typename _T1, typename _T2>
	inline void _Construct(_T1* _p, const _T2& _value)
	{
		new((void*)_p) _T1(_value);
	}

	template<typename _T1>
	inline void _Construct(_T1* _p)
	{
		new((void*)_p) _T1();
	}

	//Ö¸ÕëÄ£°å
	template<typename _Tp>
	inline void _Destory(_Tp* _pointer)
	{
		_pointer->~_Tp();
	}

	template<typename _FowardIterator>
	void __destory_aux(_FowardIterator _first, _FowardIterator _last, __false_type)
	{
		for (; _first != _last; ++_first)
		{
			destory(&* _first);
		}
	}

	template<typename _FowardIterator>
	void __destory_aux(_FowardIterator _first, _FowardIterator _last, __true_type)
	{
	}

	template<typename _FowardIterator, typename _Tp>
	inline void	__destory(_FowardIterator _first, _FowardIterator _last, _Tp*)
	{
		typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
		__destory_aux(_first, _last, _Trivial_destructor());
	}

	template<typename _FowardIterator>
	inline void _Destory(_FowardIterator _first, _FowardIterator _last)
	{
		__destory_aux(_first, _last, __VALUE_TYPE(_first));
	}

	inline void _Destory(char*, char*) {}
	inline void _Destory(int*, int*) {}
	inline void _Destory(long*, long*) {}
	inline void _Destory(float*, float*) {}
	inline void _Destory(double*, double*) {}

	template<typename _T1, typename _T2>
	inline void construct(_T1* __p, const _T2& __value)
	{
		_Construct(__p, __value);
	}

	template<typename _T1, typename _T2>
	inline void construct(_T1* __p)
	{
		_Construct(__p);
	}

	template<typename _Tp>
	inline void destory(_Tp* __pointer)
	{
		_Destory(__pointer);
	}

	template<typename _ForwardIterator>
	inline void destory(_ForwardIterator __first, _ForwardIterator __last)
	{
		_Destory(__first, __last);
	}
}

#endif // !_JDG_STL_CONSTRUCT_H
