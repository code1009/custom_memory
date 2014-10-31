/////////////////////////////////////////////////////////////////////////////
//
//  Test code
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <stdio.h>

#include "cx/memory/custom_memory_operator.hpp"
#include "cx/memory/custom_memory_api.hpp"

namespace cx
{
namespace memory
{
namespace custom_memory_operator_test
{

//===========================================================================
class base : public cx::memory::custom_memory_operator
{
public:
	int _value;

public:
	explicit base(int v) : _value(v)
	{
		printf("constructor:  base(%d)\r\n", _value);
	}

	virtual ~base()
	{
		printf("destructor : ~base(%d)\r\n", _value);
	}
};

//===========================================================================
class derived1 : public base
{
public:
	explicit derived1(int v) : base(v)
	{
		printf("constructor:  derived1(%d)\r\n", _value);
	}

	explicit derived1() : base(2)
	{
		printf("constructor:  derived1(%d)\r\n", _value);
	}

	virtual ~derived1()
	{
		printf("destructor : ~derived1(%d)\r\n", _value);
	}

public:
	void test_memory_allocation (void)
	{
		char* p;

		p = new char();
		delete p;

		p = new char[2];
		delete[] p;
	}
};

//===========================================================================
void test (void)
{
	cx::memory::custom_memory_create();

	{
		printf ("-----------------------------------------------------------------------------\r\n");
		printf ("single-object \r\n");
		printf ("-----------------------------------------------------------------------------\r\n");

		/*
		할당크기 8 bytes = 
		+ virtual function table pointer = 4bytes (32 bit compile 시)
		+ member 변수 _value int size    = 4bytes           
		*/

		base* p = new derived1(1);
		delete p;

		printf ("\r\n");
	}

	{
		printf ("-----------------------------------------------------------------------------\r\n");
		printf ("array-object \r\n");
		printf ("-----------------------------------------------------------------------------\r\n");

		/*
		할당크기 20 bytes = 
		+ compiler array 할당 count 값의 memory 크기    = 4bytes (compiler 따라 다름)
		+ array [0] 번째 virtual function table pointer = 4bytes (32bit compile 시)
		+ array [0] 번째 member 변수 _value int size    = 4bytes           
		+ array [1] 번째 virtual function table pointer = 4bytes (32bit compile 시)
		+ array [1] 번째 member 변수 _value int size    = 4bytes           
	
		전체 하나의 메모리가 할당되고 각각의 인스턴스에 대해서 생성자가 호출
		*/

		base* p = new derived1[2]();
		delete[] p;

		printf ("\r\n");
	}


	{
		printf ("-----------------------------------------------------------------------------\r\n");
		printf ("allocate memory in member function \r\n");
		printf ("-----------------------------------------------------------------------------\r\n");

		/*
		type이 상속으로 연결되지 않는 memory 할당에 대해서 global new/delete를 호출한다.
		*/
		derived1 o(3);

		o.test_memory_allocation();

		printf ("\r\n");
	}

	cx::memory::custom_memory_destroy();
}

}
}
} 

//===========================================================================
extern "C" int test_custom_memory_operator (void)
{
	printf ("/////////////////////////////////////////////////////////////////////////////\r\n");
	printf ("test_custom_memory_operator ()\r\n");
	printf ("/////////////////////////////////////////////////////////////////////////////\r\n");
	printf ("\r\n");

	cx::memory::custom_memory_operator_test::test();

	printf ("\r\n");
	printf ("\r\n");
	printf ("\r\n");

	return 0;
}

/*
[실행결과]

/////////////////////////////////////////////////////////////////////////////
test_custom_memory_operator ()
/////////////////////////////////////////////////////////////////////////////

-----------------------------------------------------------------------------
single-object
-----------------------------------------------------------------------------
[overload] new(8)
constructor:  base(1)
constructor:  derived1(1)
destructor : ~derived1(1)
destructor : ~base(1)
[overload] delete(0136E9B0)

-----------------------------------------------------------------------------
array-object
-----------------------------------------------------------------------------
[overload] new[](20)
constructor:  base(2)
constructor:  derived1(2)
constructor:  base(2)
constructor:  derived1(2)
destructor : ~derived1(2)
destructor : ~base(2)
destructor : ~derived1(2)
destructor : ~base(2)
[overload] delete[](0138E9E0)

-----------------------------------------------------------------------------
allocate memory in member function
-----------------------------------------------------------------------------
constructor:  base(3)
constructor:  derived1(3)

destructor : ~derived1(3)
destructor : ~base(3)



계속하려면 아무 키나 누르십시오 . . .

*/
