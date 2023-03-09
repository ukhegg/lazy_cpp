//
// Created by ukhegg on 07.03.23.
//

#include <catch2/catch_all.hpp>
#include <lazy_cpp/lazy_t.hpp>

namespace lazy_cpp::tests
{
    TEST_CASE("lazy_t tests")
    {
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
    }
}