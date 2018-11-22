#ifndef _TESTFILE_H
#define _TESTFILE_H
#include <string>

#include "Allocator.h"
#include "Uninitialized.h"
#include "Iterator.h"
#include "Vector.h"
#include "list.h"
namespace JDG_TestAlloc
{
	void testAlloc()
	{
		//����60��15*4���ֽڴ�С�Ŀռ䣬ʹ��__second_alloc_template
		BriefSTL::alloc::allocate(15 % 128 * sizeof(int));

		//����140��35*4���ֽڴ�С�Ŀռ䣬ʹ��__first_alloc_template
		BriefSTL::alloc::allocate(30 % 128 * sizeof(int));
	}
}

namespace JDG_TestAllocator
{
	void testAllocator()
	{
		for (int i = 1; i != 1000; ++i)
		{
			// class Foo{...}
			// Foo* pf=new Foo;
			// delete pf;
			//����Ĵ����У�new Foo�������������������1) �����ڴ� 2) �������
			//           delete Foo�������������������1) �������� 2) �ͷ��ڴ�

			// �������д������ģ��new����
			auto p = BriefSTL::allocator<int>::allocate();
			BriefSTL::allocator<int>::construct(p, i);

			// �������д������ģ��delete����
			BriefSTL::allocator<int>::destroy(p);
			BriefSTL::allocator<int>::deallocate(p);
		}
	}
}

namespace JDG_TestIteratorTraits
{
	void testIteratorTraits()
	{

	}
}

namespace JDG_TestUninitialized
{
	void testuninitialized_fill_n()
	{
		int array[10];
		BriefSTL::uninitialized_fill_n(array, 10, 1);
		for (auto n : array)
		{
			std::cout << n << std::endl;
		}
	}
}


namespace JDG_TestVector
{
	BriefSTL::vector <int> v1(5);

	void testVector1()
	{
		for (BriefSTL::vector<int>::iterator it = v1.begin(); it != v1.end(); ++it)
		{
			std::cout << "testVector1 *it: " << *it << std::endl;
		}
		std::cout << std::endl;

	}
	void testVector2()
	{
		BriefSTL::vector<int> v2(5, 100);

		for (BriefSTL::vector<int>::iterator it = v2.begin(); it != v2.end(); ++it)
		{
			std::cout << "testVector2 *it: " << *it << std::endl;
		}
	}

	void testVector3()
	{
		std::cout << std::endl;
		BriefSTL::vector<std::string> v2(5, "jdg");

		for (BriefSTL::vector<std::string>::iterator it = v2.begin(); it != v2.end(); ++it)
		{
			std::cout << "testVector3 *it: " << *it << std::endl;
		}
	}
}

namespace JDG_TestList
{
	void testList()
	{
		// constructors used in the same order as described above:
		BriefSTL::list<int> first;                                // empty list of ints
		BriefSTL::list<int> second(4, 100);                       // four ints with value 100
		BriefSTL::list<int> third(second.begin(), second.end());  // iterating through second
		BriefSTL::list<int> fourth(third);                       // a copy of third

		// the iterator constructor can also be used to construct from arrays:
		int myints[] = { 16,2,77,29 };
		BriefSTL::list<int> fifth(myints, myints + sizeof(myints) / sizeof(int));

		std::cout << "The contents of third are: ";
		for (BriefSTL::list<int>::iterator it = third.begin(); it != third.end(); it++)
			std::cout << *it << ' ';

		std::cout << '\n';

		std::cout << "The contents of fifth are: ";
		for (BriefSTL::list<int>::iterator it = fifth.begin(); it != fifth.end(); it++)
			std::cout << *it << ' ';

		std::cout << '\n';
	}
	void testListInsert()
	{
		BriefSTL::list<int> mylist;
		BriefSTL::list<int>::iterator it;

		// set some initial values:
		for (int i = 1; i <= 5; ++i) mylist.pushBack(i); // 1 2 3 4 5

		it = mylist.begin();
		++it;       // it points now to number 2           ^

		mylist.insert(it, 10);                        // 1 10 2 3 4 5

		// "it" still points to number 2                      ^
		mylist.insert(it, 2, 20);                      // 1 10 20 20 2 3 4 5

		--it;       // it points now to the second 20            ^

		BriefSTL::vector<int> myvector(2, 30);
		mylist.insert(it, myvector.begin(), myvector.end());
		// 1 10 20 30 30 20 2 3 4 5
		//               ^
		std::cout << "mylist contains:";
		for (it = mylist.begin(); it != mylist.end(); ++it)
			std::cout << ' ' << *it;
		std::cout << '\n';
	}

	void testListRemove()

	{
		int myints[] = { 17,89,7,14 };
		BriefSTL::list<int> mylist(myints, myints + 4);

		mylist.remove(89);

		std::cout << "mylist contains:";
		for (BriefSTL::list<int>::iterator it = mylist.begin(); it != mylist.end(); ++it)
			std::cout << ' ' << *it;
		std::cout << '\n';


	}
}

#endif // !_TESTFILE_H

