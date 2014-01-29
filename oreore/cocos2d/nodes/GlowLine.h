#ifndef __OREORE_COCOS2D_GLOWLINE_H__
#define __OREORE_COCOS2D_GLOWLINE_H__

#include "cocos2d.h"
#include "../../null.h"

namespace oreore
{
    class CCGlowLine : public cocos2d::CCNode, public cocos2d::CCRGBAProtocol
    {
        typedef std::vector<cocos2d::CCPoint> Points;
    private:
        Points points;
        cocos2d::CCPoint src, dst;
        cocos2d::ccColor3B color;
        int level;
        float lineWidth;

        GLubyte _displayedOpacity, _realOpacity;
        cocos2d::ccColor3B _displayedColor, _realColor;
        bool  _cascadeOpacityEnabled, _cascadeColorEnabled;

    public:
        CREATE_FUNC(CCGlowLine);

        static CCGlowLine *create(const cocos2d::CCPoint &p, const cocos2d::CCPoint &q) { return create(p, q, 5); }
        static CCGlowLine *create(const cocos2d::CCPoint &p, const cocos2d::CCPoint &q, const int segments);

        CCGlowLine() { }
        ~CCGlowLine() { }

        bool init(); // override
        bool init(const cocos2d::CCPoint &p, const cocos2d::CCPoint &q, const int segments);
        void draw(); // override

        void setColor(const cocos2d::ccColor3B& color) {  this->color = color; } // override
        const cocos2d::ccColor3B& getColor() { return color;} // override
        GLubyte getOpacity() { return _realOpacity; } // override
        GLubyte getDisplayedOpacity() { return _displayedOpacity; } // override
        void setOpacity(GLubyte opacity) { _realOpacity = opacity; } // override
        void updateDisplayedOpacity(GLubyte parentOpacity) { _displayedOpacity = parentOpacity; } // override
        bool isCascadeOpacityEnabled() { return _cascadeOpacityEnabled; } // override
        void setCascadeOpacityEnabled(bool cascadeOpacityEnabled) { _cascadeOpacityEnabled = cascadeOpacityEnabled; } // override

        const cocos2d::ccColor3B& getDisplayedColor() { return _displayedColor; } // override
        void updateDisplayedColor(const cocos2d::ccColor3B& parentColor) { _displayedColor = parentColor; } // override
        bool isCascadeColorEnabled() { return _cascadeColorEnabled; } // override
        void setCascadeColorEnabled(bool cascadeColorEnabled) { _cascadeColorEnabled = cascadeColorEnabled; } // override

        void setOpacityModifyRGB(bool bValue) { CC_UNUSED_PARAM(bValue); } // override
        bool isOpacityModifyRGB() { return false; } // override

        inline int getLevel() const { return level; }
        inline void setLevel(const int n) { level = n; }

        inline int getSegments() const { return static_cast<int>(points.size()); }
        void setSegments(const int seg);

        void setPoints(const cocos2d::CCPoint &p, const cocos2d::CCPoint &q);
        inline void setDestination(const cocos2d::CCPoint &p) { setPoints(src, p); }

        inline float getLineWidth() const { return lineWidth; }
        inline void setLineWidth(const float w) { lineWidth = w; }

        inline void addPoint(const cocos2d::CCPoint &p) { points.push_back(p); }
        inline void clear() { points.clear(); }
    };
}

#endif
