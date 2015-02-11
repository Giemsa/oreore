#ifndef __OREORE_UTILS_ANY_HPP__
#define __OREORE_UTILS_ANY_HPP__

namespace oreore
{
    /**
     * @class any
     * @brief 簡易版のboost::any
     */
    class any final
    {
        struct placeholder_base
        {
            virtual ~placeholder_base() { }
            virtual placeholder_base *clone() const = 0;
        };

        template<typename T>
        class placeholder final : public placeholder_base
        {
        private:
            placeholder(const T &value)
            : value_(value)
            { }
        public:
            T value_;

            placeholder(T &&value)
            : value_(std::forward<T>(value))
            { }

            ~placeholder() { }

            placeholder_base *clone() const
            {
                return new placeholder<T>(value_);
            }
        };
    private:
        placeholder_base *data_;

    public:
        any()
        : data_(nullptr)
        { }

        template<typename T>
        any(T &&value)
        : data_(new placeholder<
            typename std::remove_cv<
                typename std::remove_reference<T>::type
            >::type
        >(std::forward<T>(value)))
        { }

        any(const any &data)
        : data_(data.data_ ? data.data_->clone() : nullptr)
        { }

        any(any &&data)
        : data_(data.data_)
        {
            data.data_ = nullptr;
        }

        ~any()
        {
            delete data_;
        }

        any &operator=(const any& data)
        {
            delete data_;
            data_ = data.data_ ? data.data_->clone() : nullptr;
            return *this;
        }

        template<typename T>
        any &operator=(T &&value)
        {
            delete data_;
            data_ = new placeholder<T>(std::forward<T>(value));
            return *this;
        }

        template<typename T>
        const T &get() const
        {
            return dynamic_cast<placeholder<T> &>(*data_).value_;
        }

        template<typename T>
        T &get()
        {
            return dynamic_cast<placeholder<T> &>(*data_).value_;
        }

        template<typename T>
        bool is() const
        {
            return dynamic_cast<placeholder<T> *>(data_) != nullptr;
        }

        bool empty() const { return data_ != nullptr; }

        void reset()
        {
            delete data_;
            data_ = nullptr;
        }
    };
}

#endif
