#ifndef _TESTFILE_H
#define _TESTFILE_H
#include "Allocator.h"

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

#endif // !_TESTFILE_H

