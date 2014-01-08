#ifndef __OREORE_COMMON_SCREEN_BASE_H__
#define __OREORE_COMMON_SCREEN_BASE_H__

#include "cocos2d.h"
#include <cstdarg>
#include <vector>
#include "../../null.h"

/*
 * ゲーム作るときに共通しそうなやつ
*/

namespace oreore
{
    /* シーン/スクリーンのIDをユニークにふるやつ */
    class IDConnector
    {
    private:
        static unsigned int id;
    public:
        inline static unsigned int get() { return id++; }
    };

    /* スクリーンベース(シーンはこれを継承すること)
     * 初期化はbool init()を継承してその中で行う。
     * リソース初期化の都合上、コンストラクタの中で初期化してもうまく動かない場合がある。
     */
    class ScreenBase : public cocos2d::Layer
    {
    protected:
        cocos2d::Scene *_scene;

    public:
        ScreenBase();
        virtual ~ScreenBase();

        inline cocos2d::Scene *scene() const { return _scene; }
        cocos2d::SpriteBatchNode *registerSprite(const char *filename);
    };


    struct SpriteInfoBase
    {
        virtual ~SpriteInfoBase() { }
        virtual cocos2d::Sprite *getSprite() const = 0;
        virtual cocos2d::Texture2D *getTexture() const = 0;
        virtual cocos2d::Rect getRect() const = 0;
    };


    struct SpriteBatchInfo : public SpriteInfoBase
    {
        cocos2d::SpriteBatchNode *batch;
        const cocos2d::Rect rect;

        SpriteBatchInfo(cocos2d::SpriteBatchNode *batch, const float x, const float y, const float w, const float h)
            : batch(batch), rect(x, y, w, h) { }
        virtual ~SpriteBatchInfo() { }
        cocos2d::Sprite *getSprite() const { return cocos2d::Sprite::createWithTexture(batch->getTexture(), rect); }
        cocos2d::Texture2D *getTexture() const { return batch->getTexture(); }
        cocos2d::Rect getRect() const { return rect; }
    };

    struct SpriteBatchInfoWhole : public SpriteInfoBase
    {
        cocos2d::SpriteBatchNode *batch;

        SpriteBatchInfoWhole(cocos2d::SpriteBatchNode *batch)
            : batch(batch) { }
        virtual ~SpriteBatchInfoWhole() { }
        cocos2d::Sprite *getSprite() const { return cocos2d::Sprite::createWithTexture(batch->getTexture()); }
        cocos2d::Texture2D *getTexture() const { return batch->getTexture(); }
        cocos2d::Rect getRect() const
        {
            cocos2d::Rect rect;
            rect.origin = cocos2d::Point(0, 0);
            rect.size = batch->getContentSize();
            return rect;
        }
    };


    typedef std::vector<SpriteInfoBase *> SpriteInfoList;

    /* 画像の登録に使う */
    template<typename E>
    class SpriteRegistrar
    {
        template<typename T, int N, typename _E, _E C>
        friend class GameManager;
    private:
        SpriteInfoList *list;
        cocos2d::SpriteBatchNode *batch;
        const char *filename;
        SpriteRegistrar(SpriteInfoList &list, cocos2d::SpriteBatchNode *batch) : list(&list), batch(batch) { }

    public:
        SpriteRegistrar &add(const E id, const float x, const float y, const float w, const float h)
        {
            (*list)[static_cast<int>(id)] = new SpriteBatchInfo(batch, x, y, w, h);
            return *this;
        }

        SpriteRegistrar &add(const E id)
        {
            (*list)[static_cast<int>(id)] = new SpriteBatchInfoWhole(batch);
            return *this;
        }
    };

    /*
     * ゲームマネージャー(スプライトとか管理する)
     * このクラスを継承して、実際に使うゲームマネージャを作る。
     * T: 継承後のゲームマネージャークラス
     * N: 登録するスクリーン数
     * E: スプライト管理に用いるIDの列挙型とかint型default = int)
     * C: 読み込む画像の最大枚数(default = 4/immutableに管理するのであとから増やすとかはできない)
     */
    template<typename T, int N, typename E = int, E C = 4>
    class GameManager
    {
        typedef std::vector<ScreenBase *> ScreenList;
    private:
        static cocos2d::Size size;
        ScreenList screens;
        SpriteInfoList sprite_list;
        cocos2d::SpriteBatchNode **batchNodes;

    protected:
        template<typename S>
        inline void registerScreen(S *screen) { screens.at(S::getID()) = screen; }

