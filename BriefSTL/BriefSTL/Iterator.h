#ifndef _JDG_STL_ITERATOR_H
#define _JDG_STL_ITERATOR_H

namespace BriefSTL
{
	struct input_iterator_tag
	{

	};
	struct output_iterator_tag
	{

	};
	struct forward_iterator_tag :public input_iterator_tag {};
	struct bidirectional_iterator_tag :public forward_iterator_tag {};
	struct random_access_iterator_tag :public bidirectional_iterator_tag {};

	template<typename _Tp, typename _Distance>
	struct input_iterator
	{
		typedef input_iterator_tag iterator_category;
		typedef _Tp value_type;
		typedef _Distance difference_type;
		typedef _Tp* pointer;
		typedef _Tp& reference;
	};

	struct output_iterator
	{
		typedef output_iterator_tag iterator_category;
		typedef void value_type;
		typedef void difference_type;
		typedef void pointer;
		typedef void reference;
	};

	template<typename _Tp, typename _Distance>
	struct forward_iterator
	{
		typedef forward_iterator_tag iterator_category;
		typedef _Tp value_type;
		typedef _Distance difference_type;
		typedef _Tp* pointer;
		typedef _Tp& reference;
	};

	template<typename _Tp, typename _Distance>
	struct bidirectional_iterator
	{
		typedef bidirectional_iterator_tag iterator_category;
		typedef _Tp value_type;
		typedef _Distance difference_type;
		typedef _Tp* pointer;
		typedef _Tp& reference;
	};

	template<typename _Tp, typename _Distance>
	struct random_access_iterator
	{
		typedef random_access_iterator_tag iterator_category;
		typedef _Tp value_type;
		typedef _Distance difference_type;
		typedef _Tp* pointer;
		typedef _Tp& reference;
	};

	//
	template<typename _Iterator>
	struct iterator_traits
	{
		typedef typename _Iterator::iterator_category iterator_category;
		typedef typename _Iterator::value_type value_type;
		typedef typename _Iterator::difference_type difference_type;
		typedef typename _Iterator::pointer pointer;
		typedef typename _Iterator::reference reference;
	};

	template<typename _Tp>
	struct iterator_traits<_Tp*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef _Tp value_type;
		typedef ptrdiff_t difference_type;
		typedef _Tp* pointer;
		typedef _Tp& reference;
	};

	template<typename _Tp>
	struct iterator_traits<const _Tp*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef _Tp value_type;
		typedef ptrdiff_t difference_type;
		typedef const _Tp* pointer;
		typedef const _Tp& reference;
	};

	/*

	//template<typename _Iter>
	//inline typename iterator_traits<_Iter>::iterator_category
	//	__iterator_category(const _Iter&)
	//{
	//	typedef typename iterator_traits<_Iter>::iterator_category _Category;
	//	return _Category();
	//}

	//template<typename _Iter>
	//inline typename iterator_traits<_Iter>::iterator_category
	//	iterator_category(const _Iter& __i)
	//{
	//	return __iterator_category(__i);
	//}

	*/
	//该函数将上面两个函数合并，该函数的参数实际上没有用到，
	//用到的只是参数的类型

	//iterator_category有5种
	template<typename _Iter>
	inline typename iterator_traits<_Iter>::iterator_category
		iterator_category(const _Iter& __i)
	{
		typedef typename iterator_traits<_Iter>::iterator_category _Category;
		return _Category();
	}

	template<typename _Iter>
	inline typename iterator_traits<_Iter>::difference_type*
		difference_type(const _Iter& __i)
	{
		return static_cast<typename iterator_traits<_Iter>::difference_type*>(0);
	}

	template<typename _Iter>
	inline typename iterator_traits<_Iter>::value_type*
		value_type(const _Iter& __i)
	{
		return static_cast<typename iterator_traits<_Iter>::value_type*>(0);
	}

	//template <class _Tp, class _Distance>
	//inline _Tp* value_type(const input_iterator<_Tp, _Distance>&)
	//{
	//	return (_Tp*)(0);
	//}

	//template <class _Tp, class _Distance>
	//inline _Tp* value_type(const forward_iterator<_Tp, _Distance>&)
	//{
	//	return (_Tp*)(0);
	//}

	//template <class _Tp, class _Distance>
	//inline _Tp* value_type(const bidirectional_iterator<_Tp, _Distance>&)
	//{
	//	return (_Tp*)(0);
	//}

	//template <class _Tp, class _Distance>
	//inline _Tp* value_type(const random_access_iterator<_Tp, _Distance>&)
	//{
	//	return (_Tp*)(0);
	//}

	//template <class _Tp>
	//inline _Tp* value_type(const _Tp*) { return (_Tp*)(0); }


#define __ITERATOR_CATEGORY(__i) iterator_category(__i)
#define __DISTANCE_TYPE(__i)     distance_type(__i)
#define __VALUE_TYPE(__i)        value_type(__i)

	// distance start
	// __n是距离
	// 根据不同的数据类型，采取不同的处理方式，让代码效率更高
	template<typename _InputIterator, typename _Distance>
	inline void _distance(_InputIterator __first, _InputIterator __last,
		_Distance& __n, input_iterator_tag)
	{
		while (__first != __last)
		{
			++__first;
			++__n;
		}
	}

	template<typename _RandomAccessIterator, typename _Distance>
	inline void _distance(_RandomAccessIterator __first, _RandomAccessIterator __last,
		_Distance& __n, random_access_iterator_tag)
	{
		__n += __last - __first;
	}

	template<typename _InputIterator, typename _Distance>
	inline void distance(_InputIterator __first, _InputIterator __last,
		_Distance& __n, input_iterator_tag)
	{
		_distance(__first, __last, __n, iterator_category(__first));
	}

	//特化
	template<typename _InputIterator>
	inline typename iterator_traits<_InputIterator>::difference_type
		__distance(_InputIterator __first, _InputIterator __last, input_iterator_tag)
	{
		typename iterator_traits<_InputIterator>::difference_type __n = 0;
		while (__first != __last)
		{
			++__first;
			++__n;
		}
		return __n;
	}

	template<typename _RandomAccessIterator>
	inline typename iterator_traits<_RandomAccessIterator>::difference_type
		__distance(_RandomAccessIterator __first, _RandomAccessIterator __last,
			random_access_iterator_tag)
	{
		return __first - __last;
	}

	template<typename _InputIterator>
	inline typename iterator_traits<_InputIterator>::difference_type
		distance(_InputIterator __first, _InputIterator __last)
	{
		typename iterator_traits<_InputIterator>::iterator_category _Category;

		return _distance(__first, __last, _Category());
	}

	// distance end

	// Advance start
	template<typename _BidirectionalIterator, typename _Distance>
	inline void __advance(_BidirectionalIterator& __i, _Distance __n,
		bidirectional_iterator_tag)
	{
		if (__n > 0)
		{
			while (__n--)
			{
				++__i;
			}
		}
		else
		{
			while (__n++)
			{
				--__i;
			}
		}
	}
	template<typename _RandomAccessIterator, typename _Distance>
	inline void __advance(_RandomAccessIterator& __i, _Distance __n,
		random_access_iterator_tag)
	{
		__i += __n;
	}

	template<typename _InputIterator, typename _Distance>
	inline void advance(_InputIterator& __i, _Distance __n)
	{
		__advance(__i, __n, iterator_category(__i));
	}
	// Advance end

}

#endif // !_JDG_STL_ITERATOR_H

