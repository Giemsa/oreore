#include "TextureMappedPolygon.h"


namespace oreore
{
    using namespace cocos2d;

    /* CCTextureMappedPolygon */
    bool CCTextureMappedPolygon::init()
    {
        if(!CCNode::init())
            return false;

        texture = null;
        setAnchorPoint(ccp(0.5f, 0.5f));
        setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));

        return true;
    }

    void CCTextureMappedPolygon::calcTexCoord()
    {
        const float r =  1.0f / texture->getPixelsWide();
        texCoord.clear();
        for(Points::iterator it = coord.begin(); it != coord.end(); ++it)
        {
            const ccVertex2F v2f = { it->x * r, 1.0f - it->y * r };
            texCoord.push_back(v2f);
        }
    }

    void CCTextureMappedPolygon::setBlendFunc(ccBlendFunc blendFunc)
    {
        this->blendFunc = blendFunc;
    }

    void CCTextureMappedPolygon::setTexture(CCTexture2D* texture2D)
    {
        CCAssert(texture2D, "texture is null");
        CC_SAFE_RELEASE(texture);

        texture = texture2D;
        CC_SAFE_RETAIN(texture);

        ccTexParams texParams = { GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT };
        texture->setTexParameters(&texParams);
        setContentSize(getTexture()->getContentSize());

        updateBlendFunc();
        calcTexCoord();
    }

    void CCTextureMappedPolygon::setTextureCoord(const Points &points)
    {
        coord = points;
        calcTexCoord();
    }

    void CCTextureMappedPolygon::updateBlendFunc()
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

    void CCTextureMappedPolygon::cleanup()
    {
        CC_SAFE_RELEASE_NULL(texture);
        CCNode::cleanup();
    }

    void CCTextureMappedPolygon::draw()
    {
        if(texCoord.size() <= 1)
            return;

        CC_NODE_DRAW_SETUP();

        ccGLBindTexture2D(texture->getName());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        ccGLBlendFunc(blendFunc.src, blendFunc.dst);

        ccGLEnableVertexAttribs(kCCVertexAttrib_Position | kCCVertexAttrib_TexCoords);
        glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, &coord[0]);
        glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, &texCoord[0]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(texCoord.size()));
        CC_INCREMENT_GL_DRAWS(1);
    }
}
