#ifndef _JDG_STL_Vector_H
#define _JDG_STL_Vector_H
#include <algorithm> 
#include "Allocator.h"
#include "Uninitialized.h"
//#include "Construct.h"
//#include "Alloc.h"

namespace BriefSTL
{
	template<typename _Tp, typename Alloc = allocator<_Tp>>
	class vector
	{
	private:
		_Tp* M_start;
		_Tp* M_finish;
		_Tp* M_endofStorage;
		typedef allocator<_Tp> M_dataAllocator;

	public:
		typedef _Tp value_type;
		typedef _Tp* pointer_type;
		typedef const _Tp* const_pointer;
		typedef value_type* iterator;
		typedef const value_type* const_iterator;
		typedef value_type& reference;
		typedef const value_type& const_reference;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;


	protected:
		_Tp* _M_allocate(size_t __n)
		{
			return M_dataAllocator::allocate(__n);
		}
		void _M_deallocate(_Tp* __p, size_t __n)
		{
			M_dataAllocator::deallocate(__p, __n);
		}

		void _M_insert_aux(iterator __position, const _Tp& __x);
		void _M_insert_aux(iterator __position);

	public:
		iterator begin() { return M_start; }
		iterator end() { return M_finish; }
		const iterator begin() const { return M_start; }
		const iterator end()const
		{
			return M_finish;
		}
		size_type size()const
		{
			return size_type(end() - begin());
		}

		size_type max_size()const
		{
			return size_type(-1) / sizeof(_Tp);
		}
		size_type capacity()const
		{
			return (M_endofStorage - begin());
		}
		bool empty()const
		{
			return begin() == end();
		}

		reference operator[](size_type __n) {
			return *(begin() + __n);
		}
		const_reference operator[](size_type __n)const {
			return *(begin() + __n);
		}
		vector() :M_start(0), M_finish(0), M_endofStorage(0)
		{

		}

		explicit vector(const size_type __n)
			: M_start(0), M_finish(0), M_endofStorage(0)
		{
			M_start = _M_allocate(__n);
			M_finish = M_start;
			M_endofStorage = M_start + __n;

			M_finish = BriefSTL::uninitialized_fill_n(M_start, __n, _Tp());
		}
		vector(const size_type __n, const value_type& value)
			: M_start(0), M_finish(0), M_endofStorage(0)
		{
			std::cout << "value_type " << sizeof(_Tp) << std::endl;

			M_start = _M_allocate(__n);
			M_finish = M_start;
			M_endofStorage = M_start + __n;

			M_finish = BriefSTL::uninitialized_fill_n(M_start, __n, value);
		}

		~vector()
		{
			M_dataAllocator::deallocate(M_start, M_endofStorage - M_start);
		}

		vector<_Tp, Alloc>& operator=(const vector<_Tp, Alloc>& __x);
		void reserve(size_type __n)
		{
			if (capacity() < __n)
			{
				const size_type __old_size = size();
				iterator __tmp = _M_allocate_and_copy(__n, M_start, M_finish);
				destory(M_start, M_finish);
				_M_deallocate(M_start, M_endofStorage - M_start);
				M_start = __tmp;
				M_finish = M_start + __old_size;
				M_endofStorage = M_start + __n;
			}
		}

		reference front() {
			return *begin();
		}
		const_reference front()const {
			return *begin();
		}
		reference back() {
		}
		const_reference back()const {
			return *(end() - 1);
		}

		void push_back(const _Tp& __x)
		{
			if (M_finish != M_endofStorage)
			{
				construct(M_finish, __x);
				++M_finish;
			}
			else
			{
				_M_insert_aux(end(), __x);
			}
		}

		void push_back()
		{
			if (M_finish != M_endofStorage)
			{
				construct(M_finish);
				++M_finish;
			}
			else
			{
				_M_insert_aux(end());
			}
		}

		void swap(vector<_Tp, Alloc>&__x)
		{
			std::swap(M_start, __x, M_start);
			std::swap(M_finish, __x, M_finish);
			std::swap(M_endofStorage, __x, M_endofStorage);
		}

		iterator insert(iterator __position, const _Tp&__x)
		{
			size_type __n = __position - begin();
			if (M_finish != M_endofStorage && __position == end())
			{
				construct(M_finish);
				++M_finish;
			}
			else
			{
				_M_insert_aux(__position);
			}
			return begin() + __n;
		}

		void insert(iterator __pos, size_type __n, const _Tp&__x)
		{
			_M_fill_insert(__pos, __n, __x);
		}

		void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);

		void pop_back()
		{
			--M_finish;
			destory(M_finish);
		}

		iterator erase(iterator __position)
		{
			if (__position + 1 != end())
			{
				copy(__position + 1, M_finish, __position);
			}
			--M_finish;
			destory(M_finish);
			return __position;
		}
		iterator erase(iterator __first, iterator __last)
		{
			iterator __i = copy(__last, M_finish, __first);
			destory(__i, M_finish);
			M_finish = M_finish - (__last - __first);
			return __first;
		}

		void resize(size_type __newSize, const _Tp&__x)
		{
			if (__newSize < size())
			{
				erase(begin() + __newSize, end());
			}
			else
			{
				insert(end(), __newSize - size(), __x);
			}
		}

