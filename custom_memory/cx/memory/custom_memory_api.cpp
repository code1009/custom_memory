/////////////////////////////////////////////////////////////////////////////
//
// File: custom_memory_api.cpp
//
// Created by MOON, Eui-kwon.
// Created on Jul-21st, 2009.
//
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "singleton.hpp"
#include "custom_memory.hpp"



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{



//===========================================================================
namespace memory
{



/////////////////////////////////////////////////////////////////////////////
#if 0
/////////////////////////////////////////////////////////////////////////////


	
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template class singleton<custom_memory>;



/////////////////////////////////////////////////////////////////////////////
//
// Function
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool custom_memory_create(void)
{
	return singleton<custom_memory>::instance().create();
}

bool custom_memory_destroy(void)
{
	return singleton<custom_memory>::instance().destroy();
}

void custom_memory_dump(void)
{
	singleton<custom_memory>::instance().dump();
}

//===========================================================================
void* custom_memory_stl_allocate(unsigned long int size)
{
	return singleton<custom_memory>::instance().stl_allocate(size);
}

void custom_memory_stl_deallocate(void* pointer, unsigned long int size)
{
	singleton<custom_memory>::instance().stl_deallocate(pointer, size);
}

//===========================================================================
void* custom_memory_allocate(unsigned long int size)
{
	return singleton<custom_memory>::instance().allocate(size);
}

void custom_memory_deallocate(void* pointer)
{
	singleton<custom_memory>::instance().deallocate(pointer);
}

void* custom_memory_reallocate (void* pointer, unsigned long int size)
{
	return singleton<custom_memory>::instance().reallocate(pointer, size);
}



/////////////////////////////////////////////////////////////////////////////
#else
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static custom_memory g_custom_memory;



/////////////////////////////////////////////////////////////////////////////
//
// Function
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool custom_memory_create(void)
{
	return g_custom_memory.create();
}

bool custom_memory_destroy(void)
{
	return g_custom_memory.destroy();
}

void custom_memory_dump(void)
{
	g_custom_memory.dump();
}

//===========================================================================
void* custom_memory_stl_allocate(unsigned long int size)
{
	return g_custom_memory.stl_allocate(size);
}

void custom_memory_stl_deallocate(void* pointer, unsigned long int size)
{
	g_custom_memory.stl_deallocate(pointer, size);
}

//===========================================================================
void* custom_memory_allocate(unsigned long int size)
{
	return g_custom_memory.allocate(size);
}

void custom_memory_deallocate(void* pointer)
{
	g_custom_memory.deallocate(pointer);
}

void* custom_memory_reallocate (void* pointer, unsigned long int size)
{
	return g_custom_memory.reallocate(pointer, size);
}



/////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// end of "namespace memory"
} 



//===========================================================================
// end of "namespace cx"
} 
