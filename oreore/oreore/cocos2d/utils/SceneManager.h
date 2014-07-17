#ifndef __OREORE_COCOS2D_SCENEMANAGER_H__
#define __OREORE_COCOS2D_SCENEMANAGER_H__

#include "cocos2d.h"
#include <vector>
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
        virtual int getID() const = 0;

        virtual bool init() override;

        virtual cocos2d::TransitionScene *transition(cocos2d::Scene *nextScene);
        virtual bool isLazy() const = 0;

        inline float getTransitionTime() const { return transitionTime; }
        inline void setTransitionTime(const float t) { transitionTime = t; }

        virtual void onEnter(const TransitionArguments &args) { }
        virtual void onExit(const TransitionArguments &args) { }

        virtual void onEnter() { cocos2d::Layer::onEnter(); } // override
        virtual void onExit() { cocos2d::Layer::onExit(); } // override
    };

    class ManagedCCScene : public cocos2d::Scene
    {
    private:
        SceneManager *manager;
    public:
        CREATE_FUNC(ManagedCCScene);
        ~ManagedCCScene();

        inline void setManager(SceneManager *manager) { this->manager = manager; }
    };

    typedef std::vector<ManagedSceneBase *> SceneList;

    template<typename T>
    class SceneBase : public ManagedSceneBase
    {
        friend class SceneManager;
    private:
        static int _uid;
        static bool _isLazy;
        ManagedCCScene *scene;

        inline void setManager(SceneManager *manager) { scene->setManager(manager); }
    public:
        inline int getID() const { return _uid; }
        inline cocos2d::Scene *getScene() const { return scene; }

        bool isLazy() const { return _isLazy; } // override

        SceneBase()
        {
            scene = ManagedCCScene::create();
            scene->addChild(this);
        }

        virtual ~SceneBase() { }
    };

    template<typename T>
    int SceneBase<T>::_uid = -1;

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
        friend class ManagedCCScene;
    private:
        SceneList scenes;
        DebugLayer *debugLayer;
        Debugger *debugger;
        bool showDebugLayer;

        cocos2d::Ref *getCurrentScene();
        void removeScene(ManagedSceneBase *scene);
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

        template<typename T>
        T *cloneAndRegister();

        void setDebugMode(const bool debugMode);
        inline bool isDebugMode() const { return showDebugLayer; }
        inline Debugger *getDebugger() { return debugger; }
    };

    /* SceneManager impl */
    template<typename T>
    void SceneManager::addScene(const bool lazy)
    {
        if(T::_uid >= 0)
        {
            return;
        }

        T::_uid = static_cast<int>(scenes.size());
        T::_isLazy = lazy;
        if(lazy)
        {
            scenes.push_back(nullptr);
        }
        else
        {
            T *t = T::create();
            t->getScene()->retain();
            t->setManager(this);
            scenes.push_back(t);
        }
    }

    template<typename T>
    T *SceneManager::getScene()
    {
        if(T::_uid == -1)
        {
            CCAssert(false, "scene is not registered.");
            return nullptr;
        }

        ManagedSceneBase *scene = scenes[T::_uid];
        if(scene)
        {
            return static_cast<T *>(scene);
        }

        T *r = T::create();
        r->setManager(this);
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
            {
                current->getScene()->release();
            }

            scenes[current->getID()] = nullptr;
        }

        if(current)
        {
            cocos2d::Director::getInstance()->replaceScene(current->transition(getScene<T>()->getScene()));
        }
        else
        {
            cocos2d::Director::getInstance()->replaceScene(getScene<T>()->getScene());
        }
    }

    template<typename T>
    void SceneManager::forwardScene()
    {
        ManagedSceneBase *current = dynamic_cast<ManagedSceneBase *>(getCurrentScene());
        cocos2d::Scene *next = nullptr;

        if(current)
        {
            next = current->transition(getScene<T>()->getScene());
        }
        else
        {
            next = getScene<T>()->getScene();
        }

        cocos2d::Director::getInstance()->pushScene(next);
    }

    template<typename T>
    T *SceneManager::cloneAndRegister()
    {
        if(T::_uid == -1)
        {
            addScene<T>(false);
            return nullptr;
        }

        T *current = dynamic_cast<T *>(getCurrentScene());
        if(!current)
        {
            CCAssert(false, "current running scene is not under SceneManager.");
            return nullptr;
        }

        T *t = T::create();
        t->setManager(this);
        t->getScene()->retain();
        scenes[T::_uid] = t;
        current->getScene()->release();
        return current;
    }
}

#endif

