#ifndef __OREORE_COCOS2D_TEXTUREMAPPEDPOLYGON_H__
#define __OREORE_COCOS2D_TEXTUREMAPPEDPOLYGON_H__

#include "cocos2d.h"
#include "../../null.h"

namespace oreore
{
    class CCTextureMappedPolygon : public cocos2d::CCNode
    {
    public:
		typedef std::vector<cocos2d::ccVertex2F> Points;
    private:
		Points texCoord, coord;
		cocos2d::CCTexture2D *texture;
		cocos2d::ccBlendFunc blendFunc;
        
		void calcTexCoord();
	protected:
		virtual bool init() override;
    public:
		CREATE_FUNC(CCTextureMappedPolygon);

		CCTextureMappedPolygon() { }
		virtual ~CCTextureMappedPolygon() { }

		void setTexture(cocos2d::CCTexture2D* texture2D);
		inline cocos2d::CCTexture2D* getTexture() const { return texture; }
        void setTextureCoord(const Points &points);
		void updateBlendFunc();
		void setBlendFunc(cocos2d::ccBlendFunc blendFunc);
		virtual void cleanup() override;
		virtual void draw() override;
    };
}

#endif
