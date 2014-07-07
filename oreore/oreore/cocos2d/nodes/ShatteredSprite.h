#ifndef __OREORE_COCOS2D_SHATTEREDSPRITE_H__
#define __OREORE_COCOS2D_SHATTEREDSPRITE_H__

#include "cocos2d.h"
#include "../utils/Persistent.h"

namespace oreore
{
    /*
     * ShatteredSprite cocos2dx port
     *
     * original post: http://www.cocos2d-iphone.org/forums/topic/breaking-glass-shatteredsprite/
     */

    struct MoveInfo
    {
        cocos2d::Vertex3F axis, vdelta, center;
        float adelta;

        MoveInfo() : axis(), vdelta(), center(), adelta(0.0f) { }
        MoveInfo(const cocos2d::Vertex3F &axis, const cocos2d::Vertex3F &vdelta, const cocos2d::Vertex3F &center, const float adelta) :
            axis(axis), vdelta(vdelta), center(center), adelta(adelta) { }
    };

    struct ShatteredSpriteParam
    {
        int piecesX, piecesY;
        float speedVar, rotVar;
        cocos2d::Rect textureRect;
        cocos2d::Texture2D *texture;
        cocos2d::GLProgram *shader;

        ShatteredSpriteParam() :
            piecesX(0), piecesY(0), speedVar(0.0f), rotVar(0.0f),
            textureRect(), texture(nullptr), shader(nullptr) { }
        ShatteredSpriteParam(
            const int piecesX, const int piecesY, const float speedVar, const float rotVar,
            cocos2d::Texture2D *texture, const cocos2d::Rect &textureRect, cocos2d::GLProgram *shader
        )
         : piecesX(piecesX), piecesY(piecesY), speedVar(speedVar), rotVar(rotVar),
           texture(texture), textureRect(textureRect), shader(shader)
        { }
    };

    class ShatteredSprite : public cocos2d::Sprite
    {
        using Tex2FVec = std::vector<cocos2d::Tex2F>;
        using Vertex3FVec = std::vector<cocos2d::Vertex3F>;
        using Colors = std::vector<cocos2d::Color4B>;
        using MoveInfoVec = std::vector<MoveInfo>;
    private:
        Tex2FVec texCoord;
        Vertex3FVec coord;
        Colors colors;
        MoveInfoVec moveInfoVec;
        Persistent<cocos2d::GLProgram> shader;

        int piecesX;
        bool shattered;

        cocos2d::CustomCommand cmd;

        inline int getIndex(const int x, const int y) const { return x * piecesX + y; }
        void initShatterData(const ShatteredSpriteParam &param);
    protected:
        ShatteredSprite() { }
        virtual ~ShatteredSprite() { }

        bool init(const ShatteredSpriteParam &param);
        bool init() override;

        virtual void updateColor() override;
        virtual void draw(cocos2d::Renderer *renderer, const kmMat4 &parentTransform, bool parentTransformUpdated) override;
    public:
        static ShatteredSprite *create(const ShatteredSpriteParam &param);
        CREATE_FUNC(ShatteredSprite);

        void setParams(const ShatteredSpriteParam &param);
        void shatter();
    };
}

#endif
