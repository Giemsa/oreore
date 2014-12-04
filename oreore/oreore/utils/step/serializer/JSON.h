#ifndef __OREORE_UTILS_STEP_SERIALIZER_JSON_H__
#define __OREORE_UTILS_STEP_SERIALIZER_JSON_H__

#include "../Step.h"
#include "../../../libs/picojson/picojson.h"

namespace oreore
{
    namespace Step
    {
        namespace Serializer
        {
            /**
             * JSON シリアライザ
             */
            class JSON : public Serializable
            {
            private:
                bool serialize(std::ostream &out) const override
                {
                    picojson::value v;
                    if(!toJSON(v))
                    {
                        return false;
                    }

                    v.serialize(std::ostream_iterator<char>(out));
                    return out.fail();
                }

                bool deserialize(const std::istream &data) override
                {
                    picojson::value v;
                    std::string err;
                    std::istreambuf_iterator<char> it(data.rdbuf());
                    picojson::parse(v, it, std::istreambuf_iterator<char>(), &err);
                    return fromJSON(v, err);
                }

            protected:
                // JSONへのシリアライズ
                virtual bool toJSON(picojson::value &json) const = 0;

                // JSONからのデシリアライズ
                virtual bool fromJSON(const picojson::value &json, const std::string &error) = 0;

            public:
                JSON() = default;
                virtual ~JSON() = default;

            };
        }
    }
}

#endif