		void resize(size_type __newSize)
		{
			resize(__newSize, _Tp());
		}
		void clear()
		{
			erase(begin(), end());
		}
		template<typename _Tp, typename Alloc>
		bool operator==(/*const vector < _Tp, Alloc>&__x,*/ const vector<_Tp, Alloc>&__y)
		{
			return this.size() == __y.size() &&
				std::equal(this.begin(), this.end(), __y.begin());
		}

		template <class _Tp, class Alloc>
		inline bool
			operator<(/*const vector<_Tp, Alloc>& __x,*/ const vector<_Tp, Alloc>& __y)
		{

		}
	};

	template<typename _Tp, typename Alloc>
	void vector<_Tp, Alloc>::_M_insert_aux(iterator __position, const _Tp & __x)
	{
		if (M_finish != M_endofStorage)
		{
			construct(M_finish, *(M_finish - 1));
			++M_finish;
			_Tp __x_copy = __x;
			std::copy_backward(__position, M_finish - 2, M_finish - 1);
			*__position = __x_copy;
		}
		else
		{
			const size_type __old_size = size();
			size_type __len = __old_size != 0 ? 2 * __old_size : 1;
			iterator __new_start = _M_allocate(__len);
			iterator __new_finish = __new_start;

			//uninitialized_copy 返回的是新的空间 的结束地址
			__new_finish = uninitialized_copy(M_start, __position, __new_start);
			construct(__new_finish, __x);
			++__new_finish;
			__new_finish = uninitialized_copy(__position, M_finish, __new_finish);

			destory(begin(), end());
			_M_deallocate(M_start, M_endofStorage - M_start);
			M_start = __new_start;
			M_finish = __new_finish;
			M_endofStorage = __new_start + __len;
		}
	}

	template<typename _Tp, typename Alloc>
	inline void vector<_Tp, Alloc>::_M_insert_aux(iterator __position)
	{
		if (M_finish != M_endofStorage)
		{
			construct(M_finish, *(M_finish - 1));
			++M_finish;
			copy_backward(__position, M_finish - 2, M_finish - 1);
			*__position = _Tp();
		}
		else
		{
			const size_type __old_size = size();
			const size_type __len = __old_size != 0 ? 2 * __old_size : 1;
			iterator __new_start = _M_allocate(__len);
			iterator __new_finish = __new_start;

			//uninitialized_copy 返回的是新的空间 的结束地址
			__new_finish = uninitialized_copy(M_start, __position, __new_start);
			construct(__new_finish);
			++__new_finish;
			__new_finish = uninitialized_copy(__position, M_finish, __new_finish);

			destory(begin(), end());
			_M_deallocate(M_start, M_endofStorage - M_start);
			M_start = __new_start;
			M_finish = __new_finish;
			M_endofStorage = __new_start + __len;
		}
	}

	template <typename _Tp, typename Alloc>
	vector < _Tp, Alloc>&
		vector<_Tp, Alloc>::operator=(const vector<_Tp, Alloc>&__x)
	{
		if (&__x != this)
		{
			const size_type __xlen = __x.size();
			if (__xlen > capacity())
			{
				iterator __tmp = _M_allocate_and_copy(__xlen, __x.begin(), __x.end());
				destory(M_start, M_finish);
				_M_deallocate(M_start, M_endofStorage - M_start);
				M_start = __tmp;
				M_endofStorage = M_start + __xlen;
			}
			else if (size() >= __xlen)
			{
				iterator __i = copy(__x.begin(), __x.end(), begin());
				destory(__i, M_finish);
			}
			else
			{
				copy(__x.begin(), __x.begin() + size(), M_start);
				uninitialized_copy(__x.begin() + size(), __x.end(), M_finish);
			}
			M_finish = M_start + __xlen;
		}
		return *this;
	}

	template<typename _Tp, typename Alloc>
	inline void vector<_Tp, Alloc>::_M_fill_insert(iterator __pos, size_type __n,
		const _Tp & __x)
	{
		if (__n != 0)
		{
			if (size_type(M_endofStorage - M_finish) >= __n)
			{
				_Tp __x_copy = __x;
				const size_type __elems_after = M_finish - __pos;
				iterator __old_finish = M_finish;
				if (__elems_after > __n)
				{
					//uninitialized_copy(M_finish - __n, M_finish, M_finish);
					M_finish += __n;
					std::copy_backward(__pos, __old_finish, M_finish);
					M_finish += __elems_after;
					std::fill(__pos, __old_finish, __x_copy);
				}
				else
				{
					uninitialized_copy_n(M_finish, __n - __elems_after, __x_copy);
					M_finish += __n - __elems_after;
					M_finish += __elems_after;
					std::fill(__pos, __old_finish, __x_copy);
				}
			}
			else
			{
				const size_type __old_size = size();
				const size_type __len = __old_size + std::max(__old_size, __n);
				iterator __new_start = _M_allocate(__len);
				iterator __new_finish = __new_start;

				__new_finish = uninitialized_copy(M_start, __pos, __new_start);
				__new_finish = uninitialized_fill_n(__new_finish, __n, __x);
				__new_finish = uninitialized_copy(__pos, M_finish, M_finish);

				destory(M_start, M_finish);
				M_start = __new_start;
				M_finish = __new_finish;
				M_endofStorage = __new_start + __len;
			}
		}
	}



}
#endif
