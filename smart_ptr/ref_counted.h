#ifndef Y_REF_COUNT_H
#define Y_REF_COUNT_H

#include "../util/utils.h"
#include "default_delete.h"
#include <algorithm>	//for std::swap


namespace _y{




	/*
	atomic_counter
	a counter represents the count of ptr shared
	TODO:support thread safe
	*/
	class atomic_counter {
	public:
		atomic_counter() :count(1){}
		~atomic_counter(){}

		// Increment and return the result after increment atomically
		size_t operator++(){
			return inc();
		}

		size_t inc(){
			return	++count;
		}

		// Decrement and return the result after decrement atomically
		size_t operator--(){
			return dec();
		}

		size_t dec(){
			return	--count;
		}

		// Return current value - atomically
		operator size_t() const
		{
			return count;
		}
	private:
		atomic_counter(atomic_counter const &);//not support
		atomic_counter & operator=(atomic_counter const &);//not support

		size_t	count;
		//void *mutex_;
	};


	/*
	ref_counted
	any class using counted reference can inherit this class
	dispose() function must be implemented
	*/
	class ref_counted
	{
	private:

		ref_counted(ref_counted const &);
		ref_counted & operator= (ref_counted const &);

		atomic_counter use_count_;

	public:

		ref_counted() : use_count_(){}

		virtual ~ref_counted() // nothrow
		{}

		// dispose() is called when use_count_ drops to zero, to release
		// the resources managed by *this.
		virtual void dispose() = 0; // nothrow

		void add_ref()
		{
			++use_count_;
		}

		void release() // nothrow
		{
			--use_count_;
			if (use_count_==0)
				this->dispose();
		}

		size_t use_count() const // nothrow
		{
			return use_count_;
		}

		bool unique() const{
			return use_count_ == 1;
		}
	};


	/*
	a helper for class sp_counted
	*/
	template<class P, class D> 
	class _sp_counted : public ref_counted
	{
		friend class sp_counted ;
	private:

		P* p_; // copy constructor must not throw
		D d_; // copy constructor must not throw

		// pre: d( p ) must not throw

		_sp_counted(P* p, D d) : p_(p), d_(d){ }
		_sp_counted(P* p) : p_(p), d_(){  }

		virtual void dispose() // nothrow
		{
			d_(p_);
		}

		~_sp_counted(){
		}

	};

	/*
	a helper class for class shared_ptr<T> to count
	*/
	class sp_counted{
	public:
		ref_counted* _counted;

		template<class P, class D>
		sp_counted(P* p, D d) : _counted(_null_ptr){
			if (p!=_null_ptr)
				_counted = new _sp_counted<P,D>(p, d);
		}

		template<class P>
		sp_counted(P* p) : _counted(_null_ptr){
			if (p != _null_ptr)
				_counted = new _sp_counted<P,default_delete<P> >(p);
		}

		sp_counted() : _counted(_null_ptr){
		}

		~sp_counted(){
			_destroy();
		}

		inline void _destroy(){
			if (_counted != _null_ptr){
				_counted->release();
				if (_counted->use_count() == 0){
					checked_delete(_counted);//attention
					_counted = _null_ptr;
				}
			}
		}

		sp_counted& operator=(const sp_counted& x){

			//attention
			//avoid self assignment
			if (x._counted!=_counted){
				_destroy();
				_counted = const_cast<ref_counted*>(x._counted);
				if (_counted != _null_ptr)
					_counted->add_ref();
			}
			return *this;
		}

		void swap(sp_counted& x){
			std::swap(_counted, x._counted);
		}

		size_t use_count() const // nothrow
		{
			return _counted == _null_ptr ? 0 : _counted->use_count();
		}

		bool unique() const{
			return _counted == _null_ptr ? false : _counted->unique();
		}
	};
}


#endif