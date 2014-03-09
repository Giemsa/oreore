#ifndef __OREORE_COCOS2D_SCENEMANAGER_H__
#define __OREORE_COCOS2D_SCENEMANAGER_H__

#include "cocos2d.h"
#include <vector>
#include "../../null.h"
#include "Debug.h"

namespace oreore
{
    class SceneManager;

    class TransitionArguments
    {
    private:
        void *data;
    public:
        template<typename T>
        inline T &get() { return *static_cast<T *>(data); }
    };

    /* Do not inherit this class directly */
    class ManagedSceneBase : public cocos2d::CCLayer
    {
    private:
        float transitionTime;
    public:
        ManagedSceneBase();
        virtual ~ManagedSceneBase();
        virtual cocos2d::CCScene *getScene() const = 0;
        virtual unsigned int getID() const = 0;

        virtual bool init(); // override

        virtual cocos2d::CCTransitionScene *transition(cocos2d::CCScene *nextScene);

        inline float getTransitionTime() const { return transitionTime; }
        inline void setTransitionTime(const float t) { transitionTime = t; }

        virtual void onEnter(const TransitionArguments &args) { }
        virtual void onExit(const TransitionArguments &args) { }
    };

    typedef std::vector<ManagedSceneBase *> SceneList;

    template<typename T>
    class SceneBase : public ManagedSceneBase
    {
        friend class SceneManager;
    private:
        static unsigned int _uid;
        cocos2d::CCScene *scene;
    public:
        inline unsigned int getID() const { return _uid; }
        inline cocos2d::CCScene *getScene() const { return scene; }

        SceneBase()
        {
            scene = cocos2d::CCScene::create();
            scene->retain();
            scene->addChild(this);
        }

        virtual ~SceneBase()
        {
            scene->release();
        }
    };

    template<typename T>
    unsigned int SceneBase<T>::_uid = 0;


    /* Scene group */
    class SceneGroup : public ManagedSceneBase
    {
    private:
        SceneList scenes;
    public:
        SceneGroup() { }
        virtual ~SceneGroup() { }
    };

    /* loading... */
    class LoadingScene : public cocos2d::CCLayerColor
    {
        friend class SceneManager;
    private:
        float maxDuration, duration;
        cocos2d::CCScene *scene, *nextScene;

        SceneManager *manager;
        ManagedSceneBase *currentScene;
        bool disposeNext;
    public:
        static LoadingScene *create(const cocos2d::ccColor4B &color);
        static LoadingScene *create(const cocos2d::ccColor4B &color, const float maxDuration);

        LoadingScene();
        virtual ~LoadingScene();
        virtual void update(float dt);

        bool init(const cocos2d::ccColor4B &color, const float maxDuration);
        inline bool init(const cocos2d::ccColor4B &color) { return init(color, 0.0f); }
        inline cocos2d::CCScene *getScene() const { return scene; }

        void onEnter(); // override
    };

    class SceneManager
    {
        friend class LoadingScene;
    private:
        SceneList scenes;
        LoadingScene *loadingScene;
        DebugLayer *debugLayer;
        bool showDebugLayer;

        cocos2d::CCObject *getCurrentScene();
    public:
        inline static SceneManager &getInstance()
        {
            static SceneManager sm;
            return sm;
        }

        SceneManager();
        ~SceneManager();

        void init();

        template<typename T>
        void addScene(const bool lazy = true);
        LoadingScene *setLoadingScene(LoadingScene *scene);

        template<typename T>
        void switchScene(const bool disposeScene = false);

        template<typename T>
        void forwardScene();

        void backScene(const bool disposeScene = false);

        template<typename T>
        T *getScene();

        void setDebugMode(const bool debugMode);
        inline bool isDebugMode() const { return showDebugLayer; }
    };

    /* SceneManager impl */
    template<typename T>
    void SceneManager::addScene(const bool lazy)
    {
        if(T::_uid > 0)
            return;
        T::_uid = static_cast<unsigned int>(scenes.size());
        if(lazy)
            scenes.push_back(null);
        else
            scenes.push_back(T::create());
    }

    template<typename T>
    T *SceneManager::getScene()
    {
        ManagedSceneBase *scene = scenes[T::_uid];
        if(scene)
            return static_cast<T *>(scene);

        T *r = T::create();
        scenes[T::_uid] = r;
        return r;
    }

    template<typename T>
    void SceneManager::switchScene(const bool disposeScene)
    {
        ManagedSceneBase *current = dynamic_cast<ManagedSceneBase *>(getCurrentScene());

        if(disposeScene)
        {
            current->getScene()->autorelease();
            scenes[current->getID()] = null;
        }

        if(current)
            cocos2d::CCDirector::sharedDirector()->replaceScene(current->transition(getScene<T>()->getScene()));
        else
            cocos2d::CCDirector::sharedDirector()->replaceScene(getScene<T>()->getScene());
    }

    template<typename T>
    void SceneManager::forwardScene()
    {
        ManagedSceneBase *current = dynamic_cast<ManagedSceneBase *>(getCurrentScene());
        cocos2d::CCScene *next = null;

        if(loadingScene)
        {
            loadingScene->nextScene = getScene<T>()->getScene();
            loadingScene->disposeNext = false;
            if(current)
            {
                loadingScene->currentScene = current;
                next = current->transition(loadingScene->getScene());
            }
            else
            {
                loadingScene->currentScene = null;
                next = loadingScene->getScene();
            }
        }
        else
        {
            if(current)
                next = current->transition(getScene<T>()->getScene());
            else
                next = getScene<T>()->getScene();
        }
        cocos2d::CCDirector::sharedDirector()->pushScene(next);
    }
}

#endif
