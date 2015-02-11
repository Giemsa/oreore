#ifndef __OREORE_TUTORIAL_CLIPPINGNODE_H__
#define __OREORE_TUTORIAL_CLIPPINGNODE_H__

#include "../../utils/variant.hpp"
#include "cocos2d.h"

namespace oreore
{
    namespace Tutorial
    {
        // forward
        namespace detail
        {
            class TutorialBaseBase; // in TutorialBase.h
        }

        class ClippingNode
        {
            friend class detail::TutorialBaseBase;
            using CallbackFunc = std::function<bool(const cocos2d::Point &)>;
        private:
            bool touchEnabled;
            variant<cocos2d::Rect, CallbackFunc> touchInfo;

        protected:
            virtual bool checkTouch(const cocos2d::Point &pos) const;
            bool hasTouchHandle() const { return !touchInfo.empty(); }
        public:
            ClippingNode()
            : touchEnabled(true)
            { }

            virtual ~ClippingNode() { }

            /**
             * @brief くり抜き部分のタッチ入力を裏側のノードへ
             * 伝搬させるかの設定
             */
            void setTouchEnabled(const bool enable) { touchEnabled = enable; }
            bool isTouchEnabled() const { return touchEnabled; }

            /**
             * @brief タッチされた際の当たり判定矩形の登録
             * setTouchCallabackとの併用はできない
             */
            void setTouchRect(const cocos2d::Rect &rect) { touchInfo = rect; }

            const cocos2d::Rect *getTouchRect() const;

            /**
             * @brief タッチされた際のコールバック関数の登録
             * setTouchRectとの併用はできない
             * @param callback コールバック関数。引数はcocos2d::Point、戻り値はboolで、
             * 戻り値がtrueの時あたりとみなす
             */
            void setTouchCallback(const CallbackFunc &callback)
            {
                touchInfo = callback;
            }

            const CallbackFunc *getTouchCallback() const;

            /**
             * @brief setTouchRectやsetTouchCallbackで設定されたタッチ判定処理を
             * 無効化します
             */
            void resetTouchHandle()
            {
                touchInfo.reset();
            }
        };
    }
}

#endif
