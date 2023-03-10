//
// Created by Zeliukin Nick on 07.03.23.
//

#pragma once

#include <memory>
#include "lazy_cpp/lazy_impl_fwd.hpp"

namespace lazy_cpp
{
    template<class TValue>
    class lazy_t
    {
    public:
        using impl_ptr = std::shared_ptr<internal::i_lazy_impl<TValue>>;

        lazy_t() = default;

        explicit lazy_t(impl_ptr impl) : impl_(impl)
        {
        }

        bool initialized() const
        {
            return this->impl_->has_value();
        }

        TValue const &get() const
        {
            return this->impl_->get_value();
        }

        TValue &get()
        {
            return this->impl_->get_value();
        }

        template<class T>
        operator T() const;

    private:
        impl_ptr impl_;
    };

    template<class TValue>
    lazy_t<TValue> lazy_from_value(TValue &&value)
    {
        using impl_t = internal::initialized_lazy_impl_t<TValue>;
        return lazy_t<TValue>(std::make_shared<impl_t>(std::forward<TValue>(value)));
    }

    template<class TValue>
    lazy_t<TValue> lazy_from_value(TValue const &value)
    {
        using impl_t = internal::initialized_lazy_impl_t<TValue>;
        return lazy_t<TValue>(std::make_shared<impl_t>(value));
    }

    template<class TFunction>
    std::enable_if_t<std::is_invocable_v<TFunction>,
            lazy_t<std::invoke_result_t<TFunction>>> lazy_from_functor(TFunction f)
    {
        using value_type = std::invoke_result_t<TFunction>;
        using impl_t = internal::lazy_functional_impl_t<value_type>;
        return lazy_t<value_type>(std::make_shared<impl_t>(std::move(f)));
    }

    template<class TValue, class ... TParams>
    lazy_t<TValue> lazy_from_ctor_params(TParams &&... params)
    {
        using impl_t = internal::lazy_ctor_impl_t<TValue, TParams...>;
        auto impl = std::make_shared<impl_t>(std::forward<TParams>(params)...);
        return lazy_t<TValue>(impl);
    }

    template<class T>
    using shared_lazy = lazy_t<std::shared_ptr<T>>;

    template<class TValue>
    shared_lazy<TValue> shared_lazy_from_value(TValue &&value)
    {
        auto value_impl = std::make_shared<TValue>(std::forward<TValue>(value));
        using impl_t = internal::initialized_lazy_impl_t<std::shared_ptr<TValue>>;
        return lazy_t<std::shared_ptr<TValue>>(std::make_shared<impl_t>(value_impl));
    }

    template<class TValue>
    shared_lazy<TValue> shared_lazy_from_value(TValue const &value)
    {
        auto value_impl = std::make_shared<TValue>(value);
        using impl_t = internal::initialized_lazy_impl_t<std::shared_ptr<TValue>>;
        return lazy_t<std::shared_ptr<TValue>>(std::make_shared<impl_t>(value_impl));
    }

    template<class TFunction>
    std::enable_if_t<std::is_invocable_v<TFunction>,
            shared_lazy<std::invoke_result_t<TFunction>>> shared_lazy_from_functor(TFunction f)
    {
        using value_type = std::invoke_result_t<TFunction>;
        auto another_functor = [f]() -> std::shared_ptr<value_type> {
            return std::make_shared<value_type>(f());
        };
        return lazy_from_functor(another_functor);
    }

    template<class TValue, class ... TParams>
    shared_lazy<TValue> shared_lazy_from_ctor_params(TParams &&... params)
    {
        using impl_t = internal::shared_lazy_ctor_impl_t<TValue, TParams...>;
        auto impl = std::make_shared<impl_t>(std::forward<TParams>(params)...);
        return shared_lazy<TValue>(impl);
    }

    template<class TTarget, class TSource>
    lazy_t<TTarget> lazy_cast(lazy_t<TSource> const &src)
    {
        return static_cast<lazy_t<TTarget>>(src);
    }
}

#include "lazy_cpp/lazy_impl_t.hpp"

namespace lazy_cpp
{
    template<class TValue>
    template<class T>
    lazy_t<TValue>::operator T() const
    {
        static_assert(internal::helper_functions::is_lazy_v<T>, "T must be lazy_t<...>");

        using lazy_type = typename internal::helper_functions::lazy_traits<T>::lazy_type;

        static_assert(std::is_convertible_v<TValue, lazy_type>, "T::lazy_type must be convertable to TValue");
        static_assert(std::is_same_v<T, lazy_t<lazy_type>>, "paranoic check)");
        using impl_t = internal::lazy_functional_impl_t<lazy_type>;
        auto src_impl = this->impl_;

        return lazy_t<lazy_type>(std::make_shared<impl_t>([src_impl]() -> lazy_type {
            return (lazy_type) src_impl->get_value();
        }));
    }
}

