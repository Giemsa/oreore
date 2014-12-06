#ifndef __OREORE_UTILS_STEP_STORAGE_BASICSTORAGE_H__
#define __OREORE_UTILS_STEP_STORAGE_BASICSTORAGE_H__

#include "../Step.h"
#include <sstream>
#include <iostream>
#include <fstream>

/**
 * Basic Storage
 * 文字列やファイルへの出力
 */

namespace oreore
{
    namespace Step
    {
        namespace Srotage
        {
            /**
             * 文字列へのシリアライズを行うストレージ
             * 演算子は専用のものが用意されているため、仮想関数を用いた
             * 多態性を用いることは出来ない。
             */
            class StringStorage final : public Storage
            {
                friend class Storage;
            public:
                using ResultType = std::string;

            private:
                bool is_const;
                union
                {
                    std::string *str;
                    const std::string * const cstr;
                };

                inline const std::string * const getcstr() const { return is_const ? cstr : str; }
                bool setstr(const std::stringstream &ss)
                {
                    if(is_const)
                    {
                        return false;
                    }

                    if(str)
                    {
                        *str = ss.str();
                        return true;
                    }

                    return false;
                }

                bool process(const StepPhase &phase) override
                {
                    if(setstr(getBuf()))
                    {
                        return false;
                    }

                    return true;
                }

                bool process(StepPhase &phase) const override
                {
                    phase.getBuf().str(*getcstr());
                    return true;
                }

            public:
                StringStorage()
                : is_const(false), str(nullptr)
                { }

                StringStorage(std::string &str)
                : is_const(false), str(&str)
                { }

                StringStorage(const std::string &str)
                : is_const(true), cstr(&str)
                { }

                ~StringStorage() = default;

                // custom output
                void process(ResultType &result, const StepPhase &phase)
                {
                    if(setstr(getBuf()))
                    {
                        result = *getcstr();
                        return;
                    }

                    result = getBuf().str();
                }
            };

            /**
             * ファイルへの入出力を行うストレージ
             */
            class FileStorage final : public Storage
            {
            private:
                std::string filename;

                inline const char *getFileName() const { return filename.c_str(); }

                bool process(const StepPhase &phase) override
                {
                    std::ofstream ofs(filename, std::ios::binary);
                    if(!ofs)
                    {
                        return false;
                    }

                    ofs << getBuf().rdbuf() << std::flush;
                    return !ofs.fail();
                }

                bool process(StepPhase &phase) const override
                {
                    std::ifstream ifs(filename, std::ios::binary);
                    if(!ifs)
                    {
                        return false;
                    }

                    phase.getBuf() << ifs.rdbuf();
                    return true;
                }

            public:
                FileStorage(const std::string &filename)
                : filename(filename)
                { }

                FileStorage(const char *filename)
                : filename(filename)
                { }

                ~FileStorage() = default;
            };
        }
    }
}

#endif
