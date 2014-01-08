#include "GlowLine.h"
#include "../../Utils.h"
#include <cstdlib>

namespace oreore
{
    using namespace cocos2d;
    GlowLine *GlowLine::create(const Point &p, const Point &q, const int segments)
    {
        GlowLine *line = new GlowLine();
        if(line && line->init())
        {
            line->autorelease();
            return line;
        }
        delete line;
        return null;
    }

    /* GlowLine */
    bool GlowLine::init()
    {
        if(!Node::init())
            return false;
        setLevel(15);
        setSegments(5);
        setLineWidth(2.0f);
        _displayedOpacity = _realOpacity = 255;
        _displayedColor = _realColor = Color3B::WHITE;
        setCascadeOpacityEnabled(false);
        setCascadeColorEnabled(false);
        return true;
    }

    bool GlowLine::init(const Point &p, const Point &q, const int segments)
    {
        if(!Node::init())
            return false;
        src = p;
        dst = q;
        setLevel(15);
        setSegments(segments);
        setLineWidth(2.0f);
        _displayedOpacity = _realOpacity = 255;
        _displayedColor = _realColor = Color3B::WHITE;
        setCascadeOpacityEnabled(false);
        setCascadeColorEnabled(false);
        return true;
    }

    void GlowLine::setSegments(const int seg)
    {
        points.clear();
        const Point v = (dst - src) / seg;
        for(int i = 0; i < seg - 1; i++)
            points.push_back(src + v * i);
        points.push_back(dst);
    }

    void GlowLine::setPoints(const Point &p, const Point &q)
    {
        src = p;
        dst = q;
        setSegments(points.size());
    }

    void GlowLine::draw()
    {
        Node::draw();

        Points vecs;
        vecs.reserve(points.size());
        Points::iterator it = points.begin();
        const int rl = level * 2;
        for(int i = 1; it != points.end(); ++it, i++)
        {
            if(1 < i && i < points.size())
                vecs.push_back(Point(it->x + random(rl) - level, it->y + random(rl) - level));
            else
                vecs.push_back(*it);
        }
        Point *ps = &vecs[0];

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        for(int i = 4; i >= 0; i--)
        {
            glLineWidth(lineWidth + i * (static_cast<float>(std::rand() % 10 + 5) / 5));
            DrawPrimitives::setDrawColor4B(
                color.r, color.g, color.b,
                static_cast<GLubyte>(
                    static_cast<float>((std::rand() % 0x30) + 15)
                ) * (static_cast<float>(_realOpacity) / 0xFF));
            DrawPrimitives::drawPoly(ps, vecs.size(), false);
        }
        glLineWidth(lineWidth);
        DrawPrimitives::setDrawColor4B(
            0xFF, 0xFF, 0xFF,
            static_cast<GLubyte>(static_cast<float>(0x80) * (static_cast<float>(_realOpacity) / 0xFF))
        );
        DrawPrimitives::drawPoly(ps, vecs.size(), false);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}
