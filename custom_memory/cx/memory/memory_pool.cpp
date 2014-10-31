/////////////////////////////////////////////////////////////////////////////
//
// File: memory_pool.cpp
//
// Created by MOON, Eui-kwon.
// Created on Jul-21st, 2009.
//
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "memory_pool.hpp"



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{


	
//===========================================================================
namespace memory
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static const memory_pool::size_t MEMORY_POOL_MARKER_FREE      = 0XF3EE;
static const memory_pool::size_t MEMORY_POOL_MARKER_ALLOCATED = 0XA10C;



/////////////////////////////////////////////////////////////////////////////
//
// Class: memory_pool< _align_size >
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
memory_pool::memory_pool() :
	_memory_pointer (0),
	_memory_size    (0),
	_align_size     (0)
{
}
		
//===========================================================================
bool memory_pool::create (void* memory_pointer, size_t memory_size, size_t align_size)
{
	if ( 0==memory_pointer )
	{
		return false;
	}
	if ( 0==memory_size )
	{
		return false;
	}
	if ( 0==align_size )
	{
		return false;
	}
	if ( 0!=(align_size%sizeof(void*)) )
	{
		return false;
	}
	if ( 0!=((reinterpret_cast< unsigned long long int >( memory_pointer )) % align_size) )
	{
		return false;
	}


	if ( memory_size <= sizeof(memory_pool_block_t) )
	{
		return false;
	}
	

	memory_pool_block_t* block;

	block = reinterpret_cast < memory_pool_block_t* > ( memory_pointer );
	block->_prev = 0;
	block->_next = 0;
	block->_free = MEMORY_POOL_MARKER_FREE;
	block->_size = memory_size - sizeof(memory_pool_block_t);


	_memory_pointer= reinterpret_cast < byte_t* > ( memory_pointer );
	_memory_size   = memory_size;
	_align_size    = align_size;

	return true;
}

void* memory_pool::allocate (size_t size)
{
	if ( 0==size || ( size%_align_size ) )
	{
		size = _align_size*( size/_align_size+1 );
	}
		
	if ( sizeof(memory_pool_block_t) + size > _memory_size ) 
	{
		return 0;
	}


	memory_pool_block_t *block;

	// find a free block
	block = reinterpret_cast < memory_pool_block_t* > ( _memory_pointer );
	while ( block )
	{
		if ( MEMORY_POOL_MARKER_FREE==block->_free )
		{
			if ( block->_size >= size )
			{
				break;
			}
		}
						
		block = block->_next;
	}
	if ( !block )
	{
		return 0;
	}
		
		
	if ( block->_size < sizeof(memory_pool_block_t)*2 + size )
	{
		block->_free = MEMORY_POOL_MARKER_ALLOCATED;
			
		return reinterpret_cast< byte_t* >( block ) + sizeof(memory_pool_block_t);
	}
	else
	{
		// split current block			
		memory_pool_block_t* next;
			
		next = reinterpret_cast< memory_pool_block_t* >(
			reinterpret_cast< byte_t* >( block ) + size+sizeof(memory_pool_block_t)
		);

		// insert split block between block and block->_next
		if ( block->_next )
		{
			block->_next->_prev = next;
		}
			
		next ->_next = block->_next;
		next ->_prev = block;
			
		block->_next = next;

		// set size and free-mark
		next ->_size = block->_size - size - sizeof(memory_pool_block_t);
		next ->_free = MEMORY_POOL_MARKER_FREE;

		block->_free = MEMORY_POOL_MARKER_ALLOCATED;
		block->_size = size;
			
		return reinterpret_cast< byte_t* >( block ) + sizeof(memory_pool_block_t);
	}
		
	return 0;
}

void memory_pool::deallocate (void *pointer)
{
	if ( !pointer )
	{
		return;
	}


	memory_pool_block_t *block;
		
	block = reinterpret_cast< memory_pool_block_t* >( reinterpret_cast< byte_t* >( pointer ) - sizeof(memory_pool_block_t) );
		
		
	if ( MEMORY_POOL_MARKER_ALLOCATED!=block->_free )
	{
		// 올바르지 않은 pointer 
		return;
	}


	if ( block->_prev && block->_next )
	{
		if ( MEMORY_POOL_MARKER_FREE==block->_prev->_free && MEMORY_POOL_MARKER_FREE==block->_next->_free )
		{
			//   prev  <- block -> next
			// [ free  ][ alloc ][ free  ]
			block->_prev->_size += block->_size + block->_next->_size + 2*sizeof(memory_pool_block_t);
			block->_prev->_next  = block->_next->_next;
				
			if ( block->_next->_next )
			{
				block->_next->_next->_prev = block->_prev;
			}
				
			return;
		}
	}
		
	if ( block->_prev )
	{
		if ( MEMORY_POOL_MARKER_FREE==block->_prev->_free )
		{
			//   prev  <- block -> next
			// [ free  ][ alloc ][ alloc ]
			// [ free  ][ alloc ][ null  ]
			block->_prev->_size += block->_size + sizeof(memory_pool_block_t);
			block->_prev->_next  = block->_next;
				
			if ( block->_next )
			{
				block->_next->_prev = block->_prev;
			}
				
			/*
			필요없음
			block->_free = MEMORY_POOL_MARKER_FREE;
			*/

			return;
		}
	}
		
	if ( block->_next )
	{
		if ( MEMORY_POOL_MARKER_FREE==block->_next->_free )
		{
			//   prev  <- block -> next
			// [ alloc ][ alloc ][ free  ]					
			// [ null  ][ alloc ][ free  ]					
			block->_size += block->_next->_size + sizeof(memory_pool_block_t);
			block->_next  = block->_next->_next;
				
			if ( block->_next )
			{
				block->_next->_prev = block;
			}
				
			block->_free = MEMORY_POOL_MARKER_FREE;
				
			return;
		}
	}
		
	//   prev  <- block -> next
	// [ alloc ][ alloc ][ alloc ]
	// [ alloc ][ alloc ][ null  ]
	// [ null  ][ alloc ][ alloc ]
	// [ null  ][ alloc ][ null  ]
	block->_free = MEMORY_POOL_MARKER_FREE;
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// end of "namespace memory"
} 
	


//===========================================================================
// end of "namespace cx"
}
