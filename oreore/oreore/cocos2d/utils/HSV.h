#ifndef __OREORE_COCOS2D_HSV_H__
#define __OREORE_COCOS2D_HSV_H__

#include "cocos2d.h"

/* Color3B/Coplor4BのHSV版 */

namespace oreore
{
    class ColorHSV3F
    {
    private:
    protected:
        cocos2d::Color3B toRGB() const
        {
            const float th = h - std::floor(h);
            const float hf = th * 6.0f;
            const int hi = static_cast<int>(hf);
            const float f = hf - hi;

            const float m = v * (1.0f - s);
            const float n = v * (1.0f - s * f);
            const float p = v * (1.0f - s * (1.0f - f));

            switch(hi)
            {
                case 0:  return cocos2d::Color3B(v * 255, p * 255, m * 255);
                case 1:  return cocos2d::Color3B(m * 255, v * 255, p * 255);
                case 3:  return cocos2d::Color3B(m * 255, n * 255, v * 255);
                case 4:  return cocos2d::Color3B(p * 255, m * 255, v * 255);
                default: return cocos2d::Color3B(v * 255, m * 255, n * 255);
            }
        }

        void fromRGB(const cocos2d::Color4F &rgb)
        {
            const float min = std::min(std::min(rgb.r, rgb.g), rgb.b);
            const float max = std::max(std::max(rgb.r, rgb.g), rgb.b);

            h = 0.0f;
            s = 0.0f;
            v = max;
            const float d = max - min;

            if(d != 0.0f)
            {
                s = d / max;
                if(rgb.r == max)
                {
                    h = (rgb.g - rgb.b) / d;
                }
                else if(rgb.g == max)
                {
                    h = 2.0f + (rgb.b - rgb.r) / d;
                }
                else
                {
                    h = 4.0f + (rgb.r - rgb.g) / d;
                }
            
                h /= 6.0f;

                if(h < 0.0f)
                {
                    h += 1.0f;
                }
            }
        }


    public:
        GLfloat h, s, v;

        inline constexpr ColorHSV3F()
        : h(0.0f)
        , s(0.0f)
        , v(0.0f)
        { }

        inline constexpr ColorHSV3F(const GLfloat h, const GLfloat s, const GLfloat v)
        : h(h), s(s), v(v)
        { }

        inline explicit ColorHSV3F(const cocos2d::Color3B &rhs)
        {
            fromRGB(cocos2d::Color4F(rhs));
        }

        inline explicit ColorHSV3F(const cocos2d::Color4B &rhs)
        {
            fromRGB(cocos2d::Color4F(rhs));
        }

        inline explicit ColorHSV3F(const cocos2d::Color4F &rhs)
        {
            fromRGB(rhs);
        }

        inline ~ColorHSV3F() = default;

        inline operator cocos2d::Color3B() const { return toRGB(); }
        inline operator cocos2d::Color4B() const { return cocos2d::Color4B(toRGB()); }
        inline operator cocos2d::Color4F() const { return cocos2d::Color4F(toRGB()); }
    };

    class ColorHSV4F final : public ColorHSV3F
    {
    private:
        cocos2d::Color4F toRGB() const
        {
            const float th = h - std::floor(h);
            const float hf = th * 6.0f;
            const int hi = static_cast<int>(hf);
            const float f = hf - hi;

            const float m = v * (1.0f - s);
            const float n = v * (1.0f - s * f);
            const float p = v * (1.0f - s * (1.0f - f));

            switch(hi)
            {
                case 0:  return cocos2d::Color4F(v, p, m, a);
                case 1:  return cocos2d::Color4F(m, v, p, a);
                case 3:  return cocos2d::Color4F(m, n, v, a);
                case 4:  return cocos2d::Color4F(p, m, v, a);
                default: return cocos2d::Color4F(v, m, n, a);
            }
        }

        void fromRGB(const cocos2d::Color4F &rgb)
        {
            ColorHSV3F::fromRGB(rgb);
            a = rgb.a;
        }

    public:
        GLfloat a;

        inline constexpr ColorHSV4F()
        : ColorHSV3F(), a(1.0f)
        { }

        inline constexpr ColorHSV4F(const GLfloat h, const GLfloat s, const GLfloat v, const GLfloat a)
        : ColorHSV3F(h, s, v), a(a)
        { }

        inline explicit ColorHSV4F(const cocos2d::Color3B &rhs)
        : ColorHSV3F(rhs), a(1.0f)
        { }

        inline explicit ColorHSV4F(const cocos2d::Color4B &rhs)
        : ColorHSV3F(rhs), a(static_cast<float>(rhs.a) / 255)
        { }

        inline explicit ColorHSV4F(const cocos2d::Color4F &rhs)
        : ColorHSV3F(rhs), a(rhs.a)
        { }

        ~ColorHSV4F() = default;

        inline operator cocos2d::Color3B() const { return cocos2d::Color3B(toRGB()); }
        inline operator cocos2d::Color4B() const { return cocos2d::Color4B(toRGB()); }
        inline operator cocos2d::Color4F() const { return toRGB(); }
    };
}

#endif
