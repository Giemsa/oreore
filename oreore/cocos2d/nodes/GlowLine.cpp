#include "GlowLine.h"
#include "../../Utils.h"
#include <cstdlib>

namespace oreore
{
    using namespace cocos2d;
    CCGlowLine *CCGlowLine::create(const CCPoint &p, const CCPoint &q, const int segments)
    {
        CCGlowLine *line = new CCGlowLine();
        if(line && line->init())
        {
            line->autorelease();
            return line;
        }
        delete line;
        return null;
    }

    /* CCGlowLine */
    bool CCGlowLine::init()
    {
        if(!CCNode::init())
            return false;
        setLevel(15);
        setSegments(5);
        setLineWidth(2.0f);
        _displayedOpacity = _realOpacity = 255;
        _displayedColor = _realColor = ccWHITE;
        setCascadeOpacityEnabled(false);
        setCascadeColorEnabled(false);
        return true;
    }

    bool CCGlowLine::init(const CCPoint &p, const CCPoint &q, const int segments)
    {
        if(!CCNode::init())
            return false;
        src = p;
        dst = q;
        setLevel(15);
        setSegments(segments);
        setLineWidth(2.0f);
        _displayedOpacity = _realOpacity = 255;
        _displayedColor = _realColor = ccWHITE;
        setCascadeOpacityEnabled(false);
        setCascadeColorEnabled(false);
        return true;
    }

    void CCGlowLine::setSegments(const int seg)
    {
        points.clear();
        const CCPoint v = (dst - src) / seg;
        for(int i = 0; i < seg - 1; i++)
            points.push_back(src + v * i);
        points.push_back(dst);
    }

    void CCGlowLine::setPoints(const CCPoint &p, const CCPoint &q)
    {
        src = p;
        dst = q;
        setSegments(points.size());
    }

    void CCGlowLine::draw()
    {
        CCNode::draw();

        Points vecs;
        vecs.reserve(points.size());
        Points::iterator it = points.begin();
        const int rl = level * 2;
        for(int i = 1; it != points.end(); ++it, i++)
        {
            if(1 < i && i < points.size())
                vecs.push_back(CCPoint(it->x + random(rl) - level, it->y + random(rl) - level));
            else
                vecs.push_back(*it);
        }
        CCPoint *ps = &vecs[0];

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        for(int i = 4; i >= 0; i--)
        {
            glLineWidth(lineWidth + i * (static_cast<float>(std::rand() % 10 + 5) / 5));
            ccDrawColor4B(
                color.r, color.g, color.b,
                static_cast<GLubyte>(
                    static_cast<float>((std::rand() % 0x30) + 15)
                ) * (static_cast<float>(_realOpacity) / 0xFF));
            ccDrawPoly(ps, vecs.size(), false);
        }
        glLineWidth(lineWidth);
        ccDrawColor4B(
            0xFF, 0xFF, 0xFF,
            static_cast<GLubyte>(static_cast<float>(0x80) * (static_cast<float>(_realOpacity) / 0xFF))
        );
        ccDrawPoly(ps, vecs.size(), false);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}
