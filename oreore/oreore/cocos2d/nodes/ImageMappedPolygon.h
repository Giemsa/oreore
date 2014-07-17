#ifndef __OREORE_COCOS2D_IMAGEMAPPEDPOLYGON_H__
#define __OREORE_COCOS2D_IMAGEMAPPEDPOLYGON_H__

#include "cocos2d.h"

namespace oreore
{
    class ImageMappedPolygonBase : public cocos2d::Node
    {
    public:
        using Points = std::vector<cocos2d::Vertex2F>;
    private:
    protected:
        Points texCoord, coord;
        cocos2d::Texture2D *texture;
        cocos2d::BlendFunc blendFunc;
        GLenum mode;

        ImageMappedPolygonBase() { }
        virtual ~ImageMappedPolygonBase() { }

        virtual bool init() override;
        virtual void calcTexCoord() { }
    public:
        inline cocos2d::Texture2D* getTexture() const { return texture; }
        void setTexture(cocos2d::Texture2D *texture2D);
        void updateBlendFunc();
        inline cocos2d::BlendFunc getBlendFunc() const { return blendFunc; }
        void setBlendFunc(cocos2d::BlendFunc blendFunc);
        void setTextureCoord(const Points &points);

        inline void setMode(const GLenum mode) { this->mode = mode; }
        inline GLenum getMode() const { return mode; }

        virtual void cleanup() override;
    };

    class TextureMappedPolygon : public ImageMappedPolygonBase
    {
    private:
    protected:
        cocos2d::CustomCommand cmd;

        virtual bool init() override;
        void calcTexCoord() override;
    public:
        CREATE_FUNC(TextureMappedPolygon);

        TextureMappedPolygon() { }
        virtual ~TextureMappedPolygon() { }

        virtual void draw(cocos2d::Renderer *renderer, const kmMat4& transform, bool transformUpdated) override;
    };


    class ScreenMappedPolygon : public ImageMappedPolygonBase
    {
    private:
        cocos2d::Grabber *grabber;
        cocos2d::GroupCommand _groupCommand;
        cocos2d::CustomCommand _gridBeginCommand;
        cocos2d::CustomCommand _gridEndCommand;
        cocos2d::Director::Projection _directorProjection;

        void set2DProjection();
        void onBeforeDraw();
        void onAfterDraw();
    protected:
        ScreenMappedPolygon() { }
        virtual ~ScreenMappedPolygon();

        bool init() override;
        void calcTexCoord() override;
    public:
        CREATE_FUNC(ScreenMappedPolygon);

        void visit(cocos2d::Renderer *renderer, const kmMat4 &parentTransform, bool parentTransformUpdated) override;
    };
}

#endif

