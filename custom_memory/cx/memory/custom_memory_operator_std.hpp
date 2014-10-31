#ifndef __CX_MEMORY__CUSTOM_MEMORY_OPERATOR_STD__HEADER__
#define __CX_MEMORY__CUSTOM_MEMORY_OPERATOR_STD__HEADER__

/////////////////////////////////////////////////////////////////////////////
//
// File: custom_memory_operator_std.hpp
//
// Created by MOON, Eui-kwon.
// Created on Jul-21st, 2009.
//
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if defined(__CX_MEMORY_NO_STL__)



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace std
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if defined(__64BIT) || defined(_WIN64)
typedef unsigned long long int size_t;
#else
typedef unsigned int size_t;
#endif



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// placement new tag type to suppress exceptions
struct nothrow_t
{	
};

//===========================================================================
// constant for placement new tag
extern const nothrow_t nothrow;	



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// end of "namespace std"
} 



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// end of "#if defined(__CX_MEMORY_NO_STL__)"
#endif 



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// end of "#ifndef __CX_MEMORY__CUSTOM_MEMORY_OPERATOR_STD__HEADER__"
#endif 