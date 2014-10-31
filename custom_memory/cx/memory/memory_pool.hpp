#ifndef __CX_MEMORY__MEMORY_POOL__HEADER__
#define __CX_MEMORY__MEMORY_POOL__HEADER__

/////////////////////////////////////////////////////////////////////////////
//
// File: memory_pool.hpp
//
// Created by MOON, Eui-kwon.
// Created on Jul-21st, 2009.
//
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{


	
//===========================================================================
namespace memory
{



/////////////////////////////////////////////////////////////////////////////
//
// Structure: memory_pool_block_t
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
struct _memory_pool_block_t
{
public:
	typedef unsigned long int size_t;
	
public:
	struct _memory_pool_block_t* _prev;
	struct _memory_pool_block_t* _next;
	size_t                       _size;
	size_t                       _free;
};

typedef struct _memory_pool_block_t memory_pool_block_t;



/////////////////////////////////////////////////////////////////////////////
//
// Class: memory_pool
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class memory_pool
{
public:
	typedef unsigned long int size_t;
	typedef unsigned char     byte_t;		
	
public:
	byte_t* _memory_pointer;
	size_t  _memory_size   ;
	size_t  _align_size    ;
	
public:
	memory_pool();
	
public:
	bool create (void* memory_pointer, size_t memory_size, size_t align_size=sizeof(void*));

public:
	void* allocate   (size_t size);
	void  deallocate (void *pointer);
			
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// end of "namespace memory"
} 
	


//===========================================================================
// end of "namespace cx"
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// end of "__CX_MEMORY__MEMORY_POOL__HEADER__"
#endif 
