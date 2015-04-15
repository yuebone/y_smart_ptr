y_smart_ptr.
is part of a larger project dbapi.

implementations of  smart pointers,including shared_ptr,unique_ptr.
these smart ptrs are designed for C++ compilers version below 2011.
y::shared_ptr and y::unique_ptr,those classes' APIs are compatible with
std::shared_ptr and std::unique_ptr in C++11.

since older C++ compilers do not support &&,std::move etc.,
y::shared_ptr and y::unique_ptr APIs unable to to keep fully compatibility
with C++11,actually y::shared_ptr and y::unique_ptr APIs are subset of
std::shared_ptr and std::unique_ptr APIs.

in practical terms,project can use macro __cplusplus detecting to select
y::shared_ptr and y::unique_ptr or std::shared_ptr and std::unique_ptr.

except y::shared_ptr and y::unique_ptr,we implemented some other smart ptr,
like y::simple_ptr.