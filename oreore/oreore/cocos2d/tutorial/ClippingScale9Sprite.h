#ifndef __OREORE_TUTORIAL_CLIPPINGSCALE9SPRITE_H__
#define __OREORE_TUTORIAL_CLIPPINGSCALE9SPRITE_H__

#include "cocos2d.h"
#include "ClippingNode.h"
#include "extensions/cocos-ext.h"

namespace oreore
{
    namespace Tutorial
    {
        /**
         * @class ClippingScale9Sprite
         * @brief チュートリアルにおいて切り抜き領域を表すクラスです。
         * 実装にはScale9Spriteを用いているため、任意のサイズへの拡大縮小が可能です。
         * このクラスに指定するテクスチャは、アルファ値を持った白単色である必要があります。
         */
        class ClippingScale9Sprite
            : public cocos2d::extension::Scale9Sprite
            , public ClippingNode
        {
        protected:
            virtual bool initWithFile(const std::string &fileName, const float width, const float height);
            virtual bool initWithSpriteFrameName(const std::string &name, const float width, const float height);
        public:
            /**
             * @brief スプライトフレーム名からクリッピング領域を作成する
             * 指定する画像はアルファ値を持った白単色画像である必要がある
             * @param fileName ファイル名
             * @param width 固定領域の幅(パーセント)
             * @param height 固定領域の高さ(パーセント)
             */
            static ClippingScale9Sprite* createWithSpriteFrameName(const std::string &name, const float width , const float height);

            /**
             * @brief スプライトフレーム名からクリッピング領域を作成する
             * 指定する画像はアルファ値を持った白単色画像である必要がある
             * @param fileName ファイル名
             * @param size 固定領域の幅、高さ(パーセント)
             */
            static ClippingScale9Sprite* createWithSpriteFrameName(const std::string &name, const float size)
            {
                return createWithSpriteFrameName(name, size, size);
            }

            /**
             * @brief ファイルからクリッピング領域を作成する
             * 指定する画像はアルファ値を持った白単色画像である必要がある
             * @param fileName ファイル名
             * @param width 固定領域の幅(パーセント)
             * @param height 固定領域の高さ(パーセント)
             */
            static ClippingScale9Sprite* create(const std::string &fileName, const float width, const float height);

            /**
             * @brief ファイルからクリッピング領域を作成する
             * 指定する画像はアルファ値を持った白単色画像である必要がある
             * @param fileName ファイル名
             * @param size 固定領域の幅、高さ(パーセント)
             */
            static ClippingScale9Sprite* create(const std::string &fileName, const float size)
            {
                return create(fileName, size, size);
            }
        };
    };
}

#endif
