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
            bool start(Stream &stream) override
            {
                std::cout << "StringStorage start" << std::endl;
                return true;
            }

            bool end(Stream &stream) override
            {
                std::cout << "StringStorage end" << std::endl;
                return true;
            }

        public:
            StringStorage() = default;
            ~StringStorage() = default;
        };
    }
}

#endif
