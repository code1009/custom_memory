/////////////////////////////////////////////////////////////////////////////
//
// File: simple_segregated_storage.cpp
//
// Created by MOON, Eui-kwon.
// Created on Jul-21st, 2009.
//
/////////////////////////////////////////////////////////////////////////////



/*
=============================================================================
# simple_segregated_storage:
1. _head�� �׻� free�� �������� ����Ų��.
2. allocate�� _head�� allcoated-pointer�� �ǰ� 
   _head->next�� _head�� �ȴ�.
3. deallocate�� free�� ��û�� pointer�� _head�� �ǰ� 
   _head->_next�� ���� _head�� ����Ų��.
=============================================================================
*/



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "simple_segregated_storage.hpp"



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{


	
//===========================================================================
namespace memory
{



/////////////////////////////////////////////////////////////////////////////
//
// Class: simple_segregated_storage
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
simple_segregated_storage::simple_segregated_storage():
	_memory_pointer(0),
	_memory_size   (0),
	_align_size    (0),
	_storage_size  (0),
	_tail          (0),

	_head (0)
{
}

//===========================================================================
bool simple_segregated_storage::create (void* memory_pointer, size_t memory_size, size_t storage_size, size_t align_size)
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


	if ( storage_size < sizeof(simple_segregated_storage_link_t) )
	{
		storage_size = sizeof(simple_segregated_storage_link_t);
	}
	if ( storage_size%align_size )
	{
		storage_size = align_size*( storage_size/align_size+1 );
	}
	if ( storage_size > memory_size )
	{
		return false;
	}
	

	byte_t*                           pointer;
	simple_segregated_storage_link_t* head   ;
	simple_segregated_storage_link_t* link   ;
	size_t                            i      ;
	size_t                            count  ;

	pointer = reinterpret_cast< byte_t*                           >( memory_pointer );			
	head    = reinterpret_cast< simple_segregated_storage_link_t* >( memory_pointer );			
	count   = memory_size / storage_size;

	link = head;
	for( i=1; i<count; i++ )
	{
		link->_next = reinterpret_cast< simple_segregated_storage_link_t* >( pointer + i*storage_size );
		link        = link->_next;
	}
	link->_next = 0; // null-pointer

	_head = head;


	_storage_size = storage_size;

	_tail = link;

	_memory_pointer = reinterpret_cast < byte_t* > ( memory_pointer );
	_memory_size    = memory_size;
	_align_size     = align_size;

	return true;
}
	
void* simple_segregated_storage::allocate (void)
{
	if ( !_head )
	{
		return 0;
	}

	if ( !_head->_next )
	{
		// �� ���� storage�� ������ ��쿡�� �Ҵ����� �ʰ�
		// �߰������� �����ϴ� memory manager���� growing��
		// �� �� �ִ� ��ȸ�� �����Ѵ�.
		return 0;
	}

	simple_segregated_storage_link_t* link;
		
	link  = _head;
	_head = _head->_next;
		
	return reinterpret_cast< void* >(link);			
}

void simple_segregated_storage::deallocate (void* pointer)
{
	if ( !pointer )
	{
		return;
	}

	simple_segregated_storage_link_t* link;

	link  = _head;

	_head = reinterpret_cast< simple_segregated_storage_link_t* >( pointer );
	_head->_next = link;
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// end of "namespace memory"
} 



//===========================================================================
// end of "namespace cx"
} 
