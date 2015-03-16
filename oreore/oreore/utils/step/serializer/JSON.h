#ifndef __OREORE_STEP_SERIALIZER_JSON_H__
#define __OREORE_STEP_SERIALIZER_JSON_H__

#include "../Step.h"

namespace picojson
{
    class value;
}

namespace oreore
{
    namespace Step
    {
        /**
         * JSONシリアライザ
         * このクラスを継承し、serialize/deserializeを継承することで、
         * 任意のデータをJSONにシリアライズすることができます。
         * なお、JSONの操作にはpicojsonを使用します。
         */
        class JSON : public Serializable
        {
        private:
            bool serialize(std::ostream &stream) const override;
            bool deserialize(const std::istream &stream) override;

        protected:
            virtual bool serialize(picojson::value &value) const = 0;
            virtual bool deserialize(const picojson::value &value, const std::string &error) = 0;

        public:
            JSON() = default;
            virtual ~JSON() = default;
        };
    }
}

#endif
