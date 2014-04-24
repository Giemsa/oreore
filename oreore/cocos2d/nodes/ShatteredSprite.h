#ifndef __OREORE_COCOS2D_SHATTEREDSPRITE_H__
#define __OREORE_COCOS2D_SHATTEREDSPRITE_H__

#include "cocos2d.h"

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
    
    class ShatteredSprite : public cocos2d::Node
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
        
        int piecesX;
        bool shattered;

		cocos2d::CustomCommand cmd, grabCmd;
        cocos2d::Texture2D *texture;
		cocos2d::BlendFunc blendFunc;

        inline int getIndex(const int x, const int y) const { return x * piecesX + y; }
    protected:
        ShatteredSprite() { }
        virtual ~ShatteredSprite() { }

        bool init(cocos2d::Sprite *sprite, const int piecesX, const int piecesY, const float speedVar, const float rotVar);

        void updateBlendFunc();
        virtual void updateColor() override;
        virtual void cleanup() override;
        virtual void draw(cocos2d::Renderer *renderer, const kmMat4 &parentTransform, bool parentTransformUpdated) override;
    public:
        static ShatteredSprite *create(cocos2d::Sprite *sprite, const int piecesX, const int piecesY, const float speedVar, const float rotVar);
        void shatter();
    };
}

#endif
