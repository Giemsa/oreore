#include "BasicStorage.h"


namespace oreore
{
    namespace Step
    {
        namespace Srotage
        {
            const std::string operator>>(const Serializable &data, StringStorage &out)
            {
                std::ostringstream oss;
                data.serialize(oss);
                if(out.setString(oss))
                {
                    return *out.getString();
                }

                return oss.str();
            }

            const std::string operator>>(const Serializable &data, const StringStorage &out)
            {
                std::ostringstream oss;
                data.serialize(oss);
                return oss.str();
            }
        }
    }
}