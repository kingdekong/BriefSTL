#include <iostream>
#include <memory>

#include "TestFile.h"
//#include "Alloc.h"
using namespace std;


int main() {
	cout << "Holle World 01 " << endl;
	JDG_TestAlloc::testAlloc();
	cout << "Holle World 02 " << endl;

	JDG_TestAllocator::testAllocator();

	cout << "Holle World 03 " << endl;

	system("pause");
	return 0;
}