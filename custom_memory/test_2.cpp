/////////////////////////////////////////////////////////////////////////////
//
//  Test code
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <stdio.h>

#include <map>
#include <unordered_map>
#include <vector>

#include "cx/memory/custom_memory_stl_allocator.hpp"
#include "cx/memory/elapsed_timer.hpp"

namespace cx
{
namespace memory
{
namespace custom_memory_stl_allocator_test
{

//===========================================================================
static int _count = 10000;

//===========================================================================
template <typename T>
void test_1 (void)
{
	typename T container;
	int i;
	int count;

	count = _count;
	for (i=0;i<count;i++)
	{
		container.push_back (i*100);
	}
}

//===========================================================================
template <typename T>
void test_2 (void)
{
	typename T container;
	int i;
	int count;

	count = _count;
	container.reserve(_count);
	for (i=0;i<count;i++)
	{
		container.push_back (i*100);
	}
}

//===========================================================================
template <typename T>
void test_3 (void)
{
	typename T container;
	int i;
	int count;

	count = _count;
	for (i=0;i<count;i++)
	{
		container[i] = i*100;
	}
}

//===========================================================================
void test(void)
{
	typedef std::vector       <int>     std_vector;
	typedef std::list         <int>     std_list;
	typedef std::map          <int,int> std_map;
	typedef std::unordered_map<int,int> std_unordered_map;

	typedef std::vector       <int,                                          cx::memory::custom_memory_stl_allocator<int>                         > std_vector_a;
	typedef std::list         <int,                                          cx::memory::custom_memory_stl_allocator<int>                         > std_list_a;
	typedef std::map          <int, int, std::less<int>,                     cx::memory::custom_memory_stl_allocator< std::pair<const int, int> > > std_map_a;
	typedef std::unordered_map<int, int, std::hash<int>, std::equal_to<int>, cx::memory::custom_memory_stl_allocator< std::pair<const int, int> > > std_unordered_map_a;

	typedef std_vector          container_1;
	typedef std_list            container_2;
	typedef std_map             container_3;
	typedef std_unordered_map   container_4;

	typedef std_vector_a        container_1a;
	typedef std_list_a          container_2a;
	typedef std_map_a           container_3a;
	typedef std_unordered_map_a container_4a;


	windows_elapsed_timer t;


	printf ("-----------------------------------------------------------------------------\r\n");
	{
		elapsed_time_printer printer(&t,"custom_memory_create()");

		cx::memory::custom_memory_create();
	}
	
	printf ("-----------------------------------------------------------------------------\r\n");
	{
		elapsed_time_printer printer(&t,"vector");

		test_1<container_1>();
	}
	{
		elapsed_time_printer printer(&t,"vector(custom_memory_stl_allocator)");

		test_1<container_1a>();
	}
	
	printf ("-----------------------------------------------------------------------------\r\n");
	{
		elapsed_time_printer printer(&t,"vector-reserve");

		test_2<container_1>();
	}
	{
		elapsed_time_printer printer(&t,"vector-reserve(custom_memory_stl_allocator)");

		test_2<container_1a>();
	}

	printf ("-----------------------------------------------------------------------------\r\n");
	{
		elapsed_time_printer printer(&t,"list");

		test_1<container_2>();
	}
	{
		elapsed_time_printer printer(&t,"list(custom_memory_stl_allocator)");

		test_1<container_2a>();
	}	

	printf ("-----------------------------------------------------------------------------\r\n");
	{
		elapsed_time_printer printer(&t,"map");

		test_3<container_3>();
	}
	{
		elapsed_time_printer printer(&t,"map(custom_memory_stl_allocator)");

		test_3<container_3a>();
	}
	
	printf ("-----------------------------------------------------------------------------\r\n");
	{
		elapsed_time_printer printer(&t,"unordered_map");

		test_3<container_4>();
	}
	{
		elapsed_time_printer printer(&t,"unordered_map(custom_memory_stl_allocator)");

		test_3<container_4a>();
	}

	//--------------------------------------------------------------------------
	cx::memory::custom_memory_dump();
	//--------------------------------------------------------------------------

	printf ("-----------------------------------------------------------------------------\r\n");
	{
		elapsed_time_printer printer(&t,"custom_memory_destroy()");

		cx::memory::custom_memory_destroy();
	}
}

}
}
}

//===========================================================================
extern "C" int test_custom_memory_stl_allocator (void)
{
	printf ("/////////////////////////////////////////////////////////////////////////////\r\n");
	printf ("test_custom_memory_stl_allocator()\r\n");
	printf ("/////////////////////////////////////////////////////////////////////////////\r\n");
	printf ("\r\n");

	try
	{
		cx::memory::custom_memory_stl_allocator_test::test();
	}
	catch(std::bad_alloc)
	{
		printf ("\r\ncaught std::bad_alloc !!! \r\n");
	}
	catch(...)
	{
		printf ("\r\ncaught unkown exception !!! \r\n");
	}

	printf ("\r\n");
	printf ("\r\n");
	printf ("\r\n");

	return 0;
}

/*

[실행결과]
/////////////////////////////////////////////////////////////////////////////
test_custom_memory_stl_allocator()
/////////////////////////////////////////////////////////////////////////////

-----------------------------------------------------------------------------
elapsed_time = 2.201459 ms : custom_memory_create()
-----------------------------------------------------------------------------
elapsed_time = 0.108277 ms : vector
elapsed_time = 0.067224 ms : vector(custom_memory_stl_allocator)
-----------------------------------------------------------------------------
elapsed_time = 0.021040 ms : vector-reserve
elapsed_time = 0.020013 ms : vector-reserve(custom_memory_stl_allocator)
-----------------------------------------------------------------------------
elapsed_time = 1.031453 ms : list
elapsed_time = 0.320725 ms : list(custom_memory_stl_allocator)
-----------------------------------------------------------------------------
elapsed_time = 2.250209 ms : map
elapsed_time = 1.867904 ms : map(custom_memory_stl_allocator)
-----------------------------------------------------------------------------
elapsed_time = 2.205564 ms : unordered_map
elapsed_time = 1.372191 ms : unordered_map(custom_memory_stl_allocator)
-----------------------------------------------------------------------------
elapsed_time = 0.827215 ms : custom_memory_destroy()



계속하려면 아무 키나 누르십시오 . . .
*/
