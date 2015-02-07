#ifndef __OREORE__COCOS2DX_TUTORIALCLIPPINGSPRITE_H__
#define __OREORE__COCOS2DX_TUTORIALCLIPPINGSPRITE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

namespace oreore
{
    namespace Tutorial
    {
        /**
         * @class ClippingSprite
         * @brief チュートリアルにおいて切り抜き領域を表すクラスです。
         * 実装にはScale9Spriteを用いているため、任意のサイズへの拡大縮小が可能です。
         * このクラスに指定するテクスチャは、アルファ値を持った白単色である必要があります。
         */
        class ClippingSprite : public cocos2d::extension::Scale9Sprite
        {
        private:
            bool touchEnabled;

            ClippingSprite()
            : touchEnabled(true) { }
        protected:
            virtual bool initWithFile(const std::string &fileName, const float width, const float height);
            virtual bool initWithSpriteFrameName(const std::string &fileName, const float width, const float height);
        public:
            /**
             * @brief スプライトフレーム名からクリッピング領域を作成する
             * 指定する画像はアルファ値を持った白単色画像である必要がある
             * @param fileName ファイル名
             * @param width 可変領域の幅(パーセント)
             * @param height 可変領域の高さ(パーセント)
             */
            static ClippingSprite* createWithSpriteFrameName(const std::string &fileName, const float width , const float height);

            /**
             * @brief スプライトフレーム名からクリッピング領域を作成する
             * 指定する画像はアルファ値を持った白単色画像である必要がある
             * @param fileName ファイル名
             * @param size 可変領域の幅、高さ(パーセント)
             */
            static ClippingSprite* createWithSpriteFrameName(const std::string &fileName, const float size)
            {
                return createWithSpriteFrameName(fileName, size, size);
            }

            /**
             * @brief スプライトフレーム名からクリッピング領域を作成する
             * 指定する画像はアルファ値を持った白単色画像である必要がある
             * @param fileName ファイル名
             */
            static ClippingSprite* createWithSpriteFrameName(const std::string &fileName)
            {
                return createWithSpriteFrameName(fileName, 0.0f, 0.0f);
            }

            /**
             * @brief ファイルからクリッピング領域を作成する
             * 指定する画像はアルファ値を持った白単色画像である必要がある
             * @param fileName ファイル名
             * @param width 可変領域の幅(パーセント)
             * @param height 可変領域の高さ(パーセント)
             */
            static ClippingSprite* create(const std::string &fileName, const float width, const float height);

            /**
             * @brief ファイルからクリッピング領域を作成する
             * 指定する画像はアルファ値を持った白単色画像である必要がある
             * @param fileName ファイル名
             * @param size 可変領域の幅、高さ(パーセント)
             */
            static ClippingSprite* create(const std::string &fileName, const float size)
            {
                return create(fileName, size, size);
            }

            /**
             * @brief ファイルからクリッピング領域を作成する
             * 指定する画像はアルファ値を持った白単色画像である必要がある
             * @param fileName ファイル名
             */
            static ClippingSprite* create(const std::string &fileName)
            {
                return create(fileName, 0.0f, 0.0f);
            }

            /**
             * @brief くり抜き部分のタッチ入力を裏側のノードへ
             * 伝搬させるかの設定
             */
            void setTouchEnabled(const bool enable) { touchEnabled = enable; }
            bool isTouchEnabled() const { return touchEnabled; }
        };
    };
}


#endif
