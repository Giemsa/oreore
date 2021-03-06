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
        return  base + fRange;
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

    static Vertex3F v3fCreateNormalizedVertex(const float adelta)
    {
        const float sth = sinf(adelta);
        const float x = randf(0.0f, 360.0f);
        const float y = randf(0.0f, 360.0f);
        const float z = randf(0.0f, 360.0f);
        if(x == 0.0f && y == 0.0f && z == 0.0f)
        {
            return Vertex3F();
        }

        const float len = sqrtf(x * x + y * y + z * z);
        return Vertex3F(x * sth / len, y * sth / len, z * sth / len);
    }

    static inline Vertex3F getDeltaVec(const Tex2F &a, const Tex2F &b, const Tex2F &c, const Vertex3F &center, const float speedVar)
    {
        Vertex3F v((a.u + b.u + c.u) / 3 - center.x, (a.v + b.v + c.v) / 3 - center.y, -center.z);
        v3fNormalize(v);
        v.x *= randf(0.0, speedVar) + 1.0f;
        v.y *= randf(0.0, speedVar) + 1.0f;
        v.z *= randf(0.0, speedVar) + 1.0f;
        return v;
    }

    /* ShatteredSprite */
    ShatteredSprite *ShatteredSprite::create(const ShatteredSpriteParam &param)
    {
        ShatteredSprite *r = new ShatteredSprite();
        if(r && r->init(param))
        {
            r->autorelease();
            return r;
        }
        delete r;
        return nullptr;
    }

    bool ShatteredSprite::init()
    {
        if(!Sprite::init())
        {
            return false;
        }

        return true;
    }

    bool ShatteredSprite::init(const ShatteredSpriteParam &param)
    {
        if(!Sprite::initWithTexture(param.texture, param.textureRect))
        {
            return false;
        }

        initShatterData(param);
        return true;
    }

    void ShatteredSprite::setParams(const ShatteredSpriteParam &param)
    {
        setTexture(param.texture);
        setTextureRect(param.textureRect);
        initShatterData(param);
    }

    void ShatteredSprite::reset(SpriteFrame *frame)
    {
        setSpriteFrame(frame);

        // UVを再構築
        const float height = getTextureRect().size.height;

        const float rx = CC_CONTENT_SCALE_FACTOR() / getTexture()->getPixelsWide();
        const float ry = CC_CONTENT_SCALE_FACTOR() / getTexture()->getPixelsHigh();
        const float ox = getTextureRect().origin.x * CC_CONTENT_SCALE_FACTOR() / getTexture()->getPixelsWide();
        const float oy = getTextureRect().origin.y * CC_CONTENT_SCALE_FACTOR() / getTexture()->getPixelsHigh();

        texCoord.clear();
        for(Vertex3F &vtx : coord)
        {
            texCoord.push_back(Tex2F(vtx.x * rx + ox, (height - vtx.y) * ry + oy));
        }
    }

    void ShatteredSprite::initShatterData(const ShatteredSpriteParam &param)
    {
        this->piecesX = param.piecesX + 1;
        shattered = false;

        if(!param.shader)
        {
            shader = ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR);
        }
        else
        {
            shader = param.shader;
        }

        updateBlendFunc();

        const float height = param.textureRect.size.height;
        const float pieceXsize = param.textureRect.size.width / param.piecesX;
        const float pieceYsize = height / param.piecesY;
        const Vertex3F tc(getContentSize().width / 2, getContentSize().height / 2, 0.0f);

        const float rx = CC_CONTENT_SCALE_FACTOR() / param.texture->getPixelsWide();
        const float ry = CC_CONTENT_SCALE_FACTOR() / param.texture->getPixelsHigh();
        const float ox = param.textureRect.origin.x * CC_CONTENT_SCALE_FACTOR() / param.texture->getPixelsWide();
        const float oy = param.textureRect.origin.y * CC_CONTENT_SCALE_FACTOR() / param.texture->getPixelsHigh();

        const Color4B color(getColor().r, getColor().g, getColor().b, getOpacity());
        Tex2FVec ptArray((param.piecesX + 1) * (param.piecesY + 1));

        for(int x = 0; x <= param.piecesX; x++)
        {
            for(int y = 0; y <= param.piecesY; y++)
            {
                Tex2F pt(x * pieceXsize, y * pieceYsize);
                if(x > 0 && x < param.piecesX && y > 0 && y < param.piecesY)
                    t2fAdd(pt, Tex2F(roundf(randf(0.0f, pieceXsize * 0.45f)), roundf(randf(0.0f, pieceYsize * 0.45f))));
                ptArray[getIndex(x, y)] = pt;
            }
        }

        coord.clear();
        texCoord.clear();
        moveInfoVec.clear();
        colors.resize(param.piecesX * param.piecesY * 6, color);

        for(int x = 0; x < param.piecesX; x++)
        {
            for(int y = 0; y < param.piecesY; y++)
            {
                {
                    const float adelta = randf(0.0f, param.rotVar);

                    const Tex2F &p1 = ptArray[getIndex(x, y)];
                    const Tex2F &p2 = ptArray[getIndex(x + 1, y)];
                    const Tex2F &p3 = ptArray[getIndex(x, y + 1)];

                    moveInfoVec.push_back(
                        MoveInfo(
                            v3fCreateNormalizedVertex(adelta),
                            getDeltaVec(p1, p2, p3, tc, param.speedVar),
                            Vertex3F((x * pieceXsize) + (pieceXsize * 0.3), (y * pieceYsize) + (pieceYsize * 0.3), 0),
                            cosf(adelta)
                        )
                    );


                    coord.push_back(t2f2v3f(p1, 0.0f));
                    coord.push_back(t2f2v3f(p2, 0.0f));
                    coord.push_back(t2f2v3f(p3, 0.0f));

                    texCoord.push_back(Tex2F(p1.u * rx + ox, (height - p1.v) * ry + oy));
                    texCoord.push_back(Tex2F(p2.u * rx + ox, (height - p2.v) * ry + oy));
                    texCoord.push_back(Tex2F(p3.u * rx + ox, (height - p3.v) * ry + oy));
                }

                {
                    const float adelta = randf(0.0f, param.rotVar);

                    const Tex2F &p1 = ptArray[getIndex(x + 1, y)];
                    const Tex2F &p2 = ptArray[getIndex(x + 1, y + 1)];
                    const Tex2F &p3 = ptArray[getIndex(x, y + 1)];

                    moveInfoVec.push_back(
                        MoveInfo(
                            v3fCreateNormalizedVertex(adelta),
                            getDeltaVec(p1, p2, p3, tc, param.speedVar),
                            Vertex3F((x * pieceXsize) + (pieceXsize * 0.7), (y * pieceYsize) + (pieceYsize * 0.7), 0),
                            cosf(adelta)
                        )
                    );

                    coord.push_back(t2f2v3f(p1, 0.0f));
                    coord.push_back(t2f2v3f(p2, 0.0f));
                    coord.push_back(t2f2v3f(p3, 0.0f));

                    texCoord.push_back(Tex2F(p1.u * rx + ox, (height - p1.v) * ry + oy));
                    texCoord.push_back(Tex2F(p2.u * rx + ox, (height - p2.v) * ry + oy));
                    texCoord.push_back(Tex2F(p3.u * rx + ox, (height - p3.v) * ry + oy));
                }
            }
        }
    }

    void ShatteredSprite::updateColor()
    {
        Sprite::updateColor();
        if(shattered)
        {
            if(getTexture()->hasPremultipliedAlpha())
            {
                const float opacity = getOpacity() / 255.0f;
                std::fill(
                    colors.begin(), colors.end(),
                    Color4B(getColor().r * opacity, getColor().g * opacity, getColor().b * opacity, getOpacity())
                );
            }
            else
            {
                std::fill(
                    colors.begin(), colors.end(),
                    Color4B(getColor().r, getColor().g, getColor().b, getOpacity())
                );
            }
        }
    }

    void ShatteredSprite::draw(cocos2d::Renderer *renderer, const kmMat4 &parentTransform, bool parentTransformUpdated)
    {
        if(!shattered)
        {
            Sprite::draw(renderer, parentTransform, parentTransformUpdated);
            return;
        }

        Node::draw(renderer, parentTransform, parentTransformUpdated);
        CCASSERT(coord.size() == texCoord.size() && coord.size() == colors.size(), "coord.size() != texCoord.size() != colors.size()");

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

        cmd.init(getGlobalZOrder());
        cmd.func = [this]() {
            shader->use();
            shader->setUniformsForBuiltins(_modelViewTransform);

            GL::blendFunc(getBlendFunc().src, getBlendFunc().dst);
            GL::bindTexture2D(getTexture()->getName());

            GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
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

