/////////////////////////////////////////////////////////////////////////////
//
// File: memory_allocator.cpp
//
// Created by MOON, Eui-kwon.
// Created on Jul-21st, 2009.
//
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <stdio.h>

#include <memory>

#if defined(WIN32)
#include <atlbase.h>
#endif

#include "memory_pool.hpp"
#include "simple_segregated_storage.hpp"

#include "memory_allocator.hpp"




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{



//===========================================================================
namespace memory
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void memory_allocator_trace_null (const char* lpFormatStr, ...)
{
}

#if defined(WIN32)
void memory_allocator_trace (const char* lpFormatStr, ...)
{
	char    szBuffer[8192];
	va_list args;

	va_start (args, lpFormatStr);
	vsprintf_s (szBuffer, lpFormatStr, args);
	va_end   (args);      

	OutputDebugStringA (szBuffer);	
}
#else
#define memory_allocator_trace printf
#endif



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define MODULE_TRACE memory_allocator_trace
//#define MODULE_TRACE memory_allocator_trace_null



/////////////////////////////////////////////////////////////////////////////
//
// Class: memory_allocator
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
memory_allocator::memory_allocator()
{
}

memory_allocator::~memory_allocator()
{
}

//===========================================================================
void* memory_allocator::allocate (size_t size)
{
	return 0; 
}

void memory_allocator::deallocate(void* pointer)
{
}

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
memory_allocator_crt::memory_allocator_crt()
{
}

memory_allocator_crt::~memory_allocator_crt()
{
}

//===========================================================================
void* memory_allocator_crt::allocate (size_t size)
{
	return ::malloc(size);
}

void memory_allocator_crt::deallocate(void* pointer)
{
	::free(pointer);
}

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
memory_allocator_ss::memory_allocator_ss():
	_ss()
{
}

memory_allocator_ss::~memory_allocator_ss()
{
}


//===========================================================================
void* memory_allocator_ss::allocate(size_t size)
{
	if (_ss._storage_size<size)
	{
		return 0;
	}

	return _ss.allocate ();
}

void memory_allocator_ss::deallocate(void* pointer)
{
	return _ss.deallocate (pointer);
}

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
memory_allocator_mp::memory_allocator_mp():
	_mp()
{
}

memory_allocator_mp::~memory_allocator_mp()
{
}

//===========================================================================
void* memory_allocator_mp::allocate(size_t size)
{
	return _mp.allocate (size);
}

void memory_allocator_mp::deallocate(void* pointer)
{
	return _mp.deallocate (pointer);
}



/////////////////////////////////////////////////////////////////////////////
//
// Class: memory_chunk
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
memory_chunk::memory_chunk():
	_memory(0),
	_size  (0)
{
}

memory_chunk::~memory_chunk()
{
}



/////////////////////////////////////////////////////////////////////////////
//
// Class: memory_segment
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
memory_segment::memory_segment():
	_previous (0),
	_next     (0),
	_allocator(0),
	_chunk    ()
{
}

memory_segment::~memory_segment()
{
}



/////////////////////////////////////////////////////////////////////////////
//
// Class: memory_allocator_ex_ss
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
memory_allocator_ex_ss::memory_allocator_ex_ss():
	_head         (0),
	_tail         (0),
	_internal_heap(0),
	_external_heap(0),

	_growing_size (0),
	_storage_size (0),
	_allocator    (0)
{
}

memory_allocator_ex_ss::~memory_allocator_ex_ss()
{
}

//===========================================================================
bool memory_allocator_ex_ss::create (memory_allocator* internal_heap, memory_allocator* external_heap, size_t growing_size, size_t storage_size)
{
	_head          = 0            ;
	_tail          = 0            ;
	_internal_heap = internal_heap;
	_external_heap = external_heap;

	_growing_size  = growing_size ;
	_storage_size  = storage_size ;
	_allocator     = 0            ;

	return true;
}

void memory_allocator_ex_ss::destroy (void)
{
	memory_segment* next;

	memory_segment*      segment     ;
	memory_allocator_ss* allocator   ;
	void*                chunk_memory;
	size_t               chunk_size  ;

	next = _head;
	while (next)
	{
		segment      = next;
		allocator    = static_cast<memory_allocator_ss*>(segment->_allocator);
		chunk_memory = segment->_chunk._memory;
		chunk_size   = segment->_chunk._size;

		next = next->_next;

		if (segment)
		{
			segment->~memory_segment();
			_internal_heap->deallocate(segment);
		}
		if (allocator)
		{
			allocator->~memory_allocator_ss();
			_internal_heap->deallocate(allocator);
		}
		if (chunk_memory)
		{
			_external_heap->deallocate(chunk_memory);
		}
	}

	_head          = 0;
	_tail          = 0;
	_internal_heap = 0;
	_external_heap = 0;

	_growing_size  = 0;
	_storage_size  = 0;
	_allocator     = 0;
}

bool memory_allocator_ex_ss::is_valid_pointer (void* pointer) const
{
	memory_segment* next;

	memory_segment*      segment     ;
	memory_allocator_ss* allocator   ;
	void*                chunk_memory;
	size_t               chunk_size  ;

	next = _head;
	while (next)
	{
		segment      = next;
		allocator    = static_cast<memory_allocator_ss*>(segment->_allocator);
		chunk_memory = segment->_chunk._memory;
		chunk_size   = segment->_chunk._size;

		next = next->_next;

		if (true==is_valid_pointer_of_range(chunk_memory, chunk_size, pointer))
		{
			return true;
		}
	}

	return false;
}

bool memory_allocator_ex_ss::is_valid_pointer_of_range (void* begin, size_t size, void* pointer) const
{
	byte_t* base ;
	byte_t* first;
	byte_t* last ;


	base  = reinterpret_cast<byte_t*>(begin);
	first = base     ;
	last  = base+size;

	if ((first<=pointer)&&(pointer<last))
	{
		if ( 0==((base-reinterpret_cast<byte_t*>(pointer))%_storage_size) )
		{
			return true;
		}
	}

	return false;
}

void memory_allocator_ex_ss::grow (void)
{
	void* pointer;

	memory_segment*      segment     ;
	memory_allocator_ss* allocator   ;
	void*                chunk_memory;
	size_t               chunk_size  ;

	memory_allocator_ss* previous_segment_allocator;

	segment      = 0;
	allocator    = 0;
	chunk_memory = 0;
	chunk_size   = 0;
	
	pointer = _internal_heap->allocate(sizeof(memory_segment));
	if (0==pointer)
	{
		goto cleanup;
	}
	segment = new(pointer) memory_segment();


	pointer = _internal_heap->allocate(sizeof(memory_allocator_ss));
	if (0==pointer)
	{
		goto cleanup;
	}
	allocator = new(pointer) memory_allocator_ss();


	pointer = _external_heap->allocate(_growing_size);
	if (0==pointer)
	{
		goto cleanup;
	}
	chunk_memory = pointer;
	chunk_size   = _growing_size;


	//--------------------------------------------------------------------------


	if (false==allocator->_ss.create (chunk_memory, chunk_size, _storage_size))
	{
		goto cleanup;
	}

	segment->_previous      = 0;
	segment->_next          = 0;
	segment->_allocator     = allocator;
	segment->_chunk._memory = chunk_memory;
	segment->_chunk._size   = chunk_size;

	if (0==_head)
	{
		_head = segment;
		_tail = segment;
	}
	else
	{
		segment->_previous = _tail;		
		_tail  ->_next     = segment;
		_tail              = segment;
	}
	
	if (_tail->_previous)
	{
		if (_tail->_previous->_allocator)
		{
			// up-casting
			previous_segment_allocator = static_cast<memory_allocator_ss*>( _tail->_previous->_allocator );

			if (previous_segment_allocator)
			{
				previous_segment_allocator->_ss._tail->_next = allocator->_ss._head;
			}
		}
	}

	if (0==_allocator)
	{
		_allocator = allocator;
	}

	return;

	//--------------------------------------------------------------------------

cleanup:
	if (segment)
	{
		segment->~memory_segment();
		_internal_heap->deallocate(segment);
	}
	if (allocator)
	{
		allocator->~memory_allocator_ss();
		_internal_heap->deallocate(allocator);
	}
	if (chunk_memory)
	{
		_external_heap->deallocate(chunk_memory);
	}
}

void memory_allocator_ex_ss::dump (void)
{
	MODULE_TRACE ("# memory_allocator_ex_ss::dump() : instance=0x%p _storage_size=%d \r\n", this, _storage_size);
	MODULE_TRACE ( " \r\n" );
	
	dump_memory_segment (_head);

	memory_segment* next;

	memory_segment*      segment     ;
	memory_allocator_ss* allocator   ;
	void*                chunk_memory;
	size_t               chunk_size  ;

	uint_t count;

	count = 0;

	next = _head;
	while (next)
	{
		segment      = next;
		allocator    = static_cast<memory_allocator_ss*>(segment->_allocator);
		chunk_memory = segment->_chunk._memory;
		chunk_size   = segment->_chunk._size;

		next = next->_next;

		MODULE_TRACE ("# segment [%d] \r\n", count ++);
		dump_simple_segregated_storage(&allocator->_ss);
	}

	MODULE_TRACE ( " \r\n" );
	MODULE_TRACE ( " \r\n" );
	MODULE_TRACE ( " \r\n" );
}

void* memory_allocator_ex_ss::allocate (size_t size)
{
	void* pointer = 0;


	if (0==_allocator)
	{
		grow();

		if (_allocator)
		{
			pointer = _allocator->allocate(size);
		}

		return pointer;
	}

	pointer = _allocator->allocate(size);
	if (0==pointer)
	{
		grow();

		pointer = _allocator->allocate(size);
	}

	return pointer;
}

void memory_allocator_ex_ss::deallocate (void* pointer)
{
	if (_allocator)
	{
		_allocator->deallocate (pointer);
	}
}



/////////////////////////////////////////////////////////////////////////////
//
// Class: memory_allocator_ex_mp
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
memory_allocator_ex_mp::memory_allocator_ex_mp():
	_head         (0),
	_tail         (0),
	_internal_heap(0),
	_external_heap(0),
	_growing_size (0)
{
}

memory_allocator_ex_mp::~memory_allocator_ex_mp()
{
}

//===========================================================================
bool memory_allocator_ex_mp::create (memory_allocator* internal_heap, memory_allocator* external_heap, size_t growing_size)
{
	_head          = 0            ;
	_tail          = 0            ;
	_internal_heap = internal_heap;
	_external_heap = external_heap;

	_growing_size  = growing_size ;

	return true;
}

void memory_allocator_ex_mp::destroy (void)
{
	memory_segment* next;

	memory_segment*      segment     ;
	memory_allocator_mp* allocator   ;
	void*                chunk_memory;
	size_t               chunk_size  ;

	next = _head;
	while (next)
	{
		segment      = next;
		allocator    = static_cast<memory_allocator_mp*>( segment->_allocator );
		chunk_memory = segment->_chunk._memory;
		chunk_size   = segment->_chunk._size;

		next = next->_next;

		if (segment)
		{
			segment->~memory_segment();
			_internal_heap->deallocate(segment);
		}
		if (allocator)
		{
			allocator->~memory_allocator_mp();
			_internal_heap->deallocate(allocator);
		}
		if (chunk_memory)
		{
			_external_heap->deallocate(chunk_memory);
		}
	}

	_head          = 0;
	_tail          = 0;
	_internal_heap = 0;
	_external_heap = 0;

	_growing_size  = 0;
}

bool memory_allocator_ex_mp::is_valid_pointer (void* pointer) const
{
	memory_segment* next;

	memory_segment*      segment     ;
	memory_allocator_mp* allocator   ;
	void*                chunk_memory;
	size_t               chunk_size  ;

	next = _head;
	while (next)
	{
		segment      = next;
		allocator    = static_cast<memory_allocator_mp*>(segment->_allocator);
		chunk_memory = segment->_chunk._memory;
		chunk_size   = segment->_chunk._size;

		next = next->_next;

		if (true==is_valid_pointer_of_range(chunk_memory, chunk_size, pointer))
		{
			return true;
		}
	}

	return false;
}

bool memory_allocator_ex_mp::is_valid_pointer_of_range (void* begin, size_t size, void* pointer) const
{
	byte_t* base ;
	byte_t* first;
	byte_t* last ;


	base  = reinterpret_cast<byte_t*>(begin);
	first = base     +sizeof(memory_pool_block_t);
	last  = base+size-sizeof(memory_pool_block_t);

	if ((first<=pointer)&&(pointer<=last))
	{
		return true;
	}

	return false;
}

memory_segment* memory_allocator_ex_mp::find_segment (void* pointer)
{
	memory_segment* next;

	memory_segment*   segment     ;
	memory_allocator* allocator   ;
	void*             chunk_memory;
	size_t            chunk_size  ;
	
	next = _head;
	while (next)
	{
		segment      = next                   ;
		allocator    = segment->_allocator    ;
		chunk_memory = segment->_chunk._memory;
		chunk_size   = segment->_chunk._size  ;

		next = next->_next;


		if ( is_valid_pointer_of_range(chunk_memory, chunk_size, pointer) )
		{
			return segment;
		}
	}

	return 0;
}

memory_segment* memory_allocator_ex_mp::grow (size_t size)
{
	void* pointer;

	memory_segment*      segment     ;
	memory_allocator_mp* allocator   ;
	void*                chunk_memory;
	size_t               chunk_size  ;

	segment      = 0;
	allocator    = 0;
	chunk_memory = 0;
	chunk_size   = 0;

	pointer = _internal_heap->allocate(sizeof(memory_segment));
	if (0==pointer)
	{
		goto cleanup;
	}
	segment = new(pointer) memory_segment();


	pointer = _internal_heap->allocate(sizeof(memory_allocator_mp));
	if (0==pointer)
	{
		goto cleanup;
	}
	allocator = new(pointer) memory_allocator_mp();


	pointer = _external_heap->allocate(size);
	if (0==pointer)
	{
		goto cleanup;
	}
	chunk_memory = pointer;
	chunk_size   = size;

	//--------------------------------------------------------------------------

	if (false==allocator->_mp.create (chunk_memory, chunk_size))
	{
		goto cleanup;
	}

	segment->_previous      = 0;
	segment->_next          = 0;
	segment->_allocator     = allocator;
	segment->_chunk._memory = chunk_memory;
	segment->_chunk._size   = chunk_size;

	if (0==_head)
	{
		_head = segment;
		_tail = segment;
	}
	else
	{
		segment->_previous = _tail;		
		_tail  ->_next     = segment;
		_tail              = segment;
	}
	
	return segment;

	//--------------------------------------------------------------------------

cleanup:
	if (segment)
	{
		segment->~memory_segment();
		_internal_heap->deallocate(segment);
	}
	if (allocator)
	{
		allocator->~memory_allocator_mp();
		_internal_heap->deallocate(allocator);
	}
	if (chunk_memory)
	{
		_external_heap->deallocate(chunk_memory);
	}

	return 0;
}

void memory_allocator_ex_mp::dump (void)
{
	MODULE_TRACE ("# memory_allocator_ex_mp::dump() : instance=0x%p \r\n", this);
	MODULE_TRACE ( " \r\n" );
	
	dump_memory_segment (_head);

	memory_segment* next;

	memory_segment*      segment     ;
	memory_allocator_mp* allocator   ;
	void*                chunk_memory;
	size_t               chunk_size  ;

	uint_t count;


	count = 0;

	next = _head;
	while (next)
	{
		segment      = next;
		allocator    = static_cast<memory_allocator_mp*>(segment->_allocator);
		chunk_memory = segment->_chunk._memory;
		chunk_size   = segment->_chunk._size;

		next = next->_next;

		MODULE_TRACE ("# segment [%d] \r\n", count ++);
		dump_memory_pool(&allocator->_mp);
	}

	MODULE_TRACE ( " \r\n" );
	MODULE_TRACE ( " \r\n" );
	MODULE_TRACE ( " \r\n" );
}

void* memory_allocator_ex_mp::allocate (size_t size)
{
	void* pointer = 0;

	memory_segment* next;

	memory_segment*   segment     ;
	memory_allocator* allocator   ;
	void*             chunk_memory;
	size_t            chunk_size  ;
	
	next = _head;
	while (next)
	{
		segment      = next                   ;
		allocator    = segment->_allocator    ;
		chunk_memory = segment->_chunk._memory;
		chunk_size   = segment->_chunk._size  ;

		next = next->_next;

		if ( chunk_size >= size )
		{
			pointer = allocator->allocate (size);
			if (pointer)
			{
				return pointer;
			}
		}
	}


	size_t growing_size;
	
	if ( _growing_size < (size + sizeof(memory_pool_block_t)) )
	{
		growing_size = size + sizeof(memory_pool_block_t);
	}
	else
	{
		growing_size = _growing_size;
	}
	

	segment = grow(growing_size);
	if (segment)
	{
		pointer = segment->_allocator->allocate(size);
	
		return pointer;
	}

	return 0;
}

void memory_allocator_ex_mp::deallocate (void* pointer)
{
	memory_segment* segment = find_segment (pointer);

	if (segment)
	{
		segment->_allocator->deallocate (pointer);
	}
}



/////////////////////////////////////////////////////////////////////////////
//
// Class: memory_allocator_ex_crt
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
memory_allocator_ex_crt::memory_allocator_ex_crt():
	_head         (0),
	_tail         (0),
	_internal_heap(0),
	_external_heap(0),
	_allocator    ()
{
}

memory_allocator_ex_crt::~memory_allocator_ex_crt()
{
}

//===========================================================================
bool memory_allocator_ex_crt::create (void)
{
	_head          = 0          ;
	_tail          = 0          ;
	_internal_heap = &_allocator;
	_external_heap = &_allocator;

	return true;
}

void memory_allocator_ex_crt::destroy (void)
{
	memory_segment* next;

	memory_segment*       segment     ;
	memory_allocator_crt* allocator   ;
	void*                 chunk_memory;
	size_t                chunk_size  ;
	
	next = _head;
	while (next)
	{
		segment      = next;
		allocator    = static_cast<memory_allocator_crt*>(segment->_allocator);
		chunk_memory = segment->_chunk._memory;
		chunk_size   = segment->_chunk._size;

		next = next->_next;

		if (segment)
		{
			segment->~memory_segment();
			_internal_heap->deallocate(segment);
		}
//		if (allocator)
//		{
//			allocator->~memory_allocator_crt();
//			_internal_heap->deallocate(allocator);
//		}
		if (chunk_memory)
		{
			_external_heap->deallocate(chunk_memory);
		}
	}

	_head          = 0;
	_tail          = 0;
	_internal_heap = 0;
	_external_heap = 0;
}

memory_segment* memory_allocator_ex_crt::find_allocated_segment (void* pointer)
{
	memory_segment* next;

	memory_segment*   segment     ;
	memory_allocator* allocator   ;
	void*             chunk_memory;
	size_t            chunk_size  ;

	next = _head;
	while (next)
	{
		segment      = next                   ;
		allocator    = segment->_allocator    ;
		chunk_memory = segment->_chunk._memory;
		chunk_size   = segment->_chunk._size  ;

		next = next->_next;

		if ( &_allocator == allocator )
		{
			if ( chunk_memory == pointer )
			{
				return segment;
			}
		}
	}

	return 0;
}

memory_segment* memory_allocator_ex_crt::find_free_segment (size_t size)
{
	memory_segment* next;

	memory_segment*   segment     ;
	memory_allocator* allocator   ;
	void*             chunk_memory;
	size_t            chunk_size  ;

	next = _head;
	while (next)
	{
		segment      = next                   ;
		allocator    = segment->_allocator    ;
		chunk_memory = segment->_chunk._memory;
		chunk_size   = segment->_chunk._size  ;

		next = next->_next;

		if ( 0 == allocator )
		{
			if ( chunk_size >= size )
			{
				return segment;
			}
		}
	}

	return 0;
}

memory_segment* memory_allocator_ex_crt::grow (size_t size)
{
	void* pointer;

	memory_segment*       segment     ;
	memory_allocator_crt* allocator   ;
	void*                 chunk_memory;
	size_t                chunk_size  ;
	
	segment      = 0;
	allocator    = 0;
	chunk_memory = 0;
	chunk_size   = 0;

	pointer = _internal_heap->allocate(sizeof(memory_segment));
	if (0==pointer)
	{
		goto cleanup;
	}
	segment = new(pointer) memory_segment();


//	pointer = _internal_heap->allocate(sizeof(memory_allocator_crt));
//	if (0==pointer)
//	{
//		goto cleanup;
//	}
//	allocator = new(pointer) memory_allocator_crt();
	allocator = &_allocator;


	pointer = _external_heap->allocate(size);
	if (0==pointer)
	{
		goto cleanup;
	}
	chunk_memory = pointer;
	chunk_size   = size;

	//--------------------------------------------------------------------------

	segment->_previous      = 0;
	segment->_next          = 0;
	segment->_allocator     = allocator;
	segment->_chunk._memory = chunk_memory;
	segment->_chunk._size   = chunk_size;

	if (0==_head)
	{
		_head = segment;
		_tail = segment;
	}
	else
	{
		segment->_previous = _tail;		
		_tail  ->_next     = segment;
		_tail              = segment;
	}
	
	return segment;

	//--------------------------------------------------------------------------

cleanup:
	if (segment)
	{
		segment->~memory_segment();
		_internal_heap->deallocate (segment);
	}
//	if (allocator)
//	{
//		allocator->~memory_allocator_crt();
//		_internal_heap->deallocate (allocator);
//	}
	if (chunk_memory)
	{
		_external_heap->deallocate (chunk_memory);
	}

	return 0;
}

void memory_allocator_ex_crt::dump (void)
{
	MODULE_TRACE ("# memory_allocator_ex_crt::dump() : instance=0x%p \r\n", this);
	MODULE_TRACE ( " \r\n" );
	
	dump_memory_segment (_head);
	
	MODULE_TRACE ( " \r\n" );
	MODULE_TRACE ( " \r\n" );
	MODULE_TRACE ( " \r\n" );
}

void* memory_allocator_ex_crt::allocate (size_t size)
{
	memory_segment* segment;

	segment = find_free_segment (size);
	if (segment)
	{
		segment->_allocator = &_allocator;

		return segment->_chunk._memory;
	}

	segment = grow (size);
	if (segment)
	{
		return segment->_chunk._memory;
	}

	return 0;
}

void memory_allocator_ex_crt::deallocate (void* pointer)
{
	memory_segment* segment;

	segment = find_allocated_segment (pointer);
	if (segment)
	{
		segment->_allocator = 0;
	}
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void dump_memory_segment (memory_segment* head)
{
	memory_segment* next;

	memory_segment*   segment     ;
	memory_allocator* allocator   ;
	void*             chunk_memory;
	size_t            chunk_size  ;
	
	uint_t count;


	MODULE_TRACE ( "=============================================================================\r\n" );
	MODULE_TRACE ( "dump_memory_segment(0x%p) \r\n", head);
	MODULE_TRACE ( "=============================================================================\r\n" );

	count = 0;

	next = head;
	while (next)
	{
		segment      = next;
		allocator    = static_cast<memory_allocator_crt*>(segment->_allocator);
		chunk_memory = segment->_chunk._memory;
		chunk_size   = segment->_chunk._size;

		next = next->_next;

		MODULE_TRACE ( "[%d] segment=0x%p allocator=0x%p chunk_memory=0x%p chunk_size=%d \r\n", 
			count++     ,
			segment     ,
			allocator   ,
			chunk_memory,
			chunk_size  );
	}

	MODULE_TRACE ( "=============================================================================\r\n" );
	MODULE_TRACE ( " \r\n" );
}

void dump_memory_pool (memory_pool* mp)
{
	static const memory_pool::size_t MEMORY_POOL_MARKER_FREE      = 0XF3EE;
	static const memory_pool::size_t MEMORY_POOL_MARKER_ALLOCATED = 0XA10C;

	memory_pool_block_t* block;

	size_t block_count;
	size_t free_size;
	size_t allocated_size;
	

	MODULE_TRACE ( "=============================================================================\r\n" );
	MODULE_TRACE ( "dump_memory_pool(0x%p) \r\n", mp);
	MODULE_TRACE ( "=============================================================================\r\n" );
	MODULE_TRACE ( "memory: address=0x%p size=%d \r\n", mp->_memory_pointer, mp->_memory_size );
	MODULE_TRACE ( "-----------------------------------------------------------------------------\r\n" );


	allocated_size = 0;
	free_size      = 0;
	block_count    = 0;
		
	block = reinterpret_cast < memory_pool_block_t* > ( mp->_memory_pointer );
		
	while ( block )
	{
		MODULE_TRACE ( "[%d] address=0x%p size=%d free=%x prev=0x%p next=0x%p \r\n", 
			block_count++,
			block, block->_size, block->_free, block->_prev, block->_next );
			
		if ( MEMORY_POOL_MARKER_FREE == block->_free )
		{
			free_size += block->_size;
		}
		if ( MEMORY_POOL_MARKER_ALLOCATED == block->_free )
		{
			allocated_size += block->_size;
		}
			
		block = block->_next;
	}


	MODULE_TRACE ( "-----------------------------------------------------------------------------\r\n" );
	MODULE_TRACE ( "blockhead memory = %d (%d) \r\n", block_count * sizeof(memory_pool_block_t), block_count );
	MODULE_TRACE ( "overhaed memory  = %d \r\n", mp->_memory_size - allocated_size - free_size );
	MODULE_TRACE ( "allocated memory = %d \r\n", allocated_size );
	MODULE_TRACE ( "free memory      = %d \r\n", free_size );
	MODULE_TRACE ( "=============================================================================\r\n" );
	MODULE_TRACE ( " \r\n" );
}

void dump_simple_segregated_storage(simple_segregated_storage* ss)    
{
	simple_segregated_storage_link_t *link;
		
	size_t max_link_count;
	size_t free_link_count;
	size_t allocated_link_count;


	MODULE_TRACE ( "=============================================================================\r\n" );
	MODULE_TRACE ( "dump_simple_segregated_storage(0x%p) : %d \r\n", ss, ss->_storage_size );
	MODULE_TRACE ( "=============================================================================\r\n" );
	MODULE_TRACE ( "memory: address=0x%p size=%d \r\n", ss->_memory_pointer, ss->_memory_size );
	MODULE_TRACE ( "-----------------------------------------------------------------------------\r\n" );


	link = ss->_head;
	
	free_link_count=0;
	while (0!=link)
	{
		 free_link_count++;

		/*
		MODULE_TRACE ( "[%d] address=0x%p next=0x%p \r\n", free_link_count, link, link->_next );        
		*/

		link = link->_next;
	}
        
    max_link_count       = ss->_memory_size / ss->_storage_size;
	allocated_link_count = max_link_count-free_link_count;

	/*
	MODULE_TRACE ( "-----------------------------------------------------------------------------\r\n" );
	*/
	MODULE_TRACE ( "max link count        = %d \r\n", max_link_count );
	MODULE_TRACE ( "free link count       = %d \r\n", free_link_count );

	if (free_link_count <= max_link_count)
	{
		MODULE_TRACE ( "allocated link count  = %d \r\n", allocated_link_count );
		MODULE_TRACE ( "overhaed memory       = %d \r\n", ss->_memory_size-(max_link_count*ss->_storage_size ) );
		MODULE_TRACE ( "allocated memory      = %d \r\n", allocated_link_count*ss->_storage_size );
	}

	MODULE_TRACE ( "free memory           = %d \r\n", free_link_count*ss->_storage_size );
	MODULE_TRACE ( "=============================================================================\r\n" );
	MODULE_TRACE ( " \r\n" );
}



//===========================================================================
// end of "namespace memory"
} 



//===========================================================================
// end of "namespace cx"
} 
