#include "ImageMappedPolygon.h"
#include "2d/ccUtils.h"

namespace oreore
{
    using namespace cocos2d;

    /* ImageMappedPolygonBase */
    bool ImageMappedPolygonBase::init()
    {
        if(!Node::init())
        {
            return false;
        }

        texture = nullptr;
        mode = GL_TRIANGLE_FAN;
        setAnchorPoint(Point::ANCHOR_MIDDLE);
        setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));

        return true;
    }

    void ImageMappedPolygonBase::setTexture(Texture2D *texture2D)
    {
        CCAssert(texture2D, "texture is null");
        CC_SAFE_RELEASE(texture);

        texture = texture2D;
        CC_SAFE_RETAIN(texture);

        Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
        texture->setTexParameters(texParams);
        setContentSize(texture->getContentSize());

        updateBlendFunc();
        calcTexCoord();
    }

    void ImageMappedPolygonBase::updateBlendFunc()
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

    void ImageMappedPolygonBase::cleanup()
    {
        CC_SAFE_RELEASE_NULL(texture);
        Node::cleanup();
    }

    void ImageMappedPolygonBase::setBlendFunc(BlendFunc blendFunc)
    {
        this->blendFunc = blendFunc;
    }

    void ImageMappedPolygonBase::setTextureCoord(const Points &points)
    {
        coord = points;
        calcTexCoord();
    }


    /* TextureMappedPolygon */
    bool TextureMappedPolygon::init()
    {
        if(!ImageMappedPolygonBase::init())
        {
            return false;
        }

        return true;
    }

    void TextureMappedPolygon::calcTexCoord()
    {
        const float rx = CC_CONTENT_SCALE_FACTOR() / texture->getPixelsWide();
        const float ry = CC_CONTENT_SCALE_FACTOR() / texture->getPixelsHigh();
        texCoord.clear();

        for(auto p : coord)
        {
            texCoord.push_back(Vertex2F(p.x * rx, 1.0f - p.y * ry));
        }
    }

    void TextureMappedPolygon::draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated)
    {
        if(texCoord.size() <= 1)
        {
            return;
        }

        cmd.init(getGlobalZOrder());
        cmd.func = [this]() {
            CC_NODE_DRAW_SETUP();

            GL::bindTexture2D(texture->getName());

            GL::blendFunc(blendFunc.src, blendFunc.dst);

            GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORDS);
            glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, &coord[0]);
            glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, &texCoord[0]);
            glDrawArrays(mode, 0, static_cast<GLsizei>(texCoord.size()));
            CC_INCREMENT_GL_DRAWS(1);
        };

        renderer->addCommand(&cmd);
    }


    /* ScreenMappedPolygon */
    ScreenMappedPolygon::~ScreenMappedPolygon()
    {
        delete texture;
        delete grabber;
    }

    bool ScreenMappedPolygon::init()
    {
        if(!ImageMappedPolygonBase::init())
        {
            return false;
        }

        Size s = Director::getInstance()->getWinSizeInPixels();

        int width = 0;
        int height = 0;

        if(Configuration::getInstance()->supportsNPOT())
        {
            width = s.width;
            height = s.height;
        }
        else
        {
            width = ccNextPOT(s.width);
            height = ccNextPOT(s.height);
        }

        const int dataLen = width * height * 4;
        void *data = calloc(dataLen, 1);
        if(!data)
        {
            release();
            return false;
        }

        texture = new Texture2D();
        texture->initWithData(data, dataLen, Texture2D::PixelFormat::RGBA8888, width, height, s);
        free(data);

        grabber = new Grabber();
        grabber->grab(texture);

        const Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
        texture->setTexParameters(texParams);
        setContentSize(s);
        setAnchorPoint(Point::ANCHOR_MIDDLE);
        setPosition(Point(s / 2));

        updateBlendFunc();
        calcTexCoord();

        return true;
    }

    void ScreenMappedPolygon::calcTexCoord()
    {
        const float rx = CC_CONTENT_SCALE_FACTOR() / texture->getPixelsWide();
        const float ry = CC_CONTENT_SCALE_FACTOR() / texture->getPixelsHigh();
        texCoord.clear();

        for(auto p : coord)
        {
            texCoord.push_back(Vertex2F(p.x * rx, p.y * ry));
        }
    }

    void ScreenMappedPolygon::set2DProjection()
    {
        const Size &size = Director::getInstance()->getWinSizeInPixels();

        glViewport(0, 0, static_cast<GLsizei>(size.width), static_cast<GLsizei>(size.height));
        kmGLMatrixMode(KM_GL_PROJECTION);
        kmGLLoadIdentity();

        kmMat4 orthoMatrix;
        kmMat4OrthographicProjection(&orthoMatrix, 0, size.width, 0, size.height, -1, 1);
        kmGLMultMatrix(&orthoMatrix);

        kmGLMatrixMode(KM_GL_MODELVIEW);
        kmGLLoadIdentity();

        GL::setProjectionMatrixDirty();
    }

    void ScreenMappedPolygon::onBeforeDraw()
    {
        _directorProjection = Director::getInstance()->getProjection();
        set2DProjection();

        grabber->beforeRender(texture);
    }

    void ScreenMappedPolygon::onAfterDraw()
    {
        grabber->afterRender(texture);
        Director::getInstance()->setProjection(_directorProjection);

        _shaderProgram->use();
        _shaderProgram->setUniformsForBuiltins();

        GL::bindTexture2D(texture->getName());

        GL::blendFunc(getBlendFunc().src, getBlendFunc().dst);
        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORDS);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, &coord[0]);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, &texCoord[0]);
        glDrawArrays(mode, 0, static_cast<GLsizei>(texCoord.size()));
        CC_INCREMENT_GL_DRAWS(1);
    }

    void ScreenMappedPolygon::visit(cocos2d::Renderer *renderer, const kmMat4 &parentTransform, bool parentTransformUpdated)
    {
        _groupCommand.init(getGlobalZOrder());
        renderer->addCommand(&_groupCommand);
        renderer->pushGroup(_groupCommand.getRenderQueueID());

        if(parentTransformUpdated || _transformUpdated)
        {
            _modelViewTransform = transform(parentTransform);
        }

        _transformUpdated = false;

        kmGLPushMatrix();
        kmGLLoadMatrix(&_modelViewTransform);

        _gridBeginCommand.init(getGlobalZOrder());
        _gridBeginCommand.func = CC_CALLBACK_0(ScreenMappedPolygon::onBeforeDraw, this);
        renderer->addCommand(&_gridBeginCommand);

        Node::visit(renderer, parentTransform, parentTransformUpdated);

        _gridEndCommand.init(getGlobalZOrder());
        _gridEndCommand.func = CC_CALLBACK_0(ScreenMappedPolygon::onAfterDraw, this);
        renderer->addCommand(&_gridEndCommand);

        renderer->popGroup();

        kmGLPopMatrix();
    }
}

