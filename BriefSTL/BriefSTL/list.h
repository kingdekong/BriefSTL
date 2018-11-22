#ifndef _JDG_STL_LIST_H
#define _JDG_STL_LIST_H

namespace BriefSTL
{
	struct ListNodeBase
	{
		ListNodeBase* M_next;
		ListNodeBase* M_prev;
	};

	template<typename _Tp>
	class ListNode :public ListNodeBase
	{
	public:
		_Tp M_data;
	};

	//和data没有关系 只是操作M_next和M_prev
	class ListIteratorBase
	{
	public:
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef bidirectional_iterator_tag iterator_category;

		ListNodeBase* M_node;

		ListIteratorBase(ListNodeBase* _x) :M_node(_x) {}
		ListIteratorBase() {};

		void M_inc()
		{
			M_node = M_node->M_next;
		}
		void M_decr()
		{
			M_node = M_node->M_prev;
		}

		bool operator==(const ListIteratorBase& _x)const
		{
			return M_node == _x.M_node;
		}

		bool operator!=(const ListIteratorBase& _x)const
		{
			return M_node != _x.M_node;
		}


	};

	template<typename _Tp  >
	class ListIterator :public ListIteratorBase
	{
	public:
		typedef ListIterator<_Tp > iterator;
		typedef ListIterator<_Tp > constIterator;
		typedef ListIterator<_Tp >  _self;

		typedef _Tp value_type;
		typedef _Tp* pointer;
		typedef _Tp& reference;
		typedef ListNode<_Tp> _Node;

		ListIterator(_Node* _x) :ListIteratorBase(_x) {}
		ListIterator() {}
		ListIterator(const iterator* _x) :ListIteratorBase(_x->M_node) {}

		reference operator*()const { return ((_Node*)M_node)->M_data; }

		_self& operator++()
		{
			this->M_inc();
			return *this;
		}

		_self operator++(int)
		{
			_self temp = *this;
			this->M_inc();
			return temp;
		}

		_self& operator--()
		{
			this->M_decr();
			return *this;
		}
		_self operator--(int)
		{
			_self temp = *this;
			this->M_decr();
			return temp;
		}
	};

	//
	inline bidirectional_iterator_tag
		iterator_catagory(const ListIteratorBase&)
	{
		return bidirectional_iterator_tag();
	}
	template <typename _Tp, typename _Ref, typename _Ptr>
	inline _Tp* value_type(const ListIterator<_Tp>&)
	{
		return 0;
	}
	inline ptrdiff_t*
		distance_type(const ListIteratorBase&)
	{
		return 0;
	}

	//
	template<typename _Tp, typename Alloc>
	class ListBase
	{
	public:
		typedef Alloc allocator_type;
		allocator_type getAllocator()const { return allocator_type(); }

		//constructor
		ListBase(const allocator_type&)
		{
			M_node = M_getNode();
			M_node->M_next = M_node;
			M_node->M_prev = M_node;
		}

		~ListBase()
		{
			clear();
			M_putNode(this->M_node);
		}

		void clear();

	protected:
		typedef allocator<ListNode<_Tp>> Alloc_type;

		ListNode<_Tp>* M_getNode() { return  Alloc_type::allocate(1); }
		void M_putNode(ListNode<_Tp>* _p) { Alloc_type::deallocate(_p, 1); }

	protected:
		ListNode<_Tp>* M_node;
	};

	template<typename _Tp, typename Alloc>
	inline void ListBase<_Tp, Alloc>::clear()
	{
		ListNode<_Tp>* _cur = (ListNode<_Tp>*)M_node->M_next;
		while (_cur != M_node)
		{
			ListNode<_Tp>* tmp = _cur;
			_cur = (ListNode<_Tp>*)_cur->M_next;
			_Destory(&tmp->M_data);
			M_putNode(tmp);
		}
		M_node->M_next = M_node;
		M_node->M_prev = M_node;
	}

	template<typename _Tp, typename Alloc = allocator<_Tp>>
	class list :protected ListBase<_Tp, Alloc>
	{
	public:
		typedef ListBase<_Tp, Alloc> Base;
	protected:
		typedef void* VoidPointer;

