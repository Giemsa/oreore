#ifndef __OREORE_TUTORIAL_DATACONNECTOR_H__
#define __OREORE_TUTORIAL_DATACONNECTOR_H__

#include "cocos2d.h"
#include <exception>
#include <unordered_map>

namespace oreore
{
    namespace Tutorial
    {
        class unregistered_data_exception : public std::runtime_error
        {
        public:
            unregistered_data_exception()
            : std::runtime_error("unregistered id specified")
            { }
        };

        class Data final
        {
            struct PlaceHolderBase
            {
                virtual ~PlaceHolderBase() { }
                virtual PlaceHolderBase *clone() const = 0;
            };

            template<typename T>
            struct PlaceHolder final : public PlaceHolderBase
            {
                T value_;

                PlaceHolder(const T &value)
                : value_(value)
                { }

                PlaceHolder(T &&value)
                : value_(std::forward<T>(value))
                { }

                ~PlaceHolder() { }

                PlaceHolderBase *clone() const
                {
                    return new PlaceHolder<T>(value_);
                }
            };
        private:
            PlaceHolderBase *data_;

        public:
            template<typename T>
            Data(T &&value)
            : data_(new PlaceHolder<
                typename std::remove_cv<
                    typename std::remove_reference<T>::type
                >::type
            >(std::forward<T>(value)))
            { }

            Data(const Data &data)
            : data_(data.data_ ? data.data_->clone() : nullptr)
            { }

            Data(Data &&data)
            : data_(data.data_)
            {
                data.data_ = nullptr;
            }

            ~Data()
            {
                delete data_;
            }

            Data &operator=(const Data& data)
            {
                delete data_;
                data_ = data.data_ ? data.data_->clone() : nullptr;
                return *this;
            }

            template<typename T>
            Data &operator=(T &&value)
            {
                delete data_;
                data_ = new PlaceHolder<T>(std::forward<T>(value));
                return *this;
            }

            template<typename T>
            const T &get() const
            {
                return dynamic_cast<PlaceHolder<T> &>(*data_).value_;
            }

            template<typename T>
            T &get()
            {
                return dynamic_cast<PlaceHolder<T> &>(*data_).value_;
            }
        };

        /**
         * @class DataConnector
         * @brief チュートリアルで使用する情報を登録します。
         * ここに登録された値は、チュートリアル内から参照することが出来ます
         */
        template<typename T>
        class DataConnector
        {
            using DataList = std::unordered_map<int, Data>;
        private:
            DataList dataList;

        public:
            /**
             * @brief 任意のデータをIDに紐付けます
             */
            template<typename U>
            void add(const T id, U &&data)
            {
                dataList.insert(DataList::value_type(static_cast<int>(id), std::forward<U>(data)));
            }

            /**
             * @brief IDに紐付けられたデータを取得します
             */
            template<typename U>
            const U &get(const T id) const
            {
                const DataList::const_iterator it = dataList.find(static_cast<int>(id));
                if(it == dataList.end())
                {
                    throw unregistered_data_exception();
                }

                return it->second.get<U>();
            }

            /**
             * @brief IDに紐付けられたデータを取得します
             */
            template<typename U>
            U &get(const T id)
            {
                const DataList::iterator it = dataList.find(static_cast<int>(id));
                if(it == dataList.end())
                {
                    throw unregistered_data_exception();
                }

                return it->second.get<U>();
            }
        };
    }
}

#endif
