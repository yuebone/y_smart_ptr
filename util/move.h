//
// Created by bon on 15-4-14.
//

#ifndef Y_SMART_PTR_MOVE_H
#define Y_SMART_PTR_MOVE_H

namespace _y{


    template <class T>
    class _move{
    public:
        typedef T  type;
        typedef T& type_ref;

        type_ref ref;

        _move(type_ref r):ref(r){}
    };


    template <class T>
    inline _move<T> move(T& t){
        return _move<T>(t);
    }
}

#endif //Y_SMART_PTR_MOVE_H
