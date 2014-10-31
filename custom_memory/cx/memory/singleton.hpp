#ifndef __CX_MEMORY__SINGLETON__HEADER__
#define __CX_MEMORY__SINGLETON__HEADER__

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{



//===========================================================================
namespace memory
{
	
	

/////////////////////////////////////////////////////////////////////////////
//
// Class: singleton_object_creator<T>
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template <typename T>
class singleton_object_creator
{
public:
	singleton_object_creator();

public:
	inline void do_nothing() const {}
};



/////////////////////////////////////////////////////////////////////////////
//
// Class: singleton<T>
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template <typename T>
class singleton
{
public:
	typedef T object_type;

//private:
//	static singleton_object_creator<T> create_object;

private:
	singleton();

public:
	static object_type& instance ( void )
	{
		static object_type object;

//		create_object.do_nothing();

		return object;
	}
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template <typename T>
singleton_object_creator<T>::singleton_object_creator()
{
	singleton<T>::instance(); 
}

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//template <typename T>
//typename singleton_object_creator<T> singleton<T>::create_object;



//===========================================================================
// end of "namespace cx"
} 



//===========================================================================
// end of "namespace memory"
} 



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// end of "#ifndef __CX_MEMORY__SINGLETON__HEADER__"
#endif 
