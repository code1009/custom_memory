#ifndef __CX_MEMORY__CUSTOM_MEMORY_STL_ALLOCATOR__HEADER__
#define __CX_MEMORY__CUSTOM_MEMORY_STL_ALLOCATOR__HEADER__

/////////////////////////////////////////////////////////////////////////////
//
// File: custom_memory_stl_allocator.hpp
//
// Created by MOON, Eui-kwon.
// Created on Jul-21st, 2009.
//
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <memory>

#include "custom_memory_api.hpp"



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{



//===========================================================================
namespace memory
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template <class T> class custom_memory_stl_allocator;
template <class T, class U> bool operator==(const custom_memory_stl_allocator<T>&, const custom_memory_stl_allocator<U>&) throw();
template <class T, class U> bool operator!=(const custom_memory_stl_allocator<T>&, const custom_memory_stl_allocator<U>&) throw();



/////////////////////////////////////////////////////////////////////////////
//
// Class: custom_memory_stl_allocator < void >
//
// specialize for void
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template <> 
class custom_memory_stl_allocator<void>
{
public:
	typedef void        value_type;
	typedef void*       pointer;
	typedef const void* const_pointer;
	// reference-to-void members are impossible. 

public:
	template <class U>
	struct rebind
	{
		typedef custom_memory_stl_allocator<U> other;
	};
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace custom_memory_stl_allocator_helper
{
    inline void destruct(char*   ){}    
	inline void destruct(wchar_t*){}

    template <typename T> 
	inline void destruct(T *pointer)
	{
		pointer->~T();
	}
}



/////////////////////////////////////////////////////////////////////////////
//
// Class: custom_memory_stl_allocator < T >
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template <class T> 
class custom_memory_stl_allocator
{
public:
	typedef T                value_type;
	typedef T*               pointer;
	typedef T&               reference;
	typedef const T*         const_pointer;
	typedef const T&         const_reference;
	typedef ::std::size_t    size_type;
	typedef ::std::ptrdiff_t difference_type;

public:
	template <class U> 
	struct rebind
	{
		typedef custom_memory_stl_allocator<U> other;
	};

public:
	custom_memory_stl_allocator() throw()
	{
	}

	custom_memory_stl_allocator(const custom_memory_stl_allocator&) throw()
	{
	}


	template <class U> 
	custom_memory_stl_allocator(const custom_memory_stl_allocator<U>&) throw()
	{
	}

public:
	~custom_memory_stl_allocator() throw()
	{
	}

public:
	pointer address(reference x) const
	{
		return &value;
	}

	const_pointer address(const_reference x) const
	{
		return &value;
	}

public:
	pointer allocate(size_type count, custom_memory_stl_allocator<void>::const_pointer hint = 0)
	{
		// Throws: bad_alloc if the storage cannot be obtained
		// return reinterpret_cast<pointer>( ::operator new(count*sizeof(T) )); 

		(void)hint;  // Ignore the hint.

		void* p = custom_memory_stl_allocate(count*sizeof(T));

		return reinterpret_cast< pointer >(p);
	}

	// Ask allocator to release memory at pointer with size bytes.
	void deallocate(pointer p, size_type count)
	{
		// ::operator delete(p);

		custom_memory_stl_deallocate(p, count*sizeof(T));
	}

public:
	// Calculate max # of elements allocator can handle.
	size_type max_size() const throw()
	{
		// A good optimizer will see these calculations always produce the same
		// value and optimize this function away completely.
		const size_type max_bytes = size_type(-1);
		const size_type max_count = max_bytes / sizeof(T);

		return max_count;
	}

public:
	// Construct an element at the pointer.
	void construct(pointer p, const T& value)
	{
		// A call to global placement new forces a call to copy constructor.

		// placement new
		:: new((void*)p) T(value);
	}

	// Destruct the object at pointer.
	void destroy(pointer p)
	{
		/*
		// If the Type has no destructor, then some compilers complain about
		// an unreferenced parameter, so use the void cast trick to prevent
		// spurious warnings.
		(void)p;

		((T*)p)->~T();
		*/
		custom_memory_stl_allocator_helper::destruct(p);
	}
};



/////////////////////////////////////////////////////////////////////////////
//
// custom_memory_stl_allocator globals 
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template <class T, class U>
bool operator==(const custom_memory_stl_allocator<T>&, const custom_memory_stl_allocator<U>&) throw()
{
	return true;
}

template <class T, class U> 
bool operator!=(const custom_memory_stl_allocator<T>&, const custom_memory_stl_allocator<U>&) throw()
{
	return false;
}



//===========================================================================
// end of "namespace memory"
} 



//===========================================================================
// end of "namespace cx"
} 



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// end of "#ifndef __CX_MEMORY__CUSTOM_MEMORY_STL_ALLOCATOR__HEADER__"
#endif 
