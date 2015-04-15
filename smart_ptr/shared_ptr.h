#ifndef Y_SMART_PTR_SHARED_PTR_H
#define Y_SMART_PTR_SHARED_PTR_H

#include "../util/utils.h"
#include "../util/move.h"
#include "ref_counted.h"
#include <cstddef>		//for std::ptrdiff_t
#include <algorithm>	//for std::swap

namespace _y{



	/*
	shared_ptr
	*/
	template <class T> class shared_ptr{
	private:
		typedef T&	element_ref;
		typedef T*	pointer;
		typedef shared_ptr<T> this_type;

	public:
		typedef	T	element_type;

		shared_ptr() :p(_null_ptr){}

		template <class U>
		explicit shared_ptr(U* p_) : p(p_){
			used = sp_counted(p_);
		}

		template<class U, class D> 
		shared_ptr(U * p_, D d_) : p(p_){
			used = sp_counted(p_, d_);
		}

		shared_ptr(const shared_ptr& x) : p(_null_ptr) {
			*this = x;
		}

		shared_ptr& operator= (const shared_ptr& x) {
			if (x.p!=p){
				used = x.used;
				p = x.p;
			}
			return *this;
		}

		template <class U> 
		shared_ptr(const shared_ptr<U>& x) : p(_null_ptr) {
			*this = x;
		}

		template <class U> 
		shared_ptr& operator= (const shared_ptr<U>& x) {
			if (x.p != p){
				used = x.used;
				p = x.p;
			}
			return *this;
		}

		void swap(shared_ptr& x){
			std::swap(p,x.p);
			used.swap(x.used);
		}

		void reset(){
			this_type().swap(*this);
		}

		template <class U> 
		void reset(U* p_){
			this_type(p_).swap(*this);
		}

		template <class U, class D> 
		void reset(U* p_, D d_){
			this_type(p_,d_).swap(*this);
		}

		pointer get() const{
			return p;
		}

		element_ref operator*() const{
			return *p;
		}

		pointer operator->() const{
			return p;
		}

		bool unique() const{
			return used.unique();
		}

		long int use_count() const{
			return (long int)used.use_count();
		}

		operator bool() const{
			return p != _null_ptr;
		}

		~shared_ptr(){
		}

	private:
		pointer p;
		sp_counted used;
	};

	template <class T>
	void swap(shared_ptr<T>& x, shared_ptr<T>& y){
		x.swap(y);
	}



}

#endif