#ifndef __OREORE_COCOS2D_TEXTUREMAPPEDPOLYGON_H__
#define __OREORE_COCOS2D_TEXTUREMAPPEDPOLYGON_H__

#include "cocos2d.h"
#include "../../null.h"

namespace oreore
{
    class TextureMappedPolygon : public cocos2d::Node
    {
    public:
		using Points = std::vector<cocos2d::Vertex2F>;
    private:
		Points texCoord, coord;
		cocos2d::Texture2D *texture;
		cocos2d::BlendFunc blendFunc;
		cocos2d::CustomCommand cmd;
        
		void calcTexCoord();
	protected:
		virtual bool init() override;
    public:
		CREATE_FUNC(TextureMappedPolygon);

		TextureMappedPolygon() { }
		virtual ~TextureMappedPolygon() { }

		void setTexture(cocos2d::Texture2D* texture2D);
		inline cocos2d::Texture2D* getTexture() const { return texture; }
        void setTextureCoord(const Points &points);
		void updateBlendFunc();
		void setBlendFunc(cocos2d::BlendFunc blendFunc);
		virtual void cleanup() override;
		virtual void draw() override;
    };
}

#endif
