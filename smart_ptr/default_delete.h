//
// Created by bon on 15-4-14.
//

#ifndef Y_SMART_PTR_DEFAULT_DELETE_H

#include "../util/utils.h"

namespace _y{

    /*
    default deleter for unique ptr
    */
    template <typename T>
    class default_delete {
    public:
        void operator()(T* p) const{
            if (p == _null_ptr)
                return;

            checked_delete(p);
        }
    };

    /*
    default deleter for unique ptr specialized for array
    */
    template <typename T>
    class default_delete<T[]> {
    public:
        void operator()(T* p) const{
            if (p == _null_ptr)
                return;

            checked_array_delete(p);
        }
    };

}


#define Y_SMART_PTR_DEFAULT_DELETE_H

#endif //Y_SMART_PTR_DEFAULT_DELETE_H
