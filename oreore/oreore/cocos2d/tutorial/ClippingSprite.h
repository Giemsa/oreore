#ifndef __OREORE_TUTORIAL_CLIPPINGSPRITE_H__
#define __OREORE_TUTORIAL_CLIPPINGSPRITE_H__

#include "cocos2d.h"
#include "ClippingNode.h"

namespace oreore
{
    namespace Tutorial
    {
        /**
         * @class ClippingSprite
         * @brief チュートリアルにおいて切り抜き領域を表すクラスです。
         * このクラスに指定するテクスチャは、アルファ値を持った白単色である必要があります。
         */
        class ClippingSprite
            : public cocos2d::Sprite
            , public ClippingNode
        {
        protected:
            virtual bool initWithFile(const std::string &fileName);
            virtual bool initWithSpriteFrameName(const std::string &name);
        public:
            /**
             * @brief スプライトフレーム名からクリッピング領域を作成する
             * 指定する画像はアルファ値を持った白単色画像である必要がある
             * @param fileName ファイル名
             */
            static ClippingSprite* createWithSpriteFrameName(const std::string &name);

            /**
             * @brief ファイルからクリッピング領域を作成する
             * 指定する画像はアルファ値を持った白単色画像である必要がある
             * @param fileName ファイル名
             */
            static ClippingSprite* create(const std::string &fileName);
        };
    };
}


#endif
