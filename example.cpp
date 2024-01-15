#include <iostream>
#include "sehlist.h"

class Object
{
public:
	QWORD base, aComponent, bComponent;

	Object(QWORD _base) : base(_base), aComponent(_base + 0x10), bComponent(_base + 0x20)
	{
		//
	}
};

int main()
{
	SEHList<int> intList(128);//max index of this list is 128
	for (int i = 1; i <= 10; i++)
	{
		intList.add(i);
	}

	for (int value : intList)
	{
		std::cout << "intList : " << value << std::endl;
	}

	SEHList<Object> objectList(64);//max index of this list is 64
	for (QWORD i = 0; i < 0x1000; i += 0x100)
	{
		Object object(i);
		objectList.add(object);
	}

	for (const auto object : objectList)
	{
		std::cout << std::hex << "object: " << object.base << std::endl;
	}

	SEHList<Object> codeCaveList(0xA000000, 0xF0000);//list using codeCave
	//codeCave's protect must be PAGE_READWRITE.
}
