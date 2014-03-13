#include "SceneManager.h"

namespace oreore
{
    using namespace cocos2d;

    /* 
     * CCDirectorのシーンスタックに強制的にアクセスするためのクラス
     * 本来はよくない使い方だけどこうする他ないので仕方ない
     */
    class DirectorWrapper : public Director
    {
    private:
    public:
        inline Vector<Scene *> &getSceneStack() { return _scenesStack; }
        inline void setSendCleanupToScene(const bool s) { _sendCleanupToScene = s; }
        inline void setNextSceneWrapper(Scene *scene) { _nextScene = scene; }
    };


    /* ManagedSceneBase */
    ManagedSceneBase::ManagedSceneBase() : transitionTime(0.5f)
    {
    
    }

    ManagedSceneBase::~ManagedSceneBase()
    {
    
    }

    bool ManagedSceneBase::init()
    {
        if(!Layer::init())
            return false;
        return true;
    }

    TransitionScene *ManagedSceneBase::transition(Scene *nextScene)
    {
        return TransitionFade::create(transitionTime, nextScene, Color3B::BLACK);
    }


    /* SceneManager */
    SceneManager::SceneManager() : debugLayer(null),
#ifdef COCOS2D_DEBUG
        showDebugLayer(true)
#else
        showDebugLayer(false)
#endif
    {
        init();
    }

    SceneManager::~SceneManager()
    {
        if(debugLayer)
            debugLayer->release();
    }

    void SceneManager::init()
    {
#ifdef COCOS2D_DEBUG
        debugLayer = DebugLayer::create();
        Director::getInstance()->setNotificationNode(debugLayer);
#endif
    }

    Object *SceneManager::getCurrentScene()
    {
        Scene *scene = Director::getInstance()->getRunningScene();
        CCAssert(scene->getChildrenCount() == 1, "CCScene must have only 1 CCLayer.");
        return scene->getChildren().front();
    }

    void SceneManager::backScene(const bool disposeScene)
    {
        Scene *next = null;
        DirectorWrapper *dir = static_cast<DirectorWrapper *>(Director::getInstance());
        Vector<Scene *> &stack = dir->getSceneStack();
        stack.erase(stack.size() - 1);
        if(stack.empty())
        {
            dir->end();
            return;
        }

        Scene *scene = stack.back();
        ManagedSceneBase *current = dynamic_cast<ManagedSceneBase *>(getCurrentScene());

        if(disposeScene)
        {
            if(!current->isLazy())
                current->getScene()->release();
            scenes[current->getID()] = null;
        }

        dir->setSendCleanupToScene(true);

        if(current)
            next = current->transition(scene);
        else
            next = scene;

        Director::getInstance()->replaceScene(next);
    }

    void SceneManager::setDebugMode(const bool debugMode)
    {
        if(showDebugLayer == debugMode)
            return;

        if(debugMode)
        {
            if(!debugLayer)
                debugLayer = DebugLayer::create();
            Director::getInstance()->setNotificationNode(debugLayer);
            debugLayer->release();
        }
        else
        {
            if(debugLayer)
                debugLayer->retain();
            Director::getInstance()->setNotificationNode(null);
        }
        showDebugLayer = debugMode;
    }
}