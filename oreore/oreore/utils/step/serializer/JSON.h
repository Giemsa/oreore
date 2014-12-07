#ifndef __OREORE_STEP_SERIALIZER_JSON_H__
#define __OREORE_STEP_SERIALIZER_JSON_H__

#include "../Step.h"
#include "../../../libs/picojson/picojson.h"

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
            bool serialize(std::ostream &stream) const override
            {
                picojson::value v;
                if(!serialize(v))
                {
                    return false;
                }

                v.serialize(std::ostream_iterator<char>(stream));
                return !stream.fail();
            }

            bool deserialize(const std::istream &stream) override
            {
                picojson::value v;
                std::string err;
                std::istreambuf_iterator<char> it(stream.rdbuf());
                picojson::parse(v, it, std::istreambuf_iterator<char>(), &err);
                return deserialize(v, err);
            }

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
