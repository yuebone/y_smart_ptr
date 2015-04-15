//
// Created by bon on 15-4-14.
//

#ifndef Y_SMART_PTR_UNIQUE_PTR_H


#include <algorithm>	/*for std::swap*/
#include "../util/utils.h"
#include "default_delete.h"
#include "../util/move.h"

namespace _y {


    /*
    unique_ptr
    */
    template<class T, class D = default_delete <T> >
    class unique_ptr {
    private:
        typedef T &element_ref;
        typedef unique_ptr<T, D> this_type;

        unique_ptr &operator=(const unique_ptr &);//not supported
    public:

        typedef T element_type;
        typedef T *pointer;
        typedef D deleter_type;

        unique_ptr() : p(_null_ptr), d() { }

        explicit unique_ptr(pointer p_) : p(p_), d() { }

        ~unique_ptr() { _destroy(); }

        unique_ptr(_move<this_type> _m) : p(_null_ptr), d() { swap(_m.ref); }

        /*
         * FIX ME!
         *
         * never use this constructor explicitly,
         * it's to enable converting implicitly
         * like expression unique_ptr<T> newPtr=move(otherPtr),
         * which converts _move<this_type> to const unique_ptr,
         * and then call unique_ptr(const unique_ptr &cup) implicitly.
         *
         * so never use expression like unique_ptr<T> newPtr=otherPtr,
         * where otherPtr defined before,
         * since it's incompatible with C++11.
         * use unique_ptr<T> newPtr=move(otherPtr) instead.
         *
         * */
        unique_ptr(const unique_ptr &cup): p(_null_ptr), d() {
            unique_ptr &up=const_cast<unique_ptr&>(cup);
            swap(up);
        }

        unique_ptr &operator=(_move<this_type> _m) {
            //avoid self assignment
            if (p != _m.ref.p) {
                _destroy();
                swap(_m.ref);
            }
            return *this;
        }

        element_ref operator*() const {
            return *p;
        }

        pointer operator->() const {
            return p;
        }

        void reset(pointer p_ = pointer()) {
            if (p_ == p)
                return;

            _destroy();
            p=p_;
        }

        void swap(unique_ptr &_up) {
            std::swap(_up.p, p);
            std::swap(_up.d, d);
        }

        pointer get() const {
            return p;
        }

        operator bool() const {
            return p != _null_ptr;
        }

        pointer release() {
            pointer r = p;
            p = _null_ptr;
            return r;
        }

    private:

        inline void _destroy() {
            if (p != _null_ptr) {
                d(p);
                p = _null_ptr;
            }
        }

        pointer p;
        deleter_type d;
    };

    /*
    unique_ptr specialized for array
    not support -> or *,instead of []
    */
    template<class T, class D>
    class unique_ptr<T[], D> {
    private:
        typedef T &element_ref;
        typedef unique_ptr<T, D> this_type;

        unique_ptr &operator=(const unique_ptr &);//not supported
    public:

        typedef T element_type;
        typedef T *pointer;
        typedef D deleter_type;

        unique_ptr() : p(_null_ptr), d() { }

        explicit unique_ptr(pointer p_) : p(p_), d() { }

        ~unique_ptr() { _destroy(); }

        unique_ptr(_move<this_type> _m) : p(_null_ptr), d() { swap(_m.ref); }

        /*
         * FIX ME!
         *
         * never use this constructor explicitly,
         * it's to enable converting implicitly
         * like expression unique_ptr<T> newPtr=move(otherPtr),
         * which converts _move<this_type> to const unique_ptr,
         * and then call unique_ptr(const unique_ptr &cup) implicitly.
         *
         * so never use expression like unique_ptr<T> newPtr=otherPtr,
         * where otherPtr defined before,
         * since it's incompatible with C++11.
         * use unique_ptr<T> newPtr=move(otherPtr) instead.
         *
         * */
        unique_ptr(const unique_ptr &cup): p(_null_ptr), d() {
            unique_ptr &up=const_cast<unique_ptr&>(cup);
            swap(up);
        }

        unique_ptr &operator=(_move<this_type> _m) {
            //avoid self assignment
            if (p != _m.ref.p) {
                _destroy();
                swap(_m.ref);
            }
            return *this;
        }

        element_ref operator[](std::ptrdiff_t index) {
            //assert index>=0
            //assert p!=0;
            return p[index];
        }

        void reset(pointer p_ = pointer()) {
            if (p_ == p)
                return;

            _destroy();
            p=p_;
        }


        void swap(unique_ptr &_up) {
            std::swap(_up.p, p);
            std::swap(_up.d, d);
        }

        pointer get() const {
            return p;
        }

        operator bool() const {
            return p != _null_ptr;
        }

        pointer release() {
            pointer r = p;
            p = _null_ptr;
            return r;
        }

    private:

        inline void _destroy() {
            if (p != _null_ptr) {
                d(p);
                p = _null_ptr;
            }
        }

        pointer p;
        deleter_type d;
    };

    template<class T, class D>
    void swap(unique_ptr<T, D> &x, unique_ptr<T, D> &y) {
        x.swap(y);
    }
}

#define Y_SMART_PTR_UNIQUE_PTR_H

#endif //Y_SMART_PTR_UNIQUE_PTR_H