	public:
		typedef _Tp valueType;
		typedef _Tp* pointer;
		typedef const _Tp* constPointer;
		typedef _Tp& reference;
		typedef const _Tp& constReference;
		typedef ListNode<_Tp> Node;
		typedef size_t sizeType;
		typedef ptrdiff_t differenceType;

		typedef typename Base::allocator_type allocatorType;
		allocatorType getAllocator()const { return Base::getAllocator(); }

	public:
		typedef ListIterator<_Tp> iterator;
		typedef ListIterator<_Tp> constIterator;
	protected:
		Node* M_createNode(const _Tp& _x)
		{
			Node* _p = this->M_getNode();
			_Construct(&_p->M_data, _x);
			return _p;
		}

	public:
		//constructor 
		explicit list(const allocatorType& _a = allocatorType()) :Base(_a) {}

		//public interface
		iterator begin() { return (Node*)(this->M_node->M_next); }
		constIterator begin()const { return (Node*)(this->M_node->M_next); }

		iterator end() { return this->M_node; }
		constIterator end()const { return this->M_node; }

		bool empty() const { return this->M_node == this->M_node->M_next; }

		sizeType size()const
		{
			sizeType _result = 0;
			distance(begin(), end(), _result);
			return _result;
		}

		sizeType MaxSize()const { return sizeType(-1); }

		reference front() { return *begin(); }
		constReference front()const { return *begin(); }

		reference back() { return *(--end()); }
		constReference back() const { return *(--end()); }

		void swap(list<_Tp, Alloc>& _x)
		{
			std::swap(this->M_node, _x.M_node);
		}

		iterator insert(iterator _position, const _Tp& _x)
		{
			Node* tmp = M_createNode(_x);
			tmp->M_next = _position.M_node;
			tmp->M_prev = _position.M_node->M_prev;

			_position.M_node->M_prev->M_next = tmp;
			_position.M_node->M_prev = tmp;

			return tmp;
		}

		iterator insert(iterator _position) { return insert(_position, _Tp()); }

		void insert(iterator _pos, sizeType _n, const _Tp& _x)
		{
			M_fillInsert(_pos, _n, _x);
		}

		void insert(iterator __position, const _Tp* __first, const _Tp* __last);
		void insert(iterator __position, constIterator __first, constIterator __last);

		void M_fillInsert(iterator _pos, sizeType _n, const _Tp& _x);

		void pushFront(const _Tp& _x) { insert(begin(), _x); }
		void pushBack(const _Tp& _x) { insert(end(), _x); }
		void pushFront() { insert(begin()); }
		void pushBack() { insert(end()); }

		iterator erase(iterator _position)
		{
			ListNodeBase* nextNode = _position.M_node->M_next;
			ListNodeBase* prevNode = _position.M_node->M_prev;

			Node* posNode = (Node*)_position.M_node;
			prevNode->M_next = nextNode;
			nextNode->M_prev = prevNode;

			_Destory(&posNode->M_data);
			this->M_putNode(posNode);
			return iterator((Node*)nextNode);
		}

		iterator erase(iterator fisrt, iterator last);
		void clear()
		{
			Base::clear();
		}
		void resize(sizeType _newSize, const _Tp& _x);
		void resize(sizeType _newSize) { return resize(_newSize, _Tp()); }

		void popFront() { erase(begin()); }
		void popBack()
		{
			iterator tmp = end();
			erase(--tmp);
		}

		//constructor
		list(sizeType _n, const _Tp& _value, const allocatorType& _a = allocatorType())
			:Base(_a)
		{
			insert(begin(), _n, _value);
		}
		explicit list(sizeType _n)
			:Base(allocatorType())
		{
			insert(begin(), _n, _Tp());
		}

		list(const _Tp* __first, const _Tp* __last,
			const allocatorType& __a = allocatorType())
			: Base(__a)
		{
			this->insert(begin(), __first, __last);
		}
		list(const iterator _first, const iterator _last,
			const allocatorType& __a = allocatorType())
			: Base(__a)
		{
			this->insert(begin(), _first, _last);
		}

		list(const list<_Tp, Alloc>&_x)
			:Base(_x.getAllocator())
		{
			insert(begin(), _x.begin(), _x.end());
		}

		~list() {}

		list<_Tp, Alloc>& operator=(const list<_Tp, Alloc>&_x);

	public:
		void assign(sizeType _n, const _Tp&_val) { M_fillAssign(_n, _val); }
		void M_fillAssign(sizeType _n, const _Tp& _val);

	protected:
		void transfer(iterator _position, iterator _first, iterator _last)
		{
			if (_position != _last)
			{
				//remove [fisrt,last) from its old position
				_last.M_node->M_prev->M_next = _position.M_node;
				_first.M_node->M_prev->M_next = _last.M_node;
				_position.M_node->M_prev->M_next = _first.M_node;

				//splice [fisrt,last) into its new position
				ListNodeBase* tmp = _position.M_node->M_prev;
				_position.M_node->M_prev = _last.M_node->M_prev;
				_last.M_node->M_prev = _first.M_node->M_prev;
				_first.M_node->M_prev = tmp;
			}
		}
	public:
		void splice(iterator _position, list& _x)
		{
			if (!_x.empty())
			{
				this->transfer(_position, _x.begin(), _x.end());
			}
		}
		void splice(iterator _position, list&, iterator _i)
		{
			iterator _j = _i;
			++_j;
			if (_position == _i || _position == _j)
				return;
			this->transfer(_position, _i, _j);
		}
		void splice(iterator _position, list&, iterator _first, iterator _last)
		{
			if (_first != _last)
			{
				this->transfer(_position, _first, _last);
			}
		}

		void remove(const _Tp&_value);
		void unique();
		void merge(list& _x);
		void reverse();
		void sort();

	};

	//list 的operator== 不是定义在list内部的
	template<typename _Tp, typename Alloc>
	inline bool
		operator==(const list<_Tp, Alloc>&_x, const list<_Tp, Alloc>&_y)
	{
		typedef typename list<_Tp, Alloc>::constIterator constIterator;
		constIterator _end1 = _x.end();
		constIterator _end2 = _y.end();

		constIterator _begin1 = _x.begin();
		constIterator _begin2 = _y.begin();

		while ((_begin1 != _end1) && (_begin2 != _end2) && (*_begin1 == *_begin2))
		{
			++_begin1;
			++_begin2;
		}
		return ((_begin1 == _end1) && (_begin2 == _end2));
	}

	template<typename _Tp, typename Alloc>
	inline bool
		operator<(const list<_Tp, Alloc>&_x, const list<_Tp, Alloc>&_y)
	{
		return lexicographical_compare(_x.begin(), _x.end(), _y.begin(), _y.end());
	}

	template<typename _Tp, typename Alloc>
	inline bool
		operator!=(const list<_Tp, Alloc>&_x, const list<_Tp, Alloc>&_y)
	{
		return !(_x == _y);
	}

	template<typename _Tp, typename Alloc>
	inline bool
		operator>(const list<_Tp, Alloc>&_x, const list<_Tp, Alloc>&_y)
	{
		return _y < _x;
	}

	template<typename _Tp, typename Alloc>
	inline bool
		operator<=(const list<_Tp, Alloc>&_x, const list<_Tp, Alloc>&_y)
	{
		return !(_y < _x);
	}

	template<typename _Tp, typename Alloc>
	inline bool
		operator>=(const list<_Tp, Alloc>&_x, const list<_Tp, Alloc>&_y)
	{
		return !(_x < _y);
	}

	template<typename _Tp, typename Alloc>
	inline void
		swap(const list<_Tp, Alloc>&_x, const list<_Tp, Alloc>&_y)
	{
		_x.swap(_y);
	}

	template<typename _Tp, typename Alloc>
	inline void
		list<_Tp, Alloc>::insert(iterator __position, const _Tp * __first,
			const _Tp * __last)
	{
		for (; __first != __last; ++__first)
			insert(__position, *__first);
	}

	template<typename _Tp, typename Alloc>
	inline void
		list<_Tp, Alloc>::insert(iterator __position,
			constIterator __first, constIterator __last)
	{
		for (; __first != __last; ++__first)
			insert(__position, *__first);
	}

	template<typename _Tp, typename Alloc>
	inline void
		list<_Tp, Alloc>::M_fillInsert(iterator _pos, sizeType _n, const _Tp & _x)
	{
		for (; _n > 0; --_n)
		{
			insert(_pos, _x);
		}
	}

