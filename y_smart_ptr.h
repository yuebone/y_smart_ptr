//
// Created by bon on 15-4-14.
//

#ifndef Y_SMART_PTR_Y_SMART_PTR_H
#define Y_SMART_PTR_Y_SMART_PTR_H

#if __cplusplus < 201103L

#include "smart_ptr/unique_ptr.h"
#include "smart_ptr/simple_ptr.h"
#include "smart_ptr/shared_ptr.h"

namespace y
{
    using _y::unique_ptr;
    using _y::shared_ptr;
    using _y::move;

    using _y::simple_ptr;
}


#else

#include <memory>
#include "smart_ptr/simple_ptr.h"

namespace y
{
    using std::unique_ptr;
    using std::shared_ptr;
    using std::move;

    using _y::simple_ptr;
}

#endif //__cplusplus


#endif //Y_SMART_PTR_Y_SMART_PTR_H
