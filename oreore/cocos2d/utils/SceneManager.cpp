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


    /* LoadingScene */
    LoadingScene *LoadingScene::create(const Color4B &color)
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

    LoadingScene *LoadingScene::create(const Color4B &color, const float maxDuration)
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
        scene = Scene::create();
        scene->retain();
        scene->addChild(this);
    }

    LoadingScene::~LoadingScene()
    {
        scene->release();
    }

    bool LoadingScene::init(const Color4B &color, const float maxDuration)
    {
        if(!LayerColor::initWithColor(color))
            return false;

        this->maxDuration = maxDuration;

        return true;
    }

    void LoadingScene::onEnter()
    {
        LayerColor::onEnter();
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
                Director::getInstance()->replaceScene(currentScene->transition(nextScene));
            else
                Director::getInstance()->replaceScene(nextScene);
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
        Director::getInstance()->replaceScene(next);
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