#ifndef _TESTFILE_H
#define _TESTFILE_H
#include <string>

#include "Allocator.h"
#include "Uninitialized.h"
#include "Iterator.h"
#include "Vector.h"
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

#endif // !_TESTFILE_H

