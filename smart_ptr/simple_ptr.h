//
// Created by bon on 15-4-14.
//

#ifndef Y_SMART_PTR_SIMPLE_PTR_H
#define Y_SMART_PTR_SIMPLE_PTR_H

#include <cstddef>
#include "../util/utils.h"

namespace _y{
    template <typename T>
    class simple_ptr{
        typedef T &element_ref;
    public:
        typedef simple_ptr<T> this_type;
        typedef T element_type;
        typedef T *pointer;

        simple_ptr():p(_null_ptr){}

        explicit simple_ptr(pointer p_):p(p_){}

        ~simple_ptr(){_destroy();}

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

    private:
        pointer p;

        inline void _destroy() {
            if (p != _null_ptr) {
                checked_delete(p);
                p=_null_ptr;
            }
        }

        simple_ptr(const simple_ptr& sp);
        simple_ptr& operator=(const simple_ptr& sp);
    };



    template <typename T>
    class simple_ptr<T[]>{
        typedef T &element_ref;
    public:
        typedef simple_ptr<T> this_type;
        typedef T element_type;
        typedef T *pointer;

        simple_ptr():p(_null_ptr){}

        explicit simple_ptr(pointer p_):p(p_){}

        ~simple_ptr(){_destroy();}

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

    private:
        pointer p;

        inline void _destroy() {
            if (p != _null_ptr) {
                checked_array_delete(p);
                p=_null_ptr;
            }
        }

        simple_ptr(const simple_ptr& sp);
        simple_ptr& operator=(const simple_ptr& sp);
    };
}

#endif //Y_SMART_PTR_SIMPLE_PTR_H
