#include "JSON.h"
#include "../../../libs/picojson/picojson.h"

namespace oreore
{
    namespace Step
    {
        bool JSON::serialize(std::ostream &stream) const
        {
            picojson::value v;
            if(!serialize(v))
            {
                return false;
            }

            v.serialize(std::ostream_iterator<char>(stream));
            return !stream.fail();
        }

        bool JSON::deserialize(const std::istream &stream)
        {
            picojson::value v;
            std::string err;
            std::istreambuf_iterator<char> it(stream.rdbuf());
            picojson::parse(v, it, std::istreambuf_iterator<char>(), &err);
            return deserialize(v, err);
        }
    }
}
