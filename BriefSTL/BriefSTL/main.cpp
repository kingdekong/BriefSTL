#include <iostream>

#include "TestFile.h"

using namespace std;

int main() 
{
	//cout << "Holle World 01 " << endl;
	//JDG_TestAlloc::testAlloc();
	//cout << "Holle World 02 " << endl;

	//JDG_TestAllocator::testAllocator();

	//cout << "Holle World 03 " << endl;

	//JDG_TestUninitialized::testuninitialized_fill_n();

	//JDG_TestVector::testVector1();
	//JDG_TestVector::testVector2();
	//JDG_TestVector::testVector3();

	//JDG_TestList::testList();
	JDG_TestList::testListInsert();
	JDG_TestList::testListRemove();

	system("pause");
	return 0;
}