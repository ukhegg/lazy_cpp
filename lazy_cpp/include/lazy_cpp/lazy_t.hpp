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

    private:
        using functional_impl_t = internal::lazy_functional_impl_t<TValue>;
        using preset_impl_t = internal::initialized_lazy_impl_t<TValue>;
        using ctor_lazy_impl_t = internal::lazy_ctor_impl_t<TValue>;

        impl_ptr impl_;
    };

    template<class TValue>
    lazy_t<TValue> lazy_from_value(TValue &&value)
    {
        using impl_t = internal::initialized_lazy_impl_t<TValue>;
        return lazy_t<TValue>(std::make_shared<impl_t>(std::forward<TValue>(value)));
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
}

#include "lazy_cpp/lazy_impl_t.hpp"

