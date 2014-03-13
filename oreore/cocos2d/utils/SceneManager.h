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
    class ManagedSceneBase : public cocos2d::Layer
    {
    private:
        float transitionTime;
    public:
        ManagedSceneBase();
        virtual ~ManagedSceneBase();
        virtual cocos2d::Scene *getScene() const = 0;
        virtual unsigned int getID() const = 0;

        virtual bool init() override;

        virtual cocos2d::TransitionScene *transition(cocos2d::Scene *nextScene);
        virtual bool isLazy() const = 0;

        inline float getTransitionTime() const { return transitionTime; }
        inline void setTransitionTime(const float t) { transitionTime = t; }

        virtual void onEnter(const TransitionArguments &args) { }
        virtual void onExit(const TransitionArguments &args) { }
        
        virtual void onEnter() { cocos2d::CCLayer::onEnter(); } // override
        virtual void onExit() { cocos2d::CCLayer::onExit(); } // override
    };

    typedef std::vector<ManagedSceneBase *> SceneList;

    template<typename T>
    class SceneBase : public ManagedSceneBase
    {
        friend class SceneManager;
    private:
        static unsigned int _uid;
        static bool _isLazy;
        cocos2d::Scene *scene;
    public:
        inline unsigned int getID() const { return _uid; }
        inline cocos2d::Scene *getScene() const { return scene; }
        bool isLazy() const { return _isLazy; } // override

        SceneBase()
        {
            scene = cocos2d::Scene::create();
            scene->retain();
            scene->addChild(this);
        }

        virtual ~SceneBase() { }
    };

    template<typename T>
    unsigned int SceneBase<T>::_uid = 0;

    template<typename T>
    bool SceneBase<T>::_isLazy = false;


    /* Scene group */
    class SceneGroup : public ManagedSceneBase
    {
    private:
        SceneList scenes;
    public:
        SceneGroup() { }
        virtual ~SceneGroup() { }
    };

    class SceneManager
    {
        friend class LoadingScene;
    private:
        SceneList scenes;
        DebugLayer *debugLayer;
        bool showDebugLayer;

        cocos2d::Object *getCurrentScene();
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
        T::_isLazy = lazy;
        if(lazy)
            scenes.push_back(null);
        else
        {
            T *t = T::create();
            t->getScene()->retain();
            scenes.push_back(t);
        }
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
            if(!current->isLazy())
                current->getScene()->release();
            scenes[current->getID()] = null;
        }

        if(current)
            cocos2d::Director::getInstance()->replaceScene(current->transition(getScene<T>()->getScene()));
        else
            cocos2d::Director::getInstance()->replaceScene(getScene<T>()->getScene());
    }

    template<typename T>
    void SceneManager::forwardScene()
    {
        ManagedSceneBase *current = dynamic_cast<ManagedSceneBase *>(getCurrentScene());
        cocos2d::Scene *next = null;

        if(current)
            next = current->transition(getScene<T>()->getScene());
        else
            next = getScene<T>()->getScene();

        cocos2d::Director::getInstance()->pushScene(next);
    }
}

#endif
