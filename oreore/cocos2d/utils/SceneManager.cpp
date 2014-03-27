#include "SceneManager.h"

namespace oreore
{
    using namespace cocos2d;

    /* 
     * CCDirectorのシーンスタックに強制的にアクセスするためのクラス
     * 本来はよくない使い方だけどこうする他ないので仕方ない
     */
    class DirectorWrapper : public CCDirector
    {
    private:
    public:
        inline CCArray *getSceneStack() { return m_pobScenesStack; }
        inline void setSendCleanupToScene(const bool s) { m_bSendCleanupToScene = s; }
        inline void setNextSceneWrapper(CCScene *scene) { m_pNextScene = scene; }
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
        if(!CCLayer::init())
            return false;
        return true;
    }

    CCTransitionScene *ManagedSceneBase::transition(CCScene *nextScene)
    {
        return CCTransitionFade::create(transitionTime, nextScene, ccBLACK);
    }

    /* SceneManager */
    SceneManager::SceneManager() : debugLayer(null), debugger(null),
#ifdef COCOS2D_DEBUG
        showDebugLayer(true)
#else
        showDebugLayer(false)
#endif
    {
        init();
    }


    /* ManagedCCSCene */
    ManagedCCScene::~ManagedCCScene()
    {
        CCAssert(getChildrenCount() == 1, "CCScene must have only 1 CCLayer.");
        manager->removeScene(static_cast<ManagedSceneBase *>(getChildren()->objectAtIndex(0))->getID());
    }

    /* SceneManager */
    SceneManager::~SceneManager()
    {
        if(debugLayer)
        {
            debugLayer->release();
            delete debugger;
        }
    }

    void SceneManager::init()
    {
#ifdef COCOS2D_DEBUG
        debugLayer = DebugLayer::create();
        CCDirector::sharedDirector()->setNotificationNode(debugLayer);
        debugger = new Debugger(debugLayer);
#endif
    }

    void SceneManager::removeScene(const int uid)
    {
        scenes[uid] = null;
    }

    CCObject *SceneManager::getCurrentScene()
    {
        CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
        CCAssert(scene->getChildrenCount() == 1, "CCScene must have only 1 CCLayer.");
        return scene->getChildren()->objectAtIndex(0);
    }

    void SceneManager::backScene(const bool disposeScene)
    {
        cocos2d::CCScene *next = null;
        DirectorWrapper *dir = static_cast<DirectorWrapper *>(CCDirector::sharedDirector());
        CCArray *stack = dir->getSceneStack();
        stack->removeLastObject();
        const unsigned int count = stack->count();
        if(count == 0)
        {
            dir->end();
            return;
        }

        CCScene *scene = static_cast<CCScene *>(stack->lastObject());
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

        cocos2d::CCDirector::sharedDirector()->replaceScene(next);
    }

    void SceneManager::setDebugMode(const bool debugMode)
    {
        if(showDebugLayer == debugMode)
            return;

        if(debugMode)
        {
            if(!debugLayer)
            {
                debugLayer = DebugLayer::create();
                debugger = new Debugger(debugLayer);
            }
            CCDirector::sharedDirector()->setNotificationNode(debugLayer);
            debugLayer->release();
        }
        else
        {
            if(debugLayer)
                debugLayer->retain();
            CCDirector::sharedDirector()->setNotificationNode(null);
        }
        showDebugLayer = debugMode;
    }
}