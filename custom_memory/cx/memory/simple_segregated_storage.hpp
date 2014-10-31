#ifndef __CX_MEMORY__SIMPLE_SEGREGATED_STORAGE__HEADER__
#define __CX_MEMORY__SIMPLE_SEGREGATED_STORAGE__HEADER__

/////////////////////////////////////////////////////////////////////////////
//
// File: simple_segregated_storage.hpp
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
// Structure: simple_segregated_storage_link_t
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
struct _simple_segregated_storage_link_t
{
	struct _simple_segregated_storage_link_t* _next;
};

typedef struct _simple_segregated_storage_link_t simple_segregated_storage_link_t;



/////////////////////////////////////////////////////////////////////////////
//
// Class: simple_segregated_storage
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class simple_segregated_storage
{
public:
	typedef unsigned long int size_t;
	typedef unsigned char     byte_t;
	
public:
	byte_t* _memory_pointer;
	size_t  _memory_size   ;
	size_t  _align_size    ;

	size_t _storage_size;

	simple_segregated_storage_link_t* _tail;

public:
	simple_segregated_storage_link_t* _head;

public:
	simple_segregated_storage();

public:
	bool create (void* memory_pointer, size_t memory_size, size_t storage_size, size_t align_size = sizeof(void*));

public:
	void* allocate   (void);
	void  deallocate (void* pointer);
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
// #ifndef __CX_MEMORY__SIMPLE_SEGREGATED_STORAGE__HEADER__
#endif 
