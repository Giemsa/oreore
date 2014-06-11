#include "Formatter.h"
#include "../../dlog.h"
#include <iomanip>

namespace oreore
{
    using namespace cocos2d;

    std::ostream& operator <<(std::ostream& os, const Point &point)
    {
        DLog::initFlags();
        os << "(";
        DLog::outputFormat(point.x);
        os << ",";
        DLog::outputFormat(point.y);
        os << ")";
        return os;
    }

    std::ostream& operator <<(std::ostream& os, const Size &size)
    {
        DLog::initFlags();
        os << "(";
        DLog::outputFormat(size.width);
        os << ",";
        DLog::outputFormat(size.height);
        os << ")";
        return os;
    }

    std::ostream& operator <<(std::ostream& os, const Rect &rect)
    {
        DLog::initFlags();
        os << "(";
        DLog::outputFormat(rect.getMinX());
        os << ",";
        DLog::outputFormat(rect.getMinY());
        os << ",";
        DLog::outputFormat(rect.getMaxX());
        os << ",";
        DLog::outputFormat(rect.getMaxY());
        os << ")";
        return os;
    }
}
