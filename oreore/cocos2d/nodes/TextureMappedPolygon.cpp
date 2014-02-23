#include "TextureMappedPolygon.h"


namespace oreore
{
    using namespace cocos2d;

	/* TextureMappedPolygon */
	bool TextureMappedPolygon::init()
	{
		if(!Node::init())
			return false;

		texture = null;
        setAnchorPoint(Point::ANCHOR_MIDDLE);
		setShaderProgram(CCShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));
		//setTexture(GameManager::texture(SpriteIndex::Debug));
		//setContentSize(getTexture()->getContentSize());

		return true;
	}

	void TextureMappedPolygon::calcTexCoord()
	{
		const float r = /*Director::getInstance()->getContentScaleFactor()*/ 1.0f / texture->getPixelsWide();
		texCoord.clear();
		for(Points::iterator it = coord.begin(); it != coord.end(); ++it)
			texCoord.push_back(Vertex2F(it->x * r, 1.0f - it->y * r));
	}

	void TextureMappedPolygon::setBlendFunc(BlendFunc blendFunc)
	{
		this->blendFunc = blendFunc;
	}
	
	void TextureMappedPolygon::setTexture(Texture2D* texture2D)
	{
		CCAssert(texture2D, "texture is null");
		CC_SAFE_RELEASE(texture);
		
		texture = texture2D;
		CC_SAFE_RETAIN(texture);
		
		Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT };
		texture->setTexParameters(texParams);
		setContentSize(getTexture()->getContentSize());
		
		updateBlendFunc();
		calcTexCoord();
	}

    void TextureMappedPolygon::setTextureCoord(const Points &points)
    {
        coord = points;
        calcTexCoord();
    }

	void TextureMappedPolygon::updateBlendFunc()
	{
		if(!texture || !texture->hasPremultipliedAlpha())
		{
			blendFunc.src = GL_SRC_ALPHA;
			blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
		}
		else
		{
			blendFunc.src = CC_BLEND_SRC;
			blendFunc.dst = CC_BLEND_DST;
		}
	}
	
	void TextureMappedPolygon::cleanup()
	{
		CC_SAFE_RELEASE_NULL(texture);
		Node::cleanup();
	}
	
	void TextureMappedPolygon::draw()
	{
		if(texCoord.size() <= 1)
			return;

		cmd.init(_vertexZ);
		cmd.func = [this]() {
			CC_NODE_DRAW_SETUP();

			GL::bindTexture2D(texture->getName());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			GL::blendFunc(blendFunc.src, blendFunc.dst);

			GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORDS);
			glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, &coord[0]);
			glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, &texCoord[0]);
			glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(texCoord.size()));
			CC_INCREMENT_GL_DRAWS(1);
		};
		Director::getInstance()->getRenderer()->addCommand(&cmd);
	}
}