#ifndef __CX_MEMORY__MEMORY_ALLOCATOR__HEADER__
#define __CX_MEMORY__MEMORY_ALLOCATOR__HEADER__

/////////////////////////////////////////////////////////////////////////////
//
// File: memory_allocator.hpp
//
// Created by MOON, Eui-kwon.
// Created on Jul-21st, 2009.
//
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "type.hpp"
#include "simple_segregated_storage.hpp"
#include "memory_pool.hpp"



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{



//===========================================================================
namespace memory
{



/////////////////////////////////////////////////////////////////////////////
//
// Class: memory_allocator
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class memory_allocator
{
public:
	memory_allocator();
	virtual ~memory_allocator();

public:
	virtual void* allocate   (size_t size);
	virtual void  deallocate (void* pointer);
};

//===========================================================================
class memory_allocator_crt : public memory_allocator
{
public:
	memory_allocator_crt();
	virtual ~memory_allocator_crt();

public:
	virtual void* allocate   (size_t size);
	virtual void  deallocate (void* pointer);
};

//===========================================================================
class memory_allocator_ss : public memory_allocator
{
public:
	simple_segregated_storage _ss;

public:
	memory_allocator_ss();
	virtual ~memory_allocator_ss();

public:
	virtual void* allocate   (size_t size);
	virtual void  deallocate (void* pointer);
};

//===========================================================================
class memory_allocator_mp : public memory_allocator
{
public:
	memory_pool _mp;

public:
	memory_allocator_mp();
	virtual ~memory_allocator_mp();

public:
	virtual void* allocate   (size_t size);
	virtual void  deallocate (void* pointer);
};



/////////////////////////////////////////////////////////////////////////////
//
// Class: memory_chunk
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class memory_chunk
{
public:
	void*  _memory;
	size_t _size  ;

public:
	memory_chunk();
	~memory_chunk();
};



/////////////////////////////////////////////////////////////////////////////
//
// Class: memory_segment
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class memory_segment
{
public:
	memory_segment*   _previous  ;
	memory_segment*   _next      ;
	memory_allocator* _allocator ;
	memory_chunk      _chunk     ;

public:
	memory_segment();
	~memory_segment();
};



/////////////////////////////////////////////////////////////////////////////
//
// Class: memory_allocator_ex_ss
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class memory_allocator_ex_ss : public memory_allocator
{
public:
	memory_allocator_ex_ss();
	virtual ~memory_allocator_ex_ss();

public:
	memory_segment*   _head         ;
	memory_segment*   _tail         ;
	memory_allocator* _internal_heap;
	memory_allocator* _external_heap;

public:
	size_t            _growing_size ;
	size_t            _storage_size ;
	memory_allocator* _allocator    ;

public:
	bool create  (memory_allocator* internal_heap, memory_allocator* external_heap, size_t growing_size, size_t storage_size);
	void destroy (void);

public:
	bool is_valid_pointer          (void* pointer) const;
	bool is_valid_pointer_of_range (void* begin, size_t size, void* pointer) const;

public:
	void grow (void);
	void dump (void);

public:
	virtual void* allocate   (size_t size);
	virtual void  deallocate (void* pointer);
};



/////////////////////////////////////////////////////////////////////////////
//
// Class: memory_allocator_ex_mp
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class memory_allocator_ex_mp : public memory_allocator
{
public:
	memory_allocator_ex_mp();
	virtual ~memory_allocator_ex_mp();

public:
	memory_segment*   _head         ;
	memory_segment*   _tail         ;
	memory_allocator* _internal_heap;
	memory_allocator* _external_heap;

public:
	size_t _growing_size ;

public:
	bool create  (memory_allocator* internal_heap, memory_allocator* external_heap, size_t growing_size);
	void destroy (void);

public:
	bool            is_valid_pointer          (void* pointer) const;
	bool            is_valid_pointer_of_range (void* begin, size_t size, void* pointer) const;
	memory_segment* find_segment              (void* pointer);

public:
	memory_segment* grow (size_t size);
	void dump (void);

public:
	virtual void* allocate   (size_t size);
	virtual void  deallocate (void* pointer);
};



/////////////////////////////////////////////////////////////////////////////
//
// Class: memory_allocator_ex_crt
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class memory_allocator_ex_crt : public memory_allocator
{
public:
	memory_allocator_ex_crt();
	virtual ~memory_allocator_ex_crt();

public:
	memory_segment*   _head         ;
	memory_segment*   _tail         ;
	memory_allocator* _internal_heap;
	memory_allocator* _external_heap;

public:
	memory_allocator_crt _allocator;

public:
	bool create  (void);
	void destroy (void);

public:
	memory_segment* find_allocated_segment (void* pointer);
	memory_segment* find_free_segment      (size_t size);

public:
	memory_segment* grow (size_t size);
	void dump (void);

public:
	virtual void* allocate   (size_t size);
	virtual void  deallocate (void* pointer);
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void dump_memory_segment (memory_segment* head);
void dump_simple_segregated_storage(simple_segregated_storage* ss);
void dump_memory_pool (memory_pool* mp);



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// end of "namespace memory"
} 



//===========================================================================
// end of "namespace cx"
} 



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// end of "#ifndef __CX_MEMORY__MEMORY_ALLOCATOR__HEADER__"
#endif 
