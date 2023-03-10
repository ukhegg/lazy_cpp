# lazy_cpp
lazy value initialization


### initialization 

```
auto t1 = lazy_cpp::lazy_from_value(42);                  //initialization from value
auto t2 = lazy_from_functor([]() -> char { return 'a'; });//initialization from functor
auto t3 = lazy_from_ctor_params<std::string>(42, 'a');    //initialization from direct ctor parameters
auto t4 = lazy_from_ctor_params<std::string>(t1, t2);     //initialization from lazy ctor parameters
```

### shortcuts
when using lazy with std::shared_ptr<T> a `shared_lazy_t<T>` shortcut available with functions 
`shared_lazy_from_value`, `shared_lazy_from_functor`, `shared_lazy_from_ctor_params`  


### type casts
lazy_t<T> can be cast to lazy_t<U> is T is std::convertible_v<T,U> to U.This applies to lazy shared pointers:
```
auto d = shared_lazy_from_ctor_params<derived>();
auto b = static_cast<shared_lazy<base>>(d);
```

### member function calls for class types
using `call(&class::memfn)` method on lazy_t gives us lazy_t result of this function,evaluated only when lazy_t::get() is invoked
```
auto ls = lazy_from_ctor_params<std::string>("hello world");
auto lsubstr = ls.call(&std::string::substr, 0, 5);
assert(!ls.initialized());
assert(lsubstr.get() == "hello");
assert(ls.initialized());
```
### assignment operator
like in ctor,convertible values,functors with convertible return type or convertible lazy_t can be used to be assigned to another lazy_t
```
//compatible value assignment
lazy_t<int> val = lazy_from_value(5);
val = (size_t) 42;

//compatible-type functor assignment
lazy_t<int> val = lazy_from_value(5);
val = []() -> size_t { return 42; };

// compatible lazy_t assignment is shown in `type casts` section

```



