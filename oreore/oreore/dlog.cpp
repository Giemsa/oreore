#include "DLog.h"

#ifndef OREORE_RELEASE
#include <iomanip>
#include <ctime>
#include <cstdio>
#include <sys/time.h>

namespace oreore
{
    void DLog::parseArg(const char *&str)
    {
        ++str;
        while(*str && *str != '}')
        {
            switch(*str)
            {
                case '<':
                    currentFormat.align = TextAlign::Left;
                    break;
                case '>':
                    currentFormat.align = TextAlign::Right;
                    break;
                case '^':
                    currentFormat.align = TextAlign::Center;
                    break;
                case '+':
                    currentFormat.showpos = true;
                    break;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    {
                        int l = 0;
                        while(*str && '0' <= *str && *str <= '9')
                        {
                            l = l * 10 + (*str++ - '0');
                        }
                        currentFormat.len = l;
                        --str;
                    }
                    break;
                case 'd':
                    currentFormat.format = FormatType::_INTEGER;
                    break;
                case 'o':
                    currentFormat.format = FormatType::_OCT;
                    break;
                case 'b':
                    currentFormat.format = FormatType::_BOOL;
                    break;
                case 'x':
                    currentFormat.format = FormatType::_HEX;
                    break;
                case 'X':
                    currentFormat.format = FormatType::_HEX_U;
                    break;
                case 'f':
                    currentFormat.format = FormatType::_FLOAT;
                    break;
                case 's':
                    currentFormat.format = FormatType::_STRING;
                    break;
                case '\\':
                    ++str;
                    /* fall through */
                default:
                    currentFormat.fillChar = *str;
                    break;
            }
            ++str;
        }

        if(*str != '}')
        {
            throw std::runtime_error("invalid format string");
        }
    }

    void DLog::setFlags()
    {
        std::ios::fmtflags flags = os->flags();

        // adjust
        flags &= ~std::ios::adjustfield;
        switch(currentFormat.align)
        {
            case TextAlign::Left:
                flags |= std::ios::left;
                break;
            case TextAlign::Center:
                // TODO
                break;
            case TextAlign::Right:
                flags |= std::ios::right;
                break;
        }

        // width
        os->width(currentFormat.len);

        // format
        flags &= ~std::ios::basefield;
        switch(currentFormat.format)
        {
            case FormatType::_INTEGER:
                flags |= std::ios::dec;
                break;
            case FormatType::_FLOAT:
                flags |= std::ios::showpoint;
                break;
            case FormatType::_HEX_U:
                flags |= std::ios::uppercase;
                /* fall through */
            case FormatType::_HEX:
                flags |= std::ios::hex;
                break;
            case FormatType::_OCT:
                flags |= std::ios::oct;
                break;
            case FormatType::_BOOL:
                flags |= std::ios::boolalpha;
                break;
            default:
                break;
        }

        // fill char
        os->fill(currentFormat.fillChar);

        // showpos
        if(currentFormat.showpos)
        {
            flags |= std::ios::showpos;
        }

        os->flags(flags);
    }

    void DLog::resetFlags()
    {
        os->fill(' ');
        os->width(0);
        os->flags(dfmt);
    }

    void DLog::output(const char *s)
    {
        while(*s)
        {
            switch(*s)
            {
                case '\\':
                    ++s;
                    break;
                case '{':
                case '#':
                    throw std::runtime_error("parameters mismatch.");
                    break;
            }

            (*os) << *s++;
        }

        (*os) << std::endl;
    }

    void DLog::outputTime()
    {
        if(!showTime)
        {
            return;
        }

        timeval tv;
        gettimeofday(&tv, nullptr);
        
        tm *l = localtime(&tv.tv_sec);
        printf("%02d:%02d:%02d.%03d\t", l->tm_hour, l->tm_min, l->tm_sec, static_cast<int>(tv.tv_usec / 1000));
    }
}

#endif