	template<typename _Tp, typename Alloc>
	typename list<_Tp, Alloc>::iterator
		list<_Tp, Alloc>::erase(iterator first, iterator last)
	{
		while (first != last)
		{
			erase(first++);
		}
		return last;
	}

	template<typename _Tp, typename Alloc>
	inline void
		list<_Tp, Alloc>::resize(sizeType _newSize, const _Tp & _x)
	{
		iterator tmp = begin();
		sizeType len = 0;
		for (; (tmp != end()) && (len < _newSize); ++tmp, ++len)
		{
			//do nothing
		}

		if (len == _newSize)
		{
			erase(tmp, end());
		}
		else
		{
			insert(end(), _newSize - len, _x);
		}
	}
	template<typename _Tp, typename Alloc>
	inline list<_Tp, Alloc>&
		list<_Tp, Alloc>::operator=(const list<_Tp, Alloc>& _x)
	{
		if (this != _x)
		{
			iterator first1 = begin();
			iterator last1 = end();
			constIterator first2 = _x.begin();
			constIterator last2 = _x.end();

			while (first1 != last1 && first2 != last2)
			{
				*first1++ = *first2++;
			}
			if (first1 == last1)
			{
				erase(first1, last1);
			}
			else
			{
				insert(last1, first2, last2);
			}
		}
		return *this;
	}

	template<typename _Tp, typename Alloc>
	inline void
		list<_Tp, Alloc>::M_fillAssign(sizeType _n, const _Tp & _val)
	{
		iterator first = begin();
		for (; first != end() && _n > 0; ++first, --_n)
		{
			*first = _val;
		}
		if (_n > 0)
		{
			insert(end(), _n, _val);
		}
		else
		{
			erase(first, end());
		}
	}

	template<typename _Tp, typename Alloc>
	inline void
		list<_Tp, Alloc>::remove(const _Tp & _value)
	{
		iterator first = begin();
		iterator last = end();
		while (first != last)
		{
			iterator next = first;
			++next;
			if (*first == _value)
			{
				erase(first);
			}
			first = next;
		}
	}

	template<typename _Tp, typename Alloc>
	inline void
		list<_Tp, Alloc>::unique()
	{
		iterator first = begin();
		iterator last = end();
		if (first == last)
		{
			return;
		}
		iterator next = first;
		while (++next != last)
		{
			if (*first == *last)
			{
				erase(next);
			}
			else
			{
				first = next;
			}
			next = first;
		}
	}

	template<typename _Tp, typename Alloc>
	inline void
		list<_Tp, Alloc>::merge(list & _x)
	{
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = _x.begin();
		iterator last2 = _x.end();
		while (first1 != last1 && first2 != last2)
		{
			if (*first2 < *first1)
			{
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
			}
			else
			{
				++first1;
			}
			if (first2 != last2)
			{
				transfer(last1, first2, last2);
			}
		}
	}

	template<typename _Tp, typename Alloc>
	inline void
		list<_Tp, Alloc>::reverse()
	{
		ListNodeBase* tmp = this->M_node;
		do {
			std::swap(tmp->M_next, tmp->M_prev);
			tmp = tmp->M_prev;
		} while (tmp != this->M_node);
	}

	template<typename _Tp, typename Alloc>
	inline void
		list<_Tp, Alloc>::sort()
	{
		if ((this->M_node->M_next != this->M_node) && (this->M_node->M_next->M_next != this->M_node))
		{
			list<_Tp, Alloc> carry;
			list<_Tp, Alloc> counter[64];
			int fill = 0;
			while (!empty())
			{
				carry.splice(carry.begin(), *this, begin());
				int i = 0;

				while (i < fill && !counter[i].empty())
				{
					counter[i].merge(carry);
					carry.swap(counter[i++]);
				}
				carry.swap(counter[i]);
				if (i == fill)
				{
					++fill;
				}
			}
			for (int i = 1; i < fill; ++i)
			{
				counter[i].merge(counter[i - 1]);
			}
			swap(counter[fill - 1]);
		}
	}
}

#endif // !_JDG_STL_LIST_H

