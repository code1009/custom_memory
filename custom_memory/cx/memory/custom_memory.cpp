/////////////////////////////////////////////////////////////////////////////
//
// File: custom_memory.cpp
//
// Created by MOON, Eui-kwon.
// Created on Jul-21st, 2009.
//
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*

Referance.

# Windows LFH(Low-Fragmentation Heap)
- Win2k SP4, WinXP, WinServer 2k3부터 적용되기 시작해서, 
WinVISTA부터는 시스템에서 자동으로 low-fragmentation heap(LFH)가 적용
+-------------------+-------------------+-------------------+
| Buckets           | Granularity       | Range             |
+-------------------+-------------------+-------------------+
|   1~32            |   8               |    1~256          |
|  33~48            |  16               |  257~512          |
|  49~64            |  32               |  513~1024         |
|  65~80            |  64               | 1025~2048         |
|  81~96            | 128               | 2049~4096         |
|  97~112           | 256               | 4097~8192         |
| 113~128           | 512               | 8193~16384        |
+-------------------+-------------------+-------------------+

- Buckets 8bytes의 32개 구간
  8,  16,  24,  32,  40,  48,  56,  64,
 72,  80,  88,  96, 104, 112, 120, 128,
136, 144, 152, 160, 168, 176, 184, 192,
200, 208, 216, 224, 232, 240, 248, 256



# intel TBB(Threading-Building Blocks)
  multithread 상에서 memory 할당 library



# Library
boost::details::pool::singleton_default<T>
Loki::LokiAllocator



# URL
http://www.codeguru.com/cpp/cpp/cpp_mfc/stl/article.php/c4079/Allocators-STL.htm
http://gee.cs.oswego.edu/dl/html/malloc.html
http://sweeper.egloos.com/2843922
http://msdn.microsoft.com/ko-kr/library/aa366750
https://www.threadingbuildingblocks.org/

*/



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*
[0000|0000][0000|0000][0000|0000][0000|1xxx]
 ^^^^ ^^^^  ^^^^ ^^^^  ^^^^ ^^^^  ^^^^ ^^^^
 |||| ||||  |||| ||||  |||| ||||  |||| ||||
 |||| ||||  |||| ||||  |||| ||||  |||| |||+- 2^0  = 1           
 |||| ||||  |||| ||||  |||| ||||  |||| ||+-- 2^1  = 2           
 |||| ||||  |||| ||||  |||| ||||  |||| |+--- 2^2  = 4           
 |||| ||||  |||| ||||  |||| ||||  |||| +---- 2^3  = 8           
 |||| ||||  |||| ||||  |||| ||||  ||||                          
 |||| ||||  |||| ||||  |||| ||||  |||+------ 2^4  = 16          
 |||| ||||  |||| ||||  |||| ||||  ||+------- 2^5  = 32          
 |||| ||||  |||| ||||  |||| ||||  |+-------- 2^6  = 64          
 |||| ||||  |||| ||||  |||| ||||  +--------- 2^7  = 128         
 |||| ||||  |||| ||||  |||| ||||                                
 |||| ||||  |||| ||||  |||| |||+------------ 2^8  = 256         
 |||| ||||  |||| ||||  |||| ||+------------- 2^9  = 512         
 |||| ||||  |||| ||||  |||| |+-------------- 2^10 = 1024        
 |||| ||||  |||| ||||  |||| +--------------- 2^11 = 2048        
 |||| ||||  |||| ||||  ||||                                     
 |||| ||||  |||| ||||  |||+----------------- 2^12 = 4096        
 |||| ||||  |||| ||||  ||+------------------ 2^13 = 8192        
 |||| ||||  |||| ||||  |+------------------- 2^14 = 16384       
 |||| ||||  |||| ||||  +-------------------- 2^15 = 32768       
 |||| ||||  |||| ||||                                           
 |||| ||||  |||| |||+----------------------- 2^16 = 65536       
 |||| ||||  |||| ||+------------------------ 2^17 = 131072       
 |||| ||||  |||| |+------------------------- 2^18 = 262144      
 |||| ||||  |||| +-------------------------- 2^19 = 524288      
 |||| ||||  ||||                                                
 |||| ||||  |||+---------------------------- 2^20 = 1048576     
 |||| ||||  ||+----------------------------- 2^21 = 2097152     
 |||| ||||  |+------------------------------ 2^22 = 4194304     
 |||| ||||  +------------------------------- 2^23 = 8388608     
 |||| ||||                                                      
 |||| |||+---------------------------------- 2^24 = 16777216    
 |||| ||+----------------------------------- 2^25 = 33554432     
 |||| |+------------------------------------ 2^26 = 67108864    
 |||| +------------------------------------- 2^27 = 134217728   
 ||||                                                           
 |||+--------------------------------------- 2^28 = 268435456   
 ||+---------------------------------------- 2^29 = 536870912   
 |+----------------------------------------- 2^30 = 1073741824  
 +------------------------------------------ 2^31 = 2147483648  
                                             2^32 = 4294967296  
