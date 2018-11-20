#ifndef _JDG_STL_UNINITIALIZED_H
#define _JDG_STL_UNINITIALIZED_H
#include "Iterator.h"

namespace BriefSTL
{
	template<typename _InputIter, typename _ForwardIter >
	inline _ForwardIter
		__uninitialized_copy_aux(_InputIter __first, _InputIter __last,
			_ForwardIter __result, __true_type)
	{
		//Todo
		// 以后会换成Bried STL 中的copy算法实现
		return std::copy(__first, __last, __result);
	}

	template<typename _InputIter, typename _ForwardIter >
	_ForwardIter
		__uninitialized_copy_aux(_InputIter __first, _InputIter __last,
			_ForwardIter __result, __false_type)
	{
		_ForwardIter __cur = __result;
		for (; __first != __last; ++__first, ++__cur)
		{
			_Construct(&*__cur, *__first);
		}
		return __cur;
	}

	template<typename _InputIter, typename _ForwardIter, typename _Tp>
	inline _ForwardIter
		__uninitialized_copy(_InputIter __first, _InputIter __last,
			_ForwardIter __result, _Tp*)
	{
		typedef typename __type_traits<_Tp>::is_POD_type _Is_POD;
		return __uninitialized_copy_aux(__first, __last, __result, _Is_POD());
	}

	template<typename _InputIter, typename _ForwardIter>
	inline _ForwardIter
		uninitialized_copy(_InputIter __first, _InputIter __last,
			_ForwardIter __result)
	{
		return __uninitialized_copy(__first, __last, __result,
			__VALUE_TYPE(__result));
	}

	//特化 char* 
	inline char* uninitialized_copy(const char* __first, const char* __last,
		char* __result)
	{
		memmove(__result, __first, __last - __first);
		return __result + (__last - __first);
	}

	//特化 wchar*
	inline wchar_t* uninitialized_copy(const wchar_t* __first, const wchar_t* __last,
		wchar_t* __result)
	{
		memmove(__result, __first, sizeof(wchar_t)*(__last - __first));
		return __result + (__last - __first);
	}

	// uninitialized_copy_n : not part of the c++ standard
	// 
	//template <typename _InputIter, typename _Size, typename _ForwardIter>
	//std::pair< _InputIter, _ForwardIter>
	//	__uninitialized_copy_n(_InputIter __first, _Size __count,
	//		_ForwardIter __result, input_iterator_tag)
	//{
	//	_ForwardIter __cur = __result;

	//	for (; __count > 0; --__count, ++__first, ++__cur)
	//	{
	//		_Construct(&*__cur, *__first);
	//	}
	//	return pair<_InputIter, _ForwardIter>(__first, __cur);
	//}


	//template <class _RandomAccessIter, class _Size, class _ForwardIter>
	//inline pair<_RandomAccessIter, _ForwardIter>
	//	__uninitialized_copy_n(_RandomAccessIter __first, _Size __count,
	//		_ForwardIter __result, random_access_iterator_tag)
	//{
	//	_RandomAccessIter __last = __first + __count;
	//	return pair<_RandomAccessIter, _ForwardIter>(__last,
	//		uninitialized_copy(__first, __last, __result));
	//}

	//template <class _InputIter, class _Size, class _ForwardIter>
	//inline pair<_InputIter, _ForwardIter>
	//	__uninitialized_copy_n(_InputIter __first, _Size __count, _ForwardIter __result)
	//{
	//	return __uninitialized_copy_n(__first, __count, __result,
	//		__ITERATOR_CATEGORY(__first));
	//}

	//template <class _InputIter, class _Size, class _ForwardIter>
	//inline pair<_InputIter, _ForwardIter>
	//	uninitialized_copy_n(_InputIter __first, _Size __count, _ForwardIter __result)
	//{
	//	return __uninitialized_copy_n(__first, __count, __result,
	//		__ITERATOR_CATEGORY(__first));
	//}


	//uninitialized_fill start
	template<typename _ForwardIter, typename _Tp>
	inline void
		__uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last,
			const _Tp& __x, __true_type)
	{
		//Todo
		//将来替换为BriefSTL的fill
		std::fill(__first, __last, __x);
	}

	template<typename _ForwardIter, typename _Tp>
	void
		__uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last,
			const _Tp& __x, __false_type)
	{
		_ForwardIter __cur = __first;
		for (; __cur != __last; ++__cur)
		{
			_Construct(&*__cur, __x);
		}
	}

	template<typename _ForwardIter, typename _Tp, typename _Tp1>
	inline void
		__uninitialized_fill(_ForwardIter __first, _ForwardIter __last,
			const _Tp&__x, _Tp1*)
	{
		typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
		__uninitialized_fill_aux(__first, __last, __x, _Is_POD);
	}
	// 
	template<typename _ForwardIter, typename _Tp>
	inline void
		uninitialized_fill(_ForwardIter __first, _ForwardIter __last,
			const _Tp& __x)
	{
		__uninitialized_fill(__first, __last, __x, __VALUE_TYPE(__first));
	}
	//uninitialized_fill end


	//uninitialized_fill_n start
	template <class _OutputIter, class _Size, class _Tp>
	_OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
	{
		for (; __n > 0; --__n, ++__first)
		{
			*__first = __value;
		}
		return __first;
	}

	template<typename _ForwardIter, typename _Size, typename _Tp>
	inline
		_ForwardIter __uninitialized_fill_n_aux(_ForwardIter __first, _Size __n,
			const _Tp& __x, __true_type)
	{
		//Todo
		//将来替换为BriefSTL的fill_n
		//return std::fill_n(__first, __n, __x);

		return  fill_n(__first, __n, __x);
	}

	template<typename _ForwardIter, typename _Size, typename _Tp>
	_ForwardIter __uninitialized_fill_n_aux(_ForwardIter __first, _Size __n,
		const _Tp& __x, __false_type)
	{
		_ForwardIter __cur = __first;
		for (; __n > 0; --__n, ++__cur)
		{
			_Construct(&*__cur, __x);
		}
		return __cur;
	}

	template<typename _ForwardIter, typename _Size, typename _Tp, typename _Tp1>
	inline _ForwardIter
		__uninitialized_fill_n(_ForwardIter __first, _Size __n,
			const _Tp& __x, _Tp1*)
	{
		typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
		return __uninitialized_fill_n_aux(__first, __n, __x, _Is_POD());
	}
	template<typename _ForwardIter, typename _Size, typename _Tp>
	inline _ForwardIter
		uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
	{
		std::cout << __x << std::endl;
		return __uninitialized_fill_n(__first, __n, __x, __VALUE_TYPE(__first));
	}

	//uninitialized_fill_n end
}

#endif // !_JDG_STL_UNINITIALIZED_H

