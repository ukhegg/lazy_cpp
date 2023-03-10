//
// Created by ukhegg on 07.03.23.
//

#include <catch2/catch_all.hpp>
#include <lazy_cpp/lazy_t.hpp>

namespace lazy_cpp::tests
{
    TEST_CASE("lazy_t tests")
    {
        SECTION("simple test")
        {
            auto t = lazy_cpp::lazy_from_value(5);
            REQUIRE(t.get() == 5);
        }

        SECTION("lazy with functional initializer tests")
        {
            auto lazy_int = lazy_from_functor([]() { return 42; });
            REQUIRE_FALSE(lazy_int.initialized());
            REQUIRE(lazy_int.get() == 42);
            REQUIRE(lazy_int.initialized());
        }

        SECTION("lazy initialized with functor returns same object for second call")
        {
            auto lazy_int = lazy_from_functor([]() { return (int *) 42; });
            REQUIRE(lazy_int.get() == (int *) 42);
            REQUIRE(lazy_int.get() == (int *) 42);
        }

        SECTION("lazy with simple ctor params tests")
        {
            std::string s(10, 'a');
            auto lazy_s = lazy_from_ctor_params<std::string>(10, 'a');

            REQUIRE_FALSE(lazy_s.initialized());
            auto t = lazy_s.get();
            REQUIRE(t == s);
            REQUIRE(lazy_s.initialized());
        }

        SECTION("lazy with simple ctor params constructs object only on first access")
        {
            std::string s(10, 'a');
            auto lazy_s = lazy_from_ctor_params<std::string>(10, 'a');

            REQUIRE(lazy_s.get() == s);
            REQUIRE(lazy_s.get() == s);
        }

        SECTION("can construct lazy with lazy ctor params")
        {
            auto length = lazy_from_value(10);
            auto fill = lazy_from_value('a');

            auto lazy_s = lazy_from_ctor_params<std::string>(length, fill);

            REQUIRE_FALSE(lazy_s.initialized());
            auto t = lazy_s.get();
            REQUIRE(t == "aaaaaaaaaa");
            REQUIRE(lazy_s.initialized());
        }

        SECTION("can construct shared lazy from value")
        {
            auto t = shared_lazy_from_value(5);

            REQUIRE(std::is_same_v<lazy_t<std::shared_ptr<int>>, decltype(t)>);
            REQUIRE(t.initialized());
            REQUIRE(*t.get() == 5);
            REQUIRE(t.initialized());
        }

        SECTION("can construct shared lazy from functor")
        {
            auto t = shared_lazy_from_functor([]() { return 42; });

            REQUIRE(std::is_same_v<lazy_t<std::shared_ptr<int>>, decltype(t)>);
            REQUIRE_FALSE(t.initialized());
            REQUIRE(*t.get() == 42);
            REQUIRE(t.initialized());
        }

        SECTION("can construct shared lazy with lazy params")
        {
            auto length = lazy_from_value(10);
            auto fill = lazy_from_value('a');

            auto lazy_s = shared_lazy_from_ctor_params<std::string>(length, fill);
            REQUIRE(std::is_same_v<lazy_t<std::shared_ptr<std::string>>, decltype(lazy_s)>);

            REQUIRE_FALSE(lazy_s.initialized());
            auto t = lazy_s.get();
            REQUIRE(*t == "aaaaaaaaaa");
            REQUIRE(lazy_s.initialized());
        }

        SECTION("can do narrowing lazy cast")
        {
            lazy_t<size_t> ls = lazy_from_value<size_t>(10);
            auto li = lazy_cast<int>(ls);
            REQUIRE(li.get() == 10);
        }

        SECTION("can do implicit narrowing assign with shared pointers")
        {
            class base
            {
            public:
                virtual ~base() = default;

                virtual int get_42() const
                {
                    return 41;
                }
            };
            class derived : public base
            {
            public:
                int get_42() const override
                {
                    return 42;
                }
            };

            auto d = shared_lazy_from_ctor_params<derived>();
            REQUIRE(std::is_same_v<decltype(d), lazy_cpp::lazy_t<std::shared_ptr<derived>>>);
            shared_lazy<base> b;
            REQUIRE(std::is_same_v<decltype(b), lazy_cpp::lazy_t<std::shared_ptr<base>>>);
            b = static_cast<shared_lazy<base>>(d);

            REQUIRE(b.get()->get_42() == 42);
        }

        SECTION("can do lazy member function call without parameters")
        {
            auto ls = lazy_from_ctor_params<std::string>(10, 'a');
            REQUIRE_FALSE(ls.initialized());
            auto ll = ls.call(&std::string::size);

            REQUIRE(std::is_same_v<lazy_t<size_t>, decltype(ll)>);
            REQUIRE_FALSE(ls.initialized());
            REQUIRE_FALSE(ll.initialized());

            REQUIRE(ll.get() == 10);
            REQUIRE(ls.initialized());
            REQUIRE(ll.initialized());
        }

        SECTION("can do lazy member function call with parameters")
        {
            auto ls = lazy_from_ctor_params<std::string>("hello world");
            REQUIRE_FALSE(ls.initialized());
            auto lsubstr = ls.call(&std::string::substr, 0, 5);

            REQUIRE(std::is_same_v<lazy_t<std::string>, decltype(lsubstr)>);
            REQUIRE_FALSE(ls.initialized());
            REQUIRE_FALSE(lsubstr.initialized());

            REQUIRE(lsubstr.get() == "hello");
            REQUIRE(ls.initialized());
            REQUIRE(lsubstr.initialized());
        }

        SECTION("can assign function to a lazy")
        {
            lazy_t<int> val = lazy_from_value(5);
            val = []() -> size_t { return 42; };
            REQUIRE_FALSE(val.initialized());
            REQUIRE(val.get() == 42);
            REQUIRE(val.initialized());
        }

        SECTION("assign value to lazy")
        {
            lazy_t<int> val = lazy_from_value(5);

            val = (size_t) 42;
            REQUIRE(val.initialized());
            REQUIRE(val.get() == 42);
        }
    }
}