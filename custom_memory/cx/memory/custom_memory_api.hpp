#ifndef __CX_MEMORY__CUSTOM_MEMORY_API__HEADER__
#define __CX_MEMORY__CUSTOM_MEMORY_API__HEADER__

/////////////////////////////////////////////////////////////////////////////
//
// File: custom_memory_api.hpp
//
// Created by MOON, Eui-kwon.
// Created on Jul-21st, 2009.
//
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//===========================================================================



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{



//===========================================================================
namespace memory
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool  custom_memory_create         (void);
bool  custom_memory_destroy        (void);
void  custom_memory_dump           (void);

void* custom_memory_stl_allocate   (unsigned long int size);
void  custom_memory_stl_deallocate (void* pointer, unsigned long int size);

void  custom_memory_deallocate     (void* pointer);
void* custom_memory_allocate       (unsigned long int size);
void* custom_memory_reallocate     (void* pointer, unsigned long int size);


/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// end of "namespace memory"
} 



//===========================================================================
// end of "namespace cx"
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// end of "#ifndef __CX_MEMORY__CUSTOM_MEMORY_API__HEADER__"
#endif 
