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
		�Ҵ�ũ�� 8 bytes = 
		+ virtual function table pointer = 4bytes (32 bit compile ��)
		+ member ���� _value int size    = 4bytes           
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
		�Ҵ�ũ�� 20 bytes = 
		+ compiler array �Ҵ� count ���� memory ũ��    = 4bytes (compiler ���� �ٸ�)
		+ array [0] ��° virtual function table pointer = 4bytes (32bit compile ��)
		+ array [0] ��° member ���� _value int size    = 4bytes           
		+ array [1] ��° virtual function table pointer = 4bytes (32bit compile ��)
		+ array [1] ��° member ���� _value int size    = 4bytes           
	
		��ü �ϳ��� �޸𸮰� �Ҵ�ǰ� ������ �ν��Ͻ��� ���ؼ� �����ڰ� ȣ��
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
		type�� ������� ������� �ʴ� memory �Ҵ翡 ���ؼ� global new/delete�� ȣ���Ѵ�.
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
[������]

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



����Ϸ��� �ƹ� Ű�� �����ʽÿ� . . .

*/
