#ifndef __CX_MEMORY__CUSTOM_MEMORY__HEADER__
#define __CX_MEMORY__CUSTOM_MEMORY__HEADER__

/////////////////////////////////////////////////////////////////////////////
//
// File: custom_memory.hpp
//
// Created by MOON, Eui-kwon.
// Created on Jul-21st, 2009.
//
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define CUSTOM_MEMORY__FIXED_HEAP   0
#define CUSTOM_MEMORY__GROWING_HEAP 1
#define CUSTOM_MEMORY__USE_REPORT   0
#define CUSTOM_MEMORY__SUPPORT_STL  1


/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "type.hpp"
#include "memory_allocator.hpp"

#if CUSTOM_MEMORY__USE_REPORT
#include "custom_memory_report.hpp"
#endif



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{



//===========================================================================
namespace memory
{



/////////////////////////////////////////////////////////////////////////////
//
// Class: custom_memory_fixed_heap
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class custom_memory_fixed_heap
{
public:
	void*                   _heap_memory   ;
	size_t                  _heap_size     ;
	memory_allocator_mp     _heap_allocator;
	memory_allocator_ex_mp  _heap;

public:
	custom_memory_fixed_heap();
	~custom_memory_fixed_heap();

public:
	bool create  (void);
	void destroy (void);
	void dump    (void);
};



/////////////////////////////////////////////////////////////////////////////
//
// Class: custom_memory_growing_heap
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class custom_memory_growing_heap
{
public:
	memory_allocator_ex_crt _heap;

public:
	custom_memory_growing_heap();
	~custom_memory_growing_heap();

public:
	bool create  (void);
	void destroy (void);
	void dump    (void);
};



/////////////////////////////////////////////////////////////////////////////
//
// Class: custom_memory
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class custom_memory
{
private:
	custom_memory ( const custom_memory& );
	custom_memory& operator = ( const custom_memory& );

public:
	custom_memory();
	~custom_memory();

public:
	void* stl_allocate   (size_t size);
	void  stl_deallocate (void* pointer, size_t size);

	void* allocate   (size_t size);
	void  deallocate (void* pointer);

public:
	void* reallocate (void* pointer, size_t size);
	bool  get_allocated_size (void* pointer, size_t& size);

private:
	void* _allocate   (uint_t tag, size_t size);
	void  _deallocate (uint_t tag, void* pointer, size_t size);

public:
	bool create (void);
	bool destroy(void);

public:
	void dump (void);

public:
#if CUSTOM_MEMORY__USE_REPORT
	custom_memory_report _report;
#endif

private:
	memory_allocator_ex_mp  _mp;
	memory_allocator_ex_ss  _ss[256];
	size_t                  _ss_count;

private:
	//--------------------------------------------------------------------------
#if CUSTOM_MEMORY__FIXED_HEAP
	custom_memory_fixed_heap _heap;
#endif

	//--------------------------------------------------------------------------
#if CUSTOM_MEMORY__GROWING_HEAP
	custom_memory_growing_heap _heap;
#endif

	//--------------------------------------------------------------------------
	memory_allocator_ex_ss  _internal_heap;
	memory_allocator_ex_mp  _external_heap;
};



//===========================================================================
// end of "namespace memory"
} 



//===========================================================================
// end of "namespace cx"
} 


/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// end of "#ifndef __CX_MEMORY__CUSTOM_MEMORY__HEADER__"
#endif 
