#ifndef __COCOS2D_EXT_GLOWLINE_H__
#define __COCOS2D_EXT_GLOWLINE_H__

#include "cocos2d.h"
#include "../null.h"

namespace cocos2d
{

    class GlowLine : public Node, public RGBAProtocol
    {
        typedef std::vector<Point> Points;
    private:
        Points points;
        Point src, dst;
        Color3B color;
        int level;
        float lineWidth;
        
        GLubyte _displayedOpacity, _realOpacity;
        Color3B _displayedColor, _realColor;
        bool  _cascadeOpacityEnabled, _cascadeColorEnabled;
        
    public:
        CREATE_FUNC(GlowLine);

        static GlowLine *create(const Point &p, const Point &q) { return create(p, q, 5); }
        static GlowLine *create(const Point &p, const Point &q, const int segments);
        
        GlowLine() { }
        ~GlowLine() { }
        
        bool init() override;
        bool init(const Point &p, const Point &q, const int segments);
        void draw() override;
        
        
        void setColor(const Color3B& color) override {  this->color = color; }
        const Color3B& getColor() const override { return color;}
        GLubyte getOpacity() const override { return _realOpacity; }
        GLubyte getDisplayedOpacity() const override { return _displayedOpacity; }
        void setOpacity(GLubyte opacity) override { _realOpacity = opacity; }
        void updateDisplayedOpacity(GLubyte parentOpacity) override { _displayedOpacity = parentOpacity; }
        bool isCascadeOpacityEnabled() const override { return _cascadeOpacityEnabled; }
        void setCascadeOpacityEnabled(bool cascadeOpacityEnabled) override { _cascadeOpacityEnabled = cascadeOpacityEnabled; }
        
        const Color3B& getDisplayedColor() const override { return _displayedColor; }
        void updateDisplayedColor(const Color3B& parentColor) override { _displayedColor = parentColor; }
        bool isCascadeColorEnabled() const override { return _cascadeColorEnabled; }
        void setCascadeColorEnabled(bool cascadeColorEnabled) override { _cascadeColorEnabled = cascadeColorEnabled; }
        
        void setOpacityModifyRGB(bool bValue) override { CC_UNUSED_PARAM(bValue); }
        bool isOpacityModifyRGB() const override { return false; }
        
        inline int getLevel() const { return level; }
        inline void setLevel(const int n) { level = n; }
        
        inline int getSegments() const { return static_cast<int>(points.size()); }
        void setSegments(const int seg);
        
        void setPoints(const Point &p, const Point &q);
        inline void setDestination(const Point &p) { setPoints(src, p); }
       
        inline float getLineWidth() const { return lineWidth; }
        inline void setLineWidth(const float w) { lineWidth = w; }
        
        inline void addPoint(const Point &p) { points.push_back(p); }
        inline void clear() { points.clear(); }
    };
}

#endif
