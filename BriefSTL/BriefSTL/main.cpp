#include <iostream>
#include <memory>

#include "Alloc.h"
using namespace std;

namespace JDG_TestAlloc
{
	void testAlloc()
	{
		//����60��15*4���ֽڴ�С�Ŀռ䣬ʹ��__second_alloc_template
		BriefSTL::alloc::allocate(15 % 128 * sizeof(int));

		//����140��35*4���ֽڴ�С�Ŀռ䣬ʹ��__first_alloc_template
		BriefSTL::alloc::allocate(35 % 128 * sizeof(int));
	}
}

int main() {
	cout << "Holle World 01 " << endl;
	JDG_TestAlloc::testAlloc();
	cout << "Holle World 02 " << endl;

	system("pause");
	return 0;
}