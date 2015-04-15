cmake_minimum_required(VERSION 2.8)
project(y_smart_ptr)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main.cpp)

add_executable(y_smart_ptr ${SOURCE_FILES} y_smart_ptr.h util/move.h smart_ptr/unique_ptr.h smart_ptr/default_delete.h smart_ptr/simple_ptr.h)