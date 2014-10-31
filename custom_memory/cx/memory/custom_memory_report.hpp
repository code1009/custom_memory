#ifndef __CX_MEMORY__CUSTOM_MEMORY_REPORT__HEADER__
#define __CX_MEMORY__CUSTOM_MEMORY_REPORT__HEADER__

/////////////////////////////////////////////////////////////////////////////
//
// File: custom_memory_report.hpp
//
// Created by MOON, Eui-kwon.
// Created on Jul-21st, 2009.
//
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <map>
#include <vector>

#include "type.hpp"


/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{



//===========================================================================
namespace memory
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class custom_memory_information
{
public:
	void*  _address   ;
	uint_t _block     ;
	uint_t _size      ;
	uint_t _tag       ;
	int_t  _allocate  ;

public:
	custom_memory_information();
	custom_memory_information(const custom_memory_information& o);
	custom_memory_information& operator=(const custom_memory_information& rhs);

public:
	void dump (void);
};



/////////////////////////////////////////////////////////////////////////////
//
// Class: custom_memory_report
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class custom_memory_report
{
public:
	typedef std::vector<custom_memory_information>       memory_allocation_list;
	typedef std::map<void*, custom_memory_information*>  memory_allocation_table;

	memory_allocation_list  _list ;
	memory_allocation_table _table;

public:
	custom_memory_report();
	~custom_memory_report();

public:
	void allocate   (void* pointer, uint_t block, uint_t size, uint_t tag);
	void deallocate (void* pointer, uint_t block, uint_t size, uint_t tag);

	void report (void);
	void reset (void);
};



//===========================================================================
// end of "namespace memory"
} 



//===========================================================================
// end of "namespace cx"
} 


/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// end of "#ifndef __CX_MEMORY__CUSTOM_MEMORY_REPORT__HEADER__"
#endif 
