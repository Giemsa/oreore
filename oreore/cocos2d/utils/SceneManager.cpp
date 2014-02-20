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


    /* LoadingScene */
    LoadingScene *LoadingScene::create(const ccColor4B &color)
    {
        LoadingScene *r = new LoadingScene();
        if(r && r->init(color))
        {
            r->autorelease();
            return r;
        }
        delete r;
        return null;
    }

    LoadingScene *LoadingScene::create(const ccColor4B &color, const float maxDuration)
    {
        LoadingScene *r = new LoadingScene();
        if(r && r->init(color, maxDuration))
        {
            r->autorelease();
            return r;
        }
        delete r;
        return null;
    }

    LoadingScene::LoadingScene() : disposeNext(false)
    {
        scene = cocos2d::CCScene::create();
        scene->retain();
        scene->addChild(this);
    }

    LoadingScene::~LoadingScene()
    {
        scene->release();
    }

    bool LoadingScene::init(const ccColor4B &color, const float maxDuration)
    {
        if(!CCLayerColor::initWithColor(color))
            return false;

        this->maxDuration = maxDuration;

        return true;
    }

    void LoadingScene::onEnter()
    {
        CCLayerColor::onEnter();
        duration = 0.0f;
        scheduleUpdate();
    }

    void LoadingScene::update(float dt)
    {
        duration += dt;
        if(duration > maxDuration)
        {
            unscheduleUpdate();
      
            if(currentScene)
                CCDirector::sharedDirector()->replaceScene(currentScene->transition(nextScene));
            else
                CCDirector::sharedDirector()->replaceScene(nextScene);
            if(disposeNext)
                currentScene->getScene()->release();
        }
    }

    /* SceneManager */
    SceneManager::SceneManager() : loadingScene(null)
    {
    
    }

    SceneManager::~SceneManager()
    {
    
    }

    void SceneManager::init()
    {
    
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
            current->getScene()->autorelease();
            scenes[current->getID()] = null;
        }

        dir->setSendCleanupToScene(true);

        if(loadingScene)
        {
            loadingScene->nextScene = scene;
            loadingScene->disposeNext = disposeScene;
            if(disposeScene)
                current->getScene()->retain();
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
                next = current->transition(scene);
            else
                next = scene;
        }
        cocos2d::CCDirector::sharedDirector()->replaceScene(next);
    }

    LoadingScene *SceneManager::setLoadingScene(LoadingScene *scene)
    {
        LoadingScene *tmp = loadingScene;
        if(tmp)
            tmp->release();
        if(scene)
        {
            scene->retain();
            scene->manager = this;
        }
        loadingScene = scene;
        return tmp;
    }
}