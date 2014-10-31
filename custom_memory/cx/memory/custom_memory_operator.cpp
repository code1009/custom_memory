/////////////////////////////////////////////////////////////////////////////
//
// File: custom_memory_operator.cpp
//
// Created by MOON, Eui-kwon.
// Created on Jul-21st, 2009.
//
/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <stdio.h>
#include <memory>

#include "custom_memory_api.hpp"

#include "custom_memory_operator.hpp"



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*

# new/delete의 3가지 형식
1. Single-object forms
2. Array forms
3. Placement forms



# new/delete의 3가지 동작
+-------------------+-------------------+-------------------+-------------------+-------------------+-------------------+
|                   |                   |                   |                   |                   |                   |
|                   | 추가 매개변수     | 메모리할당여부    | 실패가능          | 예외              | 전역 교체가능     |
|                   |                   |                   |                   |                   | (연산자오버로딩)  |
|                   |                   |                   |                   |                   |                   |
+-------------------+-------------------+-------------------+-------------------+-------------------+-------------------+
| new               | 없음              | O 할당            | O 예외 던짐       | std::bad_alloc    | O 가능            |
+-------------------+-------------------+-------------------+-------------------+-------------------+-------------------+
| nothrow new       | std::nothrow_t    | O 할당            | O null 반환       | X                 | O 가능            |
+-------------------+-------------------+-------------------+-------------------+-------------------+-------------------+
| placement new     | void*             | X 할당하지않음    | X                 | X                 | X 불가능          |
+-------------------+-------------------+-------------------+-------------------+-------------------+-------------------+



# placement new
- 전역에 연산자오버로딩 함수는 구현이 되지 않는다.
- 함수 프로토타입
void* operator new     (std::size_t size, void* place) throw();
void* operator new[]   (std::size_t size, void* place) throw();
void  operator delete  (void* object,     void* place) throw();
void  operator delete[](void* object,     void* place) throw();
- 코드
=============================================================================
void* p;                 // memory placement pointer
T*    q;                 // object pointer
p = ::operator new(5);   // allocate only!
q = new (p)T();          // construct
q->~T();                 // deconstruct: YOUR responsibility
// delete (p) q;         // <- does not exist!! It would invoke the following line:
::operator delete(p, q); // does nothing!
::operator delete(q);    // deallocate
=============================================================================

*/



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define CUSTOM_MEMORY_OPERATOR__TRACE 1
#define CUSTOM_MEMORY_OPERATOR__CPPRT 0
#define CUSTOM_MEMORY_OPERATOR__CRT   0



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{



//===========================================================================
namespace memory
{



/////////////////////////////////////////////////////////////////////////////
//
// Function
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace custom_memory_operator_helper
{

static inline void* _allocate (std::size_t size)
{
#if CUSTOM_MEMORY_OPERATOR__CRT
	return malloc (size);
#else	
	return custom_memory_allocate (size);
#endif
}

static inline void _deallocate (void* object)
{
#if CUSTOM_MEMORY_OPERATOR__CRT
	free (object);
#else	
	custom_memory_deallocate (object);
#endif
}

//===========================================================================
// end of "namespace custom_memory_operator_helper"
}




/////////////////////////////////////////////////////////////////////////////
//
// Class: custom_memory_operator
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
custom_memory_operator::custom_memory_operator()
{
}

custom_memory_operator::~custom_memory_operator()
{
}

//===========================================================================
void* __MSC__CDECL custom_memory_operator::operator new (std::size_t size) /*throw(std::bad_alloc)*/
{
#if CUSTOM_MEMORY_OPERATOR__TRACE
	printf ("[overload] new(%d)\r\n", size);
#endif

#if CUSTOM_MEMORY_OPERATOR__CPPRT
	return ::operator new(size);
#else
	return custom_memory_operator_helper::_allocate (size);
#endif
}

void* __MSC__CDECL custom_memory_operator::operator new[] (std::size_t size) /*throw(std::bad_alloc)*/
{
#if CUSTOM_MEMORY_OPERATOR__TRACE
	printf ("[overload] new[](%d)\r\n", size);
#endif

#if CUSTOM_MEMORY_OPERATOR__CPPRT
	return ::operator new[](size);
#else
	return custom_memory_operator_helper::_allocate (size);
#endif
}

void __MSC__CDECL custom_memory_operator::operator delete (void* object) throw()
{
#if CUSTOM_MEMORY_OPERATOR__TRACE
	printf ("[overload] delete(%p)\r\n", object);
#endif

#if CUSTOM_MEMORY_OPERATOR__CPPRT
	::operator delete(object);
#else
	custom_memory_operator_helper::_deallocate (object);
#endif
}

void __MSC__CDECL custom_memory_operator::operator delete[] (void* object) throw()
{
#if CUSTOM_MEMORY_OPERATOR__TRACE
	printf ("[overload] delete[](%p)\r\n", object);
#endif

#if CUSTOM_MEMORY_OPERATOR__CPPRT
	::operator delete[](object);
#else
	custom_memory_operator_helper::_deallocate (object);
#endif
}

//===========================================================================
void* __MSC__CDECL custom_memory_operator::operator new (std::size_t size, const std::nothrow_t& e) throw()
{
#if CUSTOM_MEMORY_OPERATOR__TRACE
	printf ("[overload] new(%d,e)\r\n", size);
#endif

#if CUSTOM_MEMORY_OPERATOR__CPPRT
	return ::operator new(size, e);
#else
	return custom_memory_operator_helper::_allocate (size);
#endif
}

void* __MSC__CDECL custom_memory_operator::operator new[] (std::size_t size, const std::nothrow_t& e) throw()
{
#if CUSTOM_MEMORY_OPERATOR__TRACE
	printf ("[overload] new[](%d,e)\r\n", size);
#endif

#if CUSTOM_MEMORY_OPERATOR__CPPRT
	return ::operator new[](size, e);
#else
	return custom_memory_operator_helper::_allocate (size);
#endif
}

void __MSC__CDECL custom_memory_operator::operator delete (void* object, const std::nothrow_t& e) throw()
{
#if CUSTOM_MEMORY_OPERATOR__TRACE
	printf ("[overload] delete(%p,e)\r\n", object);
#endif

#if CUSTOM_MEMORY_OPERATOR__CPPRT
	::operator delete(object,e);
#else
	custom_memory_operator_helper::_deallocate (object);
#endif
}

void __MSC__CDECL custom_memory_operator::operator delete[] (void* object, const std::nothrow_t& e) throw()
{
#if CUSTOM_MEMORY_OPERATOR__TRACE
	printf ("[overload] delete[](%p,e)\r\n", object);
#endif

#if CUSTOM_MEMORY_OPERATOR__CPPRT
	::operator delete[](object,e);
#else
	custom_memory_operator_helper::_deallocate (object);
#endif
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// end of namespace memory
} 



//===========================================================================
// end of namespace cx
} 
