#include "ShatteredSprite.h"
#include "../utils/Utils.h"
#include "../../Utils.h"


namespace oreore
{
    using namespace cocos2d;

    /* utils */
    static float randf(const float base, const float range)
    {
        const long lRange = random(static_cast<unsigned long int>(range * 2 * 10000));
        const float fRange = static_cast<float>(lRange) / 10000.0 - range;
        return	base + fRange;
    }

    static inline void t2fAdd(Tex2F &v0, const Tex2F &v1)
    {
        v0.u += v1.u;
        v0.v += v1.v;
    }

    static inline void v3fAdd(Vertex3F &v0, const Vertex3F &v1)
    {
        v0.x += v1.x;
        v0.y += v1.y;
        v0.z += v1.z;
    }

    static inline void v2fRotateByAngle(Vertex2F &v0, const Vertex2F &c, const float angle)
    {
        const GLfloat x = v0.x - c.x;
        const GLfloat y = v0.y - c.y;
        const float ca = cosf(angle);
        const float sa = sinf(angle);
        v0.x = c.x + (x * ca - y * sa);
        v0.y = c.y + (x * sa + y * ca);
    }

    static inline Vertex3F t2f2v3f(const Tex2F &t, const GLfloat z)
    {
        return Vertex3F(t.u, t.v, z);
    }

    static inline void v3fNormalize(Vertex3F &v)
    {
        const float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
        if(len == 0.0f)
        {
            v = Vertex3F();
            return;
        }
    
        v.x /= len;
        v.y /= len;
        v.z /= len;
    }

    static Vertex3F v3fCreateNormalizedVerex(const float adelta)
    {
        const float sth = sinf(adelta);
        const float x = randf(0.0f, 360.0f);
        const float y = randf(0.0f, 360.0f);
        const float z = randf(0.0f, 360.0f);
        if(x == 0.0f && y == 0.0f && z == 0.0f)
            return Vertex3F();
        const float len = sqrtf(x * x + y * y + z * z);
        return Vertex3F(x * sth / len, y * sth / len, z * sth / len);
    }

    static inline Vertex3F getDeltaVec(const Tex2F &a, const Tex2F &b, const Tex2F &c, const Vertex3F &center, const float speedVar)
    {
        Vertex3F v((a.u + b.u + c.u) / 3 - center.x, (a.v * b.v + c.v) / 3 - center.y, -center.z);
        v3fNormalize(v);
        v.x *= randf(0.0, speedVar) + 1.0f;
        v.y *= randf(0.0, speedVar) + 1.0f;
        v.z *= randf(0.0, speedVar) + 1.0f;
        return v;
    }

    /* ShatteredSprite */
    ShatteredSprite *ShatteredSprite::create(Sprite *sprite, const int piecesX, const int piecesY, const float speedVar, const float rotVar)
    {
        ShatteredSprite *r = new ShatteredSprite();
        if(r && r->init(sprite, piecesX, piecesY, speedVar, rotVar))
        {
            r->autorelease();
            return r;
        }
        delete r;
        return nullptr;
    }

