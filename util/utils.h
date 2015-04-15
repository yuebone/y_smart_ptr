#ifndef Y_UTILS_H
#define Y_UTILS_H

namespace _y{

#define _null_ptr 0
	
	template<bool> struct CompileTimeAssert;
	template<> struct CompileTimeAssert<true> { };
	
	template<class T> void checked_delete(T* p)
	{
		typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
		delete p;
	}

	template<class T> struct checked_deleter
	{
		typedef void result_type;
		typedef T * argument_type;
		void operator()(T * x) const
		{
			checked_delete(x);
		}
	};


	template<class T> void checked_array_delete(T* p)
	{
		typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
		delete[] p;
	}

	template<class T> struct checked_array_deleter
	{
		typedef void result_type;
		typedef T * argument_type;
		void operator()(T * x) const
		{
			checked_array_delete(x);
		}
	};

}

#endif // !_BON_UTILS_H_