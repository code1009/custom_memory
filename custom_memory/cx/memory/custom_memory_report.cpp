/////////////////////////////////////////////////////////////////////////////
//
// File: custom_memory_report.cpp
//
// Created by MOON, Eui-kwon.
// Created on Jul-21st, 2009.
//
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "custom_memory_report.hpp"

#if defined(WIN32)
#include <atlbase.h>
#endif



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{



//===========================================================================
namespace memory
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void custom_memory_report_trace_null (const char* lpFormatStr, ...)
{
}

#if defined(WIN32)
void custom_memory_report_trace (const char* lpFormatStr, ...)
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
#define MODULE_TRACE custom_memory_report_trace
//#define MODULE_TRACE custom_memory_report_trace_null



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
custom_memory_information::custom_memory_information()
{
	_address  = 0;
	_block    = 0;
	_size     = 0;
	_tag      = 0;
	_allocate = 0;
}

custom_memory_information::custom_memory_information(const custom_memory_information& o)
{
	_address  = o._address    ;
	_block    = o._block      ;
	_size     = o._size       ;
	_tag      = o._tag        ;
	_allocate = o._allocate   ;
}

custom_memory_information& custom_memory_information::operator=(const custom_memory_information& rhs)
{
	_address  = rhs._address  ;
	_block    = rhs._block    ;
	_size     = rhs._size     ;
	_tag      = rhs._tag      ;
	_allocate = rhs._allocate ;

	return *this;
}

void custom_memory_information::dump (void)
{
	MODULE_TRACE ("  address=%p block=%d size=%d tag=%d allocate=%d \r\n",
		_address  ,
		_block    ,
		_size     ,
		_tag      ,
		_allocate );
}

	
/////////////////////////////////////////////////////////////////////////////
//
// Class: custom_memory_report
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
custom_memory_report::custom_memory_report()
{
	_list.reserve (1024*512);	
}

custom_memory_report::~custom_memory_report()
{
	reset ();
}

void custom_memory_report::reset (void)
{
	_list .clear ();
	_table.clear ();
}

//===========================================================================
void custom_memory_report::allocate (void* pointer, uint_t block, uint_t size, uint_t tag)
{
	custom_memory_information* p;

	p = _table[ pointer ];
	if (0==p)
	{
		custom_memory_information o;

		_list.push_back (o);

		p = &(_list.at (_list.size()-1));

		_table[ pointer ] = p;
	}

	if ( 0!=p->_allocate )
	{
		MODULE_TRACE ("custom_memory_report::allocate(): invalid allocation = %p:%d \r\n", pointer, p->_allocate );
	}

	p->_address  = pointer;
	p->_block    = block;
	p->_size     = size;
	p->_tag      = tag;

	p->_allocate++;
}

void custom_memory_report::deallocate (void* pointer, uint_t block, uint_t size, uint_t tag)
{
	custom_memory_information* p;

	p = _table[ pointer ];
	if (0!=p)
	{
		if ( ( p->_block != block ) && ( 0!=block ) )
		{
			MODULE_TRACE ("custom_memory_report::deallocate(): invalid block \r\n");
		}
		if ( ( p->_size != size ) && ( 0!=size ) )
		{
			MODULE_TRACE ("custom_memory_report::deallocate(): invalid size \r\n");
		}
		if ( p->_tag !=tag )
		{
			MODULE_TRACE ("custom_memory_report::deallocate(): invalid tag \r\n");
		}
		if ( p->_address !=pointer )
		{
			MODULE_TRACE ("custom_memory_report::deallocate(): invalid pointer \r\n");
		}
		if ( 1!=p->_allocate )
		{
			MODULE_TRACE ("custom_memory_report::deallocate(): invalid deallocation = %p:%d \r\n", pointer, p->_allocate );
		}

		p->_allocate--;
	}
	else
	{
		MODULE_TRACE ("custom_memory_report::deallocate(): pointer not found \r\n");
	}
}

void custom_memory_report::report (void)
{
	MODULE_TRACE ("/////////////////////////////////////////////////////////////////////////////\r\n");
	MODULE_TRACE ("custom_memory_report::report()\r\n");
	MODULE_TRACE ("/////////////////////////////////////////////////////////////////////////////\r\n");
	MODULE_TRACE ("\r\n");

	std::map<uint_t,uint_t> size_list;
	std::map<uint_t,uint_t> block_list;
	std::map<uint_t,uint_t>::iterator it;

	uint_t i, count;
	uint_t size, block;
	uint_t allocation_count;


	MODULE_TRACE ("# allocated list \r\n");
	allocation_count = 0;

	count = _list.size();
	for (i=0; i<count; i++)
	{
		size  = _list[i]._size;
		block = _list[i]._block;

		size_list  [size ]++;
		block_list [block]++;

		if (0!=_list[i]._allocate)
		{
			_list[i].dump();
			allocation_count++;
		}
	}
	MODULE_TRACE ("  allocated count = %d \r\n",allocation_count);
	MODULE_TRACE ("\r\n");


	MODULE_TRACE ("# size list \r\n");
	for (it = size_list.begin(); it != size_list.end(); it++)
	{
		MODULE_TRACE ("  size %d = %d \r\n",
		(*it).first ,
		(*it).second);
	}
	MODULE_TRACE ("\r\n");

	MODULE_TRACE ("# block list \r\n");
	for (it = block_list.begin(); it != block_list.end(); it++)
	{
		MODULE_TRACE ("  block %d = %d \r\n",
		(*it).first ,
		(*it).second);
	}
	MODULE_TRACE ("\r\n");
}



//===========================================================================
// end of "namespace memory"
} 



//===========================================================================
// end of "namespace cx"
} 