    bool ShatteredSprite::init(Sprite *sprite, const int piecesX, const int piecesY, const float speedVar, const float rotVar)
    {
        if(!Node::init())
            return false;

        texture = nullptr;
        this->piecesX = piecesX + 1;
        shattered = false;

        const float width = sprite->getTexture()->getContentSizeInPixels().width;
        const float height = sprite->getTexture()->getContentSizeInPixels().height;

        setContentSize(sprite->getContentSize());
        setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
        setAnchorPoint(Point::ANCHOR_MIDDLE);

        RenderTexture *rt = RenderTexture::create(width, height);
        rt->retain();
        rt->setKeepMatrix(true);
        rt->begin();
        
        sprite->setPosition(center(sprite));
        sprite->visit();

        rt->end();
        
        grabCmd.init(getGlobalZOrder());
        grabCmd.func = [=]() {
            texture = rt->getSprite()->getTexture();
            const Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
            texture->setTexParameters(texParams);
            texture->retain();
            rt->release();
            
            updateBlendFunc();

            const float pieceXsize = width / piecesX;
            const float pieceYsize = height / piecesY;
            const Vertex3F tc(getContentSize().width / 2, getContentSize().height / 2, 0.0f);

            const float rx = 1.0f / texture->getPixelsWide();
            const float ry = 1.0f / texture->getPixelsHigh();

            const Color4B color(getColor().r, getColor().g, getColor().b, getOpacity());
            Tex2FVec ptArray((piecesX + 1) * (piecesY + 1));

            for(int x = 0; x <= piecesX; x++)
                for(int y = 0; y <= piecesY; y++)
                {
                    Tex2F pt(x * pieceXsize, y * pieceYsize);
                    if(x > 0 && x < piecesX && y > 0 && y < piecesY)
                        t2fAdd(pt, Tex2F(roundf(randf(0.0f, pieceXsize * 0.45f)), roundf(randf(0.0f, pieceYsize * 0.45f))));
                    ptArray[getIndex(x, y)] = pt;
                }

            colors.resize(piecesX * piecesY * 6, color);

            for(int x = 0; x < piecesX; x++)
            {
                for(int y = 0; y < piecesY; y++)
                {
                    {
                        const float adelta = randf(0.0f, rotVar);
                        
                        const Tex2F &p1 = ptArray[getIndex(x, y)];
                        const Tex2F &p2 = ptArray[getIndex(x + 1, y)];
                        const Tex2F &p3 = ptArray[getIndex(x, y + 1)];

                        moveInfoVec.push_back(
                            MoveInfo(
                                v3fCreateNormalizedVerex(adelta),
                                getDeltaVec(p1, p2, p3, tc, speedVar),
                                Vertex3F((x * pieceXsize) + (pieceXsize * 0.3), (y * pieceYsize) + (pieceYsize * 0.3), 0),
                                cosf(adelta)
                            )
                        );


                        coord.push_back(t2f2v3f(p1, 0.0f));
                        coord.push_back(t2f2v3f(p2, 0.0f));
                        coord.push_back(t2f2v3f(p3, 0.0f));

                        texCoord.push_back(Tex2F(p1.u * rx, p1.v * ry));
                        texCoord.push_back(Tex2F(p2.u * rx, p2.v * ry));
                        texCoord.push_back(Tex2F(p3.u * rx, p3.v * ry));
                    }

                    {
                        const float adelta = randf(0.0f, rotVar);
                        
                        const Tex2F &p1 = ptArray[getIndex(x + 1, y)];
                        const Tex2F &p2 = ptArray[getIndex(x + 1, y + 1)];
                        const Tex2F &p3 = ptArray[getIndex(x, y + 1)];
                        
                        moveInfoVec.push_back(
                            MoveInfo(
                                v3fCreateNormalizedVerex(adelta),
                                getDeltaVec(p1, p2, p3, tc, speedVar),
                                Vertex3F((x * pieceXsize) + (pieceXsize * 0.7), (y * pieceYsize) + (pieceYsize * 0.7), 0),
                                cosf(adelta)
                            )
                        );

                        coord.push_back(t2f2v3f(p1, 0.0f));
                        coord.push_back(t2f2v3f(p2, 0.0f));
                        coord.push_back(t2f2v3f(p3, 0.0f));

                        texCoord.push_back(Tex2F(p1.u * rx, p1.v * ry));
                        texCoord.push_back(Tex2F(p2.u * rx, p2.v * ry));
                        texCoord.push_back(Tex2F(p3.u * rx, p3.v * ry));
                    }
                }
            }
        };
        Director::getInstance()->getRenderer()->addCommand(&grabCmd);

        return true;
    }

	void ShatteredSprite::updateBlendFunc()
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
	
	void ShatteredSprite::cleanup()
	{
		CC_SAFE_RELEASE_NULL(texture);
		Node::cleanup();
	}

    void ShatteredSprite::updateColor()
    {
        Node::updateColor();
        const Color4B color(getColor().r, getColor().g, getColor().b, getOpacity());
        for(auto &c : colors)
            c = color;
    }

    void ShatteredSprite::draw(cocos2d::Renderer *renderer, const kmMat4 &parentTransform, bool parentTransformUpdated)
    {
        Node::draw(renderer, parentTransform, parentTransformUpdated);
        if(!texture)
            return;

        CCASSERT(coord.size() == texCoord.size() && coord.size() == colors.size(), "coord.size() != texCoord.size() != colors.size()");

        if(shattered)
        {
            kmQuaternion p, q, r;

            for(int i = 0; i < moveInfoVec.size(); i++)
            {
                Vertex3F *c = &coord[i * 3];
                MoveInfo &info = moveInfoVec[i];

                v3fAdd(info.center, info.vdelta);

                kmQuaternionFill(&p, -info.axis.x, -info.axis.y, -info.axis.z, info.adelta);
                kmQuaternionFill(&r, info.axis.x, info.axis.y, info.axis.z, info.adelta);

                for(int j = 0; j < 3; j++, c++)
                {
                    v3fAdd(*c, info.vdelta);

                    kmQuaternionFill(&q, c->x - info.center.x, c->y - info.center.y, c->z - info.center.z, 0);
                    kmQuaternionMultiply(&q, &p, &q);
                    kmQuaternionMultiply(&q, &q, &r);

                    c->x = q.x + info.center.x;
                    c->y = q.y + info.center.y;
                    c->z = q.z + info.center.z;
                }
            }
        }

        cmd.init(getGlobalZOrder());
        cmd.func = [this]() {
            CC_NODE_DRAW_SETUP();
			GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

			GL::blendFunc(blendFunc.src, blendFunc.dst);

			GL::bindTexture2D(texture->getName());

			glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, &coord[0]);
			glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, &texCoord[0]);
            glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, &colors[0]);
			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(coord.size()));
			CC_INCREMENT_GL_DRAWS(1);
        };
        renderer->addCommand(&cmd);
    }

    void ShatteredSprite::shatter()
    {
        shattered = true;
    }
}