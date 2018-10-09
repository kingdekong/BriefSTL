#ifndef _TESTFILE_H
#define _TESTFILE_H
#include "Allocator.h"

namespace JDG_TestAlloc
{
	void testAlloc()
	{
		//分配60（15*4）字节大小的空间，使用__second_alloc_template
		BriefSTL::alloc::allocate(15 % 128 * sizeof(int));

		//分配140（35*4）字节大小的空间，使用__first_alloc_template
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
			//上面的代码中，new Foo背后包括了两个动作：1) 配置内存 2) 构造对象
			//           delete Foo背后包括了两个动作：1) 析构对象 2) 释放内存

			// 下面两行代码可做模仿new操作
			auto p = BriefSTL::allocator<int>::allocate();
			BriefSTL::allocator<int>::construct(p, i);

			// 下面两行代码可做模仿delete操作
			BriefSTL::allocator<int>::destroy(p);
			BriefSTL::allocator<int>::deallocate(p);
		}
	}
}

#endif // !_TESTFILE_H

