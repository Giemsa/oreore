#ifndef __OREORE_UTILS_VARIANT_HPP__
#define __OREORE_UTILS_VARIANT_HPP__

#include <type_traits>
#include <stdexcept>

namespace oreore
{
    class invalid_type_exception : public std::logic_error
    {
    public:
        invalid_type_exception()
        : std::logic_error("invalid type specified")
        { }
    };

    namespace detail
    {
        template<size_t ...N>
        struct max;

        template<size_t N>
        struct max<N>
        {
            static constexpr size_t value = N;
        };

        template<size_t N, size_t ...R>
        class max<N, R...>
        {
        private:
            static constexpr size_t r = max<R...>::value;
        public:
            static constexpr size_t value = (N > r) ? N : r;
        };

        template<typename N, typename ...L>
        class get_index
        {
            template<int I, typename F, typename ...T>
            struct get_index_
            {
                static constexpr int value = -1;
            };

            template<int I, typename F, typename H, typename ...T>
            struct get_index_<I, F, H, T...>
            {
                static constexpr int value =
                    std::is_same<F, H>::value ?
                        I :
                        get_index_<I + 1, F, T...>::value;
            };
        public:
            static constexpr int value = get_index_<0, N, L...>::value;
        };

        template<typename N, typename ...L>
        struct find
        {
            static constexpr bool value = get_index<N, L...>::value >= 0;
        };

        template<bool C, typename T, typename S>
        struct select
        {
            using type = S;
        };

        template<typename T, typename S>
        struct select<true, T, S>
        {
            using type = T;
        };

        template<size_t N, typename ...L>
        class get
        {
            template<size_t I, size_t J, typename ...T>
            struct get_
            {
                using type = void;
            };

            template<size_t I, size_t J, typename H, typename ...T>
            struct get_<I, J, H, T...>
            {
                using type = typename select<I == J, H, typename get_<I, J + 1, T...>::type>::type;
            };
        public:
            using type = typename get_<N, 0, L...>::type;
        };
    }

    /**
     * @class variant
     * @brief 簡易版のboost::variant
     */
    template<typename ...T>
    class variant final
    {
        using Storage = typename std::aligned_storage<
            detail::max<sizeof(T)...>::value,
            detail::max<std::alignment_of<T>::value...>::value
        >::type;

        template<int N, typename ...L>
        struct Destructor
        {
            using U = typename detail::get<N, L...>::type;

            template<typename S>
            static void call(const int index, S &storage)
            {
                if(index == N)
                {
                    reinterpret_cast<U *>(&storage)->~U();
                    return;
                }

                Destructor<N - 1, L...>::call(index, storage);
            }
        };

        template<typename ...L>
        struct Destructor<0, L...>
        {
            using U = typename detail::get<0, L...>::type;

            template<typename S>
            static void call(const int index, S &storage)
            {
                if(index == 0)
                {
                    reinterpret_cast<U *>(&storage)->~U();
                }
            }
        };

    private:
        int type_;
        Storage storage_;

    public:
        variant()
        : type_(-1)
        { }

        template<
            typename U,
            typename R = typename std::remove_cv<
                typename std::remove_reference<U>::type
            >::type,
            typename = typename std::enable_if<detail::find<R, T...>::value>::type
        >
        variant(U &&data)
        : type_(detail::get_index<R, T...>::value)
        {
            new(&storage_) R(std::forward<U>(data));
        }

        ~variant()
        {
            Destructor<sizeof...(T) - 1, T...>::call(type_, storage_);
        }

        template<
            typename U,
            typename = typename std::enable_if<detail::find<U, T...>::value>::type
        >
        U &get()
        {
            if(type_ != detail::get_index<U, T...>::value)
            {
                throw invalid_type_exception();
            }

            return *reinterpret_cast<U *>(&storage_);
        }

        template<
            typename U,
            typename = typename std::enable_if<detail::find<U, T...>::value>::type
        >
        const U &get() const
        {
            if(type_ != detail::get_index<U, T...>::value)
            {
                throw invalid_type_exception();
            }

            return *reinterpret_cast<const U *>(&storage_);
        }

        template<
            typename U,
            typename R = typename std::remove_cv<
                typename std::remove_reference<U>::type
            >::type,
            typename = typename std::enable_if<detail::find<R, T...>::value>::type
        >
        variant &operator=(U &&data)
        {
            Destructor<sizeof...(T) - 1, T...>::call(type_, storage_);
            new(&storage_) R(std::forward<U>(data));
            type_ = detail::get_index<R, T...>::value;
            return *this;
        }
    };
}

#endif
