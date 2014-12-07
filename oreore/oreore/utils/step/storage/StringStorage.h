#ifndef __OREORE_UTILS_STEP_STORAGE_STRINGSTORAGE_H__
#define __OREORE_UTILS_STEP_STORAGE_STRINGSTORAGE_H__

#include "../Step.h"

namespace oreore
{
    namespace Step
    {
        class StringStorage final : public Storage<StringStorage>
        {
        private:
            bool is_const;

            union
            {
                std::string *buf;
                const std::string *cbuf;
            };

            bool start(Stream &stream) override
            {
                stream.getStream().str(is_const ? *cbuf : *buf);
                return true;
            }

            bool end(Stream &stream) override
            {
                if(is_const)
                {
                    return false;
                }

                *buf = stream.getStream().str();
                return true;
            }

        public:
            StringStorage(std::string &str)
            : buf(&str), is_const(false)
            { }

            StringStorage(const std::string &str)
            : cbuf(&str), is_const(true)
            { }

            ~StringStorage() = default;
        };
    }
}

#endif
