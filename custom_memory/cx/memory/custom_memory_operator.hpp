#ifndef __CX_MEMORY__CUSTOM_MEMORY_OPERATOR__HEADER__
#define __CX_MEMORY__CUSTOM_MEMORY_OPERATOR__HEADER__

/////////////////////////////////////////////////////////////////////////////
//
// File: custom_memory_operator.hpp
//
// Created by MOON, Eui-kwon.
// Created on Jul-21st, 2009.
//
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if defined(__CX_MEMORY_NO_STL__)
#include "custom_memory_operator_std.hpp"
#else
#include <cstddef>
#include <new>
#endif

#if defined (_MSC_VER)
#define __MSC__CDECL __cdecl
#else
#define __MSC__CDECL 
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
class custom_memory_operator
{
public:
	custom_memory_operator();
	virtual ~custom_memory_operator();

/*
public:
	static inline void* operator new     (std::size_t size, void* place) throw() { return ::operator new      (size,   place); }
	static inline void* operator new[]   (std::size_t size, void* place) throw() { return ::operator new[]    (size,   place); }
	static inline void  operator delete  (void* object,     void* place) throw() { return ::operator delete   (object, place); }
	static inline void  operator delete[](void* object,     void* place) throw() { return ::operator delete[] (object, place); }
*/

public:
	static void* __MSC__CDECL operator new      (std::size_t size); /*throw(std::bad_alloc);*/
	static void* __MSC__CDECL operator new[]    (std::size_t size); /*throw(std::bad_alloc);*/
	static void  __MSC__CDECL operator delete   (void* object)      throw();
	static void  __MSC__CDECL operator delete[] (void* object)      throw();

public:
	static void* __MSC__CDECL operator new      (std::size_t size, const std::nothrow_t& e) throw();
	static void* __MSC__CDECL operator new[]    (std::size_t size, const std::nothrow_t& e) throw();
	static void  __MSC__CDECL operator delete   (void* object,     const std::nothrow_t& e) throw();
	static void  __MSC__CDECL operator delete[] (void* object,     const std::nothrow_t& e) throw();
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
// end of "#ifndef __CX_MEMORY__CUSTOM_MEMORY_OPERATOR__HEADER__"
#endif 
