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
                friend const std::string operator>>(const Serializable &, StringStorage &);
                friend const std::string operator>>(const Serializable &, const StringStorage &);

            private:
                bool is_const;
                union
                {
                    std::string *str;
                    const std::string * const cstr;
                };

                inline std::string *getString() { return str; }
                inline const std::string * const getString() const { return cstr; }
                bool setString(const std::ostringstream &oss)
                {
                    if(is_const)
                    {
                        return false;
                    }

                    if(str)
                    {
                        *str = oss.str();
                        return true;
                    }

                    return false;
                }

                /* unused */
                bool store(const Encrypter &enc) const override
                {
                    return false;
                }

                /* unused */
                bool store(const Serializable &data) const override
                {
                    return false;
                }

                bool restore(Encrypter &enc) const override
                {
                    return true;
                }

                bool restore(Serializable &data) const override
                {
                    std::istringstream iss;
                    if(is_const)
                    {
                        iss.str(*cstr);
                    }
                    else
                    {
                        iss.str(*str);
                    }

                    return data.deserialize(iss);
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
            };

            const std::string operator>>(const Serializable &data, StringStorage &out);
            const std::string operator>>(const Serializable &data, const StringStorage &out);

            /**
             * ファイルへの入出力を行うストレージ
             * ファイルからの読み込みは実装中
             */
            class FileStorage final : public Storage
            {
            private:
                std::string filename;

                inline const char *getFileName() const { return filename.c_str(); }

                bool store(const Encrypter &enc) const override
                {
                    return true;
                }

                bool store(const Serializable &data) const override
                {
                    std::ofstream ofs(filename);
                    return data.serialize(ofs);
                }

                bool restore(Encrypter &enc) const override
                {
                    return true;
                }

                bool restore(Serializable &data) const override
                {
                    std::ifstream ifs(filename);
                    return data.deserialize(ifs);
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
