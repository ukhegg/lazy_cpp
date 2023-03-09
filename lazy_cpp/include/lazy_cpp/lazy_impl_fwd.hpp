//
// Created by Zeliukin Nick on 09.03.23.
//

#ifndef LAZY_CPP_PROJ_LAZY_IMPL_FWD_HPP
#define LAZY_CPP_PROJ_LAZY_IMPL_FWD_HPP

namespace lazy_cpp::internal
{
    template<class TValue>
    class i_lazy_impl
    {
    public:
        virtual bool has_value() const = 0;

        virtual bool try_get_value(TValue &value) = 0;

        virtual TValue &get_value() = 0;

        virtual TValue const &get_value() const = 0;
    };

    template<class TValue>
    class initialized_lazy_impl_t : public i_lazy_impl<TValue>
    {
    public:
        explicit initialized_lazy_impl_t(TValue value);

        bool has_value() const override;

        bool try_get_value(TValue &value) override;

        TValue &get_value() override;

        const TValue &get_value() const override;

    private:
        TValue value_;
    };

    template<class TValue>
    class lazy_with_optional_value_impl_base_t : public i_lazy_impl<TValue>
    {
    public:
        bool has_value() const override;

        bool try_get_value(TValue &value) override;

        TValue &get_value() override;

        TValue const &get_value() const override;

    protected:
        virtual TValue initialize_value() = 0;

    private:
        std::optional<TValue> value_;
    };

    template<class TValue>
    class lazy_functional_impl_t : public lazy_with_optional_value_impl_base_t<TValue>
    {
    public:
        using getter_t = std::function<TValue()>;

        explicit lazy_functional_impl_t(getter_t getter);

    protected:
        TValue initialize_value() override;

    private:
        getter_t getter_;
    };

    template<class TValue, class ... TParams>
    class lazy_ctor_impl_t : public lazy_with_optional_value_impl_base_t<TValue>
    {
    public:
        explicit lazy_ctor_impl_t(TParams &&... params);

    protected:
        using ctor_prams_t = std::tuple<TParams...>;

        TValue initialize_value() override;

    private:
        ctor_prams_t ctor_params_;

        template<size_t ... Is>
        TValue construct_value(std::index_sequence<Is...>);

        template<class T>
        auto get_value(T &ctor_param);
    };


    template<class TValue, class ... TParams>
    class shared_lazy_ctor_impl_t : public lazy_with_optional_value_impl_base_t<std::shared_ptr<TValue>>
    {
    public:
        explicit shared_lazy_ctor_impl_t(TParams &&... params);

    protected:
        using ctor_prams_t = std::tuple<TParams...>;

        std::shared_ptr<TValue> initialize_value() override;

    private:
        ctor_prams_t ctor_params_;

        template<size_t ... Is>
        std::shared_ptr<TValue> construct_value(std::index_sequence<Is...>);

        template<class T>
        auto get_value(T &ctor_param);
    };
}

#endif //LAZY_CPP_PROJ_LAZY_IMPL_FWD_HPP