        /* リソースの読み込みを行う。
         * 登録先スクリーンは予めregisterScreenで読み込んでおく必要がある。
         */
        template<typename S>
        SpriteRegistrar<E> loadSprite(const char *filename)
        {
            ScreenBase *screen = screens[S::getID()];
            cocos2d::SpriteBatchNode *batch = screen->registerSprite(filename);
            return SpriteRegistrar<E>(sprite_list, batch);
        }

        /* リソースの読み込みを行う。
         * 登録先は任意のレイヤ
         */
        SpriteRegistrar<E> loadSprite(cocos2d::Layer *layer, const char *filename)
        {
            cocos2d::SpriteBatchNode *batch = cocos2d::SpriteBatchNode::create(filename);
            layer->addChild(batch);
            return SpriteRegistrar<E>(sprite_list, batch);
        }

        /* すべての登録済みスクリーンに対しinitメソッドを呼び出す。 */
        void initScreens()
        {
            for(ScreenList::iterator it = screens.begin(); it != screens.end(); ++it)
                if(*it)
                    (*it)->init();
        }
    public:
        typedef E ElementType;

        GameManager()
            : screens(N), sprite_list(static_cast<int>(C))
        {
            size = cocos2d::Director::getInstance()->getWinSize();
        }

        virtual ~GameManager()
        {
            for(typename SpriteInfoList::iterator it = sprite_list.begin(); it != sprite_list.end(); ++it)
                delete *it;
        }

        virtual void init() = 0;

        static inline T *get()
        {
            static T ins;
            return &ins;
        }

        template<typename S>
        static inline S *screen() { return static_cast<S *>(get()->screens[S::getID()]); }

        inline static cocos2d::Size &getScreenSize() { return size; }
        inline static float getWidth() { return size.width; }
        inline static float getHeight() { return size.height; }
        inline static cocos2d::Sprite *sprite(const E id) { return get()->sprite_list[static_cast<int>(id)]->getSprite(); }
        inline static cocos2d::Texture2D *texture(const E id) { return get()->sprite_list[static_cast<int>(id)]->getTexture(); }
        inline static cocos2d::Rect rect(const E id) { return get()->sprite_list[static_cast<int>(id)]->getRect(); }
    };


    template<typename T, int N, typename E, E C>
    cocos2d::Size GameManager<T, N, E, C>::size;

    /*
     * Menuのボタンを作るクラス
     * T:                GameManagerクラス
     * buttonBG:         ボタンの背景画像のインデックス
     * selectedButtonBG: 選択時のボタンの背景画像のインデックス
     */
    template<typename T, typename T::ElementType buttonBG, typename T::ElementType selectedButtonBG>
    class MenuItemGenerator
    {
            typedef typename T::ElementType element_type;
    private:
        cocos2d::Sprite *bg, *selbg;

        cocos2d::Sprite *getSprite(cocos2d::Sprite *bg, cocos2d::Sprite *text, const cocos2d::Size &size)
        {
            cocos2d::RenderTexture *tex = cocos2d::RenderTexture::create(size.width, size.height);
            tex->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
            bg->visit();
            text->visit();
            tex->end();
            return cocos2d::Sprite::createWithTexture(tex->getSprite()->getTexture());
        }

        cocos2d::Sprite *getSprite(const element_type index) const
        {
            cocos2d::Sprite *sprite = T::sprite(index);
            sprite->setAnchorPoint(cocos2d::Point::ZERO);
            sprite->setPosition(cocos2d::Point::ZERO);
            return sprite;
        }
    public:
        MenuItemGenerator() : bg(null), selbg(null) { }
        virtual ~MenuItemGenerator() { }

        cocos2d::MenuItemSprite *generate(const element_type index, const cocos2d::ccMenuCallback &callback)
        {
            if(!bg)
                bg = getSprite(buttonBG);
            if(!selbg)
                selbg = getSprite(selectedButtonBG);

            const cocos2d::Size &size = bg->getContentSize();
            cocos2d::Sprite *text = T::sprite(index);
            text->setPosition(cocos2d::Point(size.width / 2, size.height / 2));
            text->setFlippedY(true);

            return cocos2d::MenuItemSprite::create(
                getSprite(bg, text, size),
                getSprite(selbg, text,size),
                callback
            );
        }

        template<typename S>
        inline S *generate(const cocos2d::ccMenuCallback &callback) const
        {
            return S::create(
                getSprite(buttonBG),
                getSprite(selectedButtonBG),
                callback
            );
        }
    };
}

#define CONNECT_ID  \
    private:        \
        static unsigned int __uid;  \
    public:         \
        inline static unsigned int getID() { return __uid; }

#define INIT_BASE_SCREEN(T) unsigned int T::__uid = oreore::IDConnector::get();

#endif
