#ifndef __OREORE_UTILS_STEP_STORAGE_FILESTORAGE_H__
#define __OREORE_UTILS_STEP_STORAGE_FILESTORAGE_H__

#include "../Step.h"
#include <fstream>

namespace oreore
{
    namespace Step
    {
        /**
         * ファイルからデータを読みだしてストリームを構築、
         * またはストリームの内容をファイルへ書き出します。
         */
        class FileStorage final : public Storage<FileStorage>
        {
        private:
            std::string filename;

            bool start(Stream &stream) override
            {
                std::ifstream ifs(filename);
                if(!ifs)
                {
                    return false;
                }

                stream.getStream() << ifs.rdbuf();
                return !stream.getStream().fail();
            }

            bool end(Stream &stream) override
            {
                std::ofstream ofs(filename);
                if(!ofs)
                {
                    return false;
                }

                ofs << stream.getStream().rdbuf();

                return !ofs.fail();
            }

        public:
            FileStorage(const std::string &filename)
            : filename(filename)
            { }

            ~FileStorage() = default;
        };
    }
}


#endif