*/



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <stdio.h>

#include <memory>

#if defined(WIN32)
#include <atlbase.h>
#endif


#include "custom_memory.hpp"



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{



//===========================================================================
namespace memory
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void custom_memory_trace_null (const char* lpFormatStr, ...)
{
}

#if defined(WIN32)
void custom_memory_trace (const char* lpFormatStr, ...)
{
	char    szBuffer[8192];
	va_list args;

	va_start (args, lpFormatStr);
	vsprintf_s (szBuffer, lpFormatStr, args);
	va_end   (args);      

	OutputDebugStringA (szBuffer);	
}
#else
#define custom_memory_trace printf
#endif



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define MODULE_TRACE custom_memory_trace
//#define MODULE_TRACE custom_memory_trace_null



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
custom_memory_growing_heap::custom_memory_growing_heap ()
{
}

custom_memory_growing_heap::~custom_memory_growing_heap ()
{
}

//===========================================================================
bool custom_memory_growing_heap::create (void)
{
	if (false==_heap.create ())
	{
		return false;
	}

	return true;
}

void custom_memory_growing_heap::destroy (void)
{
	_heap.destroy ();
}

void custom_memory_growing_heap::dump (void)
{
	MODULE_TRACE ("*****************************************************************************\r\n");
	MODULE_TRACE ("growing heap \r\n");
	MODULE_TRACE ("*****************************************************************************\r\n");
	_heap.dump ();
	MODULE_TRACE ( " \r\n" );
	MODULE_TRACE ( " \r\n" );
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
custom_memory_fixed_heap::custom_memory_fixed_heap ():
	_heap_memory(0),
	_heap_size  (0)
{
}

custom_memory_fixed_heap::~custom_memory_fixed_heap ()
{
}

//===========================================================================
bool custom_memory_fixed_heap::create (void)
{
	_heap_size   = 1024*1024*16;
	_heap_memory = :: malloc (_heap_size);

	if (0==_heap_memory)
	{
		return false;
	}

	if (false==_heap_allocator._mp.create (_heap_memory, _heap_size))
	{
		return false;
	}

	if (false==_heap.create (&_heap_allocator,&_heap_allocator, 1*1024*1024))
	{
		return false;
	}

	return true;
}

void custom_memory_fixed_heap::destroy (void)
{
#if 0
	// 불필요한 code
	_heap.destroy ();
#endif

	if (_heap_memory)
	{
		:: free(_heap_memory);
		_heap_memory = 0;
	}
}

void custom_memory_fixed_heap::dump (void)
{
	MODULE_TRACE ("*****************************************************************************\r\n");
	MODULE_TRACE ("fixed heap allocator\r\n");
	MODULE_TRACE ("*****************************************************************************\r\n");
	dump_memory_pool (&_heap_allocator._mp);
	MODULE_TRACE ( " \r\n" );
	MODULE_TRACE ( " \r\n" );

	MODULE_TRACE ("*****************************************************************************\r\n");
	MODULE_TRACE ("fixed heap \r\n");
	MODULE_TRACE ("*****************************************************************************\r\n");
	_heap.dump();
	MODULE_TRACE ( " \r\n" );
	MODULE_TRACE ( " \r\n" );
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
custom_memory::custom_memory()
{
}

custom_memory::~custom_memory()
{
}

//===========================================================================
bool custom_memory::create (void)
{
	size_t internal_heap_ss_size;

	internal_heap_ss_size = 0;
	if ( internal_heap_ss_size < sizeof(memory_segment     )){ internal_heap_ss_size=sizeof(memory_segment     ); }
	if ( internal_heap_ss_size < sizeof(memory_allocator_ss)){ internal_heap_ss_size=sizeof(memory_allocator_ss); }
	if ( internal_heap_ss_size < sizeof(memory_allocator_mp)){ internal_heap_ss_size=sizeof(memory_allocator_mp); }

	//-----------------------------------------------------------------------
	if (false==_heap.create ())
	{
		return false;
	}

	//-----------------------------------------------------------------------
	if (false==_internal_heap.create (&_heap._heap, &_heap._heap, 64*1024, internal_heap_ss_size))
	{
		return false;
	}

	if (false==_external_heap.create (&_heap._heap, &_heap._heap, 64*1024))
	{
		return false;
	}

	//-----------------------------------------------------------------------
	if (false==_mp.create (&_internal_heap, &_external_heap, 64*1024))
	{
		return false;
	}

	//-----------------------------------------------------------------------
	typedef struct _ss_information_t
	{
		size_t grow   ;
		size_t storage;
	}
	ss_information_t;

	ss_information_t ssi[8] =
	{
		{1024*1024,    4},
		{1024*1024,    8},
		{1024*1024,   16},
		{1024*1024,   32},
		{1024*1024,   64},
//		{1024*1024,  128},
//		{1024*1024,  256},
		{1024*1024,  512},
		{1024*1024, 1024},
//		{1024*1024, 2048},
		{1024*1024, 4096}
//		{1024*1024, 8192}
	};
	_ss_count = sizeof(ssi)/sizeof(ss_information_t);

	//-----------------------------------------------------------------------
	size_t i;

	for (i=0; i<_ss_count; i++)
	{
		if (false==_ss[i].create (&_internal_heap, &_external_heap, ssi[i].grow, ssi[i].storage))
		{
			return false;
		}
	}

#if 1
	// 미리 메모리 할당
	for (i=0; i<_ss_count; i++)
	{
		_ss[i].grow();
	}
#endif


#if CUSTOM_MEMORY__USE_REPORT
	_report.reset();
#endif

	return true;
}

bool custom_memory::destroy(void)
{
	//--------------------------------------------------------------------------
#if 0
	// 불필요한 code
	size_t i;

	for (i=0; i<_ss_count; i++)
	{
		_ss[i].destroy ();
	}

	_mp.destroy ();
	
	_external_heap.destroy ();
	_internal_heap.destroy ();
#endif

	_heap.destroy ();

	return true;
}

void custom_memory::dump(void)
{
	MODULE_TRACE ("*****************************************************************************\r\n");
	MODULE_TRACE ("custom_memory::dump() \r\n");
	MODULE_TRACE ("*****************************************************************************\r\n");
	MODULE_TRACE ("sizeof(memory_segment     ) = %d \r\n", sizeof(memory_segment     ));
	MODULE_TRACE ("sizeof(memory_allocator_ss) = %d \r\n", sizeof(memory_allocator_ss));
	MODULE_TRACE ("sizeof(memory_allocator_mp) = %d \r\n", sizeof(memory_allocator_mp));
	MODULE_TRACE ( " \r\n" );
	MODULE_TRACE ( " \r\n" );

	_heap.dump ();
	
	MODULE_TRACE ("*****************************************************************************\r\n");
	MODULE_TRACE ("external heap \r\n");
	MODULE_TRACE ("*****************************************************************************\r\n");
	_external_heap.dump();
	MODULE_TRACE ( " \r\n" );
	MODULE_TRACE ( " \r\n" );

	MODULE_TRACE ("*****************************************************************************\r\n");
	MODULE_TRACE ("internal heap \r\n");
	MODULE_TRACE ("*****************************************************************************\r\n");
	_internal_heap.dump();
	MODULE_TRACE ( " \r\n" );
	MODULE_TRACE ( " \r\n" );

	MODULE_TRACE ("*****************************************************************************\r\n");
	MODULE_TRACE ("mp \r\n");
	MODULE_TRACE ("*****************************************************************************\r\n");
	_mp.dump();
	MODULE_TRACE ( " \r\n" );
	MODULE_TRACE ( " \r\n" );

	size_t i;

	for (i=0; i<_ss_count; i++)
	{
		MODULE_TRACE ("*****************************************************************************\r\n");
		MODULE_TRACE ("ss[%d] \r\n", i);
		MODULE_TRACE ("*****************************************************************************\r\n");
		_ss[i].dump ();
		MODULE_TRACE ( " \r\n" );
		MODULE_TRACE ( " \r\n" );
	}

#if CUSTOM_MEMORY__USE_REPORT
	_report.report();
#endif
}

//===========================================================================
void* custom_memory::_allocate (uint_t tag, size_t size)
{	
	void* pointer;

	size_t i;

	for (i=0; i<_ss_count; i++)
	{
		if ( size <= _ss[i]._storage_size )
		{
			pointer = _ss[i].allocate (size);

#if CUSTOM_MEMORY__USE_REPORT
			_report.allocate (pointer, _ss[i]._storage_size, size, tag);
#endif

			return pointer;
		}
	}

	pointer = _mp.allocate (size);	

#if CUSTOM_MEMORY__USE_REPORT
	_report.allocate (pointer, 0, size, tag);
#endif

	return pointer;

	//-----------------------------------------------------------------------
	// return malloc (size);
}

void custom_memory::_deallocate (uint_t tag, void* pointer, size_t size)
{
	size_t i;

	for (i=0; i<_ss_count; i++)
	{
		if ( size <= _ss[i]._storage_size )
		{
#if CUSTOM_MEMORY__USE_REPORT
			_report.deallocate (pointer, _ss[i]._storage_size, size, tag);
#endif

			_ss[i].deallocate (pointer);
			return;
		}
	}

#if CUSTOM_MEMORY__USE_REPORT
	_report.deallocate (pointer, 0, size, tag);
#endif

	_mp.deallocate (pointer);

	//-----------------------------------------------------------------------
	// free (pointer);
}

//===========================================================================
void* custom_memory::stl_allocate (size_t size)
{
#if CUSTOM_MEMORY__SUPPORT_STL
	void* pointer = _allocate (0, size);
	
	if (pointer==0)
	{
		dump();
		printf("custom_memory::stl_allocate() error\r\n");
		throw std::bad_alloc();
	}

	return pointer;
#else
	return 0;
#endif
}

void custom_memory::stl_deallocate (void* pointer, size_t size)
{
#if CUSTOM_MEMORY__SUPPORT_STL
	_deallocate (0, pointer, size);
#endif
}

//===========================================================================
void* custom_memory::allocate (size_t size)
{
	void* pointer = _allocate (1, size);
	
	if (pointer==0)
	{
		dump();
		printf("custom_memory::allocate() error\r\n");
	}

	return pointer;
}

void custom_memory::deallocate (void* pointer)
{
	size_t i;

	for (i=0; i<_ss_count; i++)
	{
		if (_ss[i].is_valid_pointer(pointer))
		{
#if CUSTOM_MEMORY__USE_REPORT
			_report.deallocate (pointer, _ss[i]._storage_size, 0, 1);
#endif
			_ss[i].deallocate (pointer);
			return;
		}
	}

	if (_mp.is_valid_pointer(pointer))
	{
#if CUSTOM_MEMORY__USE_REPORT
		_report.deallocate (pointer, 0, 0, 1);
#endif
		_mp.deallocate(pointer);
		return;
	}

	//-----------------------------------------------------------------------
	// free (pointer);
}

//===========================================================================
void* custom_memory::reallocate (void* pointer, size_t size)
{
	if (0==size)
	{
		deallocate(pointer);
		return 0;
	}
	if (0==pointer)
	{
		return allocate(size);
	}

	void*  new_pointer;
	size_t previously_allocated_size;
	size_t memory_copy_size;
	
	if (true==get_allocated_size (pointer, previously_allocated_size))
	{
		if (size <= previously_allocated_size)
		{
			return pointer;
		}

		memory_copy_size = (previously_allocated_size < size) ? previously_allocated_size : size;

		new_pointer = allocate (size);
		if (new_pointer)
		{
			memcpy(new_pointer, pointer, memory_copy_size);
			deallocate(pointer);

			return new_pointer;
		}
		//else
		//{
		//	new_pointer 할당 실패시 호출한 함수에서 pointer에 대한 deallocate에서 해야한다.
		//}
	}
	
	return 0;
}

bool  custom_memory::get_allocated_size (void* pointer, size_t& size)
{
	size_t i;

	for (i=0; i<_ss_count; i++)
	{
		if (_ss[i].is_valid_pointer(pointer))
		{
			size = _ss[i]._storage_size;
			return true;
		}
	}

	if (_mp.is_valid_pointer(pointer))
	{
		static const memory_pool::size_t MEMORY_POOL_MARKER_FREE      = 0XF3EE;
		static const memory_pool::size_t MEMORY_POOL_MARKER_ALLOCATED = 0XA10C;

		memory_pool_block_t *block;
		

		block = reinterpret_cast< memory_pool_block_t* >( reinterpret_cast< byte_t* >( pointer ) - sizeof(memory_pool_block_t) );
		
		if (MEMORY_POOL_MARKER_ALLOCATED == block->_free)
		{
			size = block->_size;
			return true;
		}
		if (MEMORY_POOL_MARKER_FREE == block->_free)
		{
			return false;
		}
	}

	return false;
}



//===========================================================================
// end of "namespace memory"
} 



//===========================================================================
// end of "namespace cx"
} 
