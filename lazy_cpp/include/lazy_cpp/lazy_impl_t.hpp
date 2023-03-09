//
// Created by ukhegg on 07.03.23.
//

#pragma once
#include "lazy_cpp/lazy_impl_fwd.hpp"

namespace lazy_cpp
{
    template<class TValue>
    class lazy_t;
}

namespace lazy_cpp::internal
{
    template<class TValue>
    initialized_lazy_impl_t<TValue>::initialized_lazy_impl_t(TValue value)
            : value_(std::move(value))
    {
    }

    template<class TValue>
    bool initialized_lazy_impl_t<TValue>::has_value() const
    {
        return true;
    }

    template<class TValue>
    bool initialized_lazy_impl_t<TValue>::try_get_value(TValue &value)
    {
        value = this->value_;
        return true;
    }

    template<class TValue>
    TValue &initialized_lazy_impl_t<TValue>::get_value()
    {
        return this->value_;
    }

    template<class TValue>
    const TValue &initialized_lazy_impl_t<TValue>::get_value() const
    {
        return this->value_;
    }


    template<class TValue>
    bool lazy_with_optional_value_impl_base_t<TValue>::has_value() const
    {
        return value_.has_value();
    }

    template<class TValue>
    bool lazy_with_optional_value_impl_base_t<TValue>::try_get_value(TValue &value)
    {
        if (!this->value_.has_value()) return false;
        value = value_.value();
        return true;
    }

    template<class TValue>
    TValue &lazy_with_optional_value_impl_base_t<TValue>::get_value()
    {
        if (!this->value_.has_value()) {
            this->value_ = this->initialize_value();
        }
        return value_.value();
    }

    template<class TValue>
    TValue const &lazy_with_optional_value_impl_base_t<TValue>::get_value() const
    {
        if (!this->value_.has_value()) {
            auto unconst_this = const_cast<lazy_with_optional_value_impl_base_t *>(this);
            unconst_this->value_ = unconst_this->initialize_value();
        }
        return value_.value();
    }

    namespace helper_functions
    {
        template<class T>
        struct is_lazy : std::false_type
        {
        };

        template<class TValue>
        struct is_lazy<::lazy_cpp::lazy_t<TValue>> : std::true_type
        {

        };

        template<class TValue>
        static constexpr auto is_lazy_v = is_lazy<TValue>::value;

        template<class T>
        struct lazy_traits
        {
        };

        template<class T>
        struct lazy_traits<::lazy_cpp::lazy_t<T>>
        {
            using lazy_type = T;
        };
    }

    template<class TValue>
    lazy_functional_impl_t<TValue>::lazy_functional_impl_t(lazy_functional_impl_t::getter_t getter)
            : getter_(std::move(getter))
    {
    }

    template<class TValue>
    TValue lazy_functional_impl_t<TValue>::initialize_value()
    {
        return getter_();
    }

    template<class TValue, class... TParams>
    lazy_ctor_impl_t<TValue, TParams...>::lazy_ctor_impl_t(TParams &&... params)
            : ctor_params_(std::forward<TParams>(params)...)
    {

    }

    template<class TValue, class... TParams>
    TValue lazy_ctor_impl_t<TValue, TParams...>::initialize_value()
    {
        return this->construct_value(std::make_index_sequence<std::tuple_size_v<ctor_prams_t>>());
    }

    template<class TValue, class... TParams>
    template<size_t... Is>
    TValue lazy_ctor_impl_t<TValue, TParams...>::construct_value(std::index_sequence<Is...>)
    {
#ifdef NDEBUG
        return TValue{get_value(std::get<Is>(this->ctor_params_))...};
#else
        auto ctor_params = std::make_tuple(get_value(std::get<Is>(this->ctor_params_))...);
        return std::make_from_tuple<TValue>(ctor_params);
#endif
    }

    template<class TValue, class... TParams>
    template<class T>
    auto lazy_ctor_impl_t<TValue, TParams...>::get_value(T &ctor_param)
    {
        //if T is shared_ptr<lazy_impl_t<...>>
        if constexpr (helper_functions::is_lazy_v<T>) {
            using lazy_type = typename helper_functions::lazy_traits<T>::lazy_type;

            lazy_t<lazy_type> &l(ctor_param);
            return l.get();
        } else {
            return ctor_param;
        }
    }

    template<class TValue, class... TParams>
    shared_lazy_ctor_impl_t<TValue, TParams...>::shared_lazy_ctor_impl_t(TParams &&... params)
            : ctor_params_(std::forward<TParams>(params)...)
    {

    }

    template<class TValue, class... TParams>
    std::shared_ptr<TValue> shared_lazy_ctor_impl_t<TValue, TParams...>::initialize_value()
    {
        return this->construct_value(std::make_index_sequence<std::tuple_size_v<ctor_prams_t>>());
    }

    template<class TValue, class... TParams>
    template<size_t... Is>
    std::shared_ptr<TValue> shared_lazy_ctor_impl_t<TValue, TParams...>::construct_value(std::index_sequence<Is...>)
    {
        return std::make_shared<TValue>(get_value(std::get<Is>(this->ctor_params_))...);
    }

    template<class TValue, class... TParams>
    template<class T>
    auto shared_lazy_ctor_impl_t<TValue, TParams...>::get_value(T &ctor_param)
    {
        if constexpr (helper_functions::is_lazy_v<T>) {
            using lazy_type = typename helper_functions::lazy_traits<T>::lazy_type;

            lazy_t<lazy_type> &l(ctor_param);
            return l.get();
        } else {
            return ctor_param;
        }
    }
}
