#ifndef __OREORE_TUTORIAL_DATACONNECTOR_H__
#define __OREORE_TUTORIAL_DATACONNECTOR_H__

#include "cocos2d.h"
#include "../../utils/any.hpp"
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

        /**
         * @class DataConnector
         * @brief チュートリアルで使用する情報を登録します。
         * ここに登録された値は、チュートリアル内から参照することが出来ます
         */
        template<typename T>
        class DataConnector
        {
            using DataList = std::unordered_map<int, any>;
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
