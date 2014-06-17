#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "../../oreore/oreore/oreore.h"

using namespace cocos2d;
using namespace oreore;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview)
    {
        glview = GLView::create("oreore example");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // DLog is oreore Debug Log Utility
    // set show number
    DLog::setShowTime(true);


    // register scene manager
    // if call addScene<>(false), the scene is created on application start and
    // does not released when scene is changed (always stay on memory).
    // if addScene<>(true), the scene is created on its needed, and it is released when scene is changed.
    SceneManager::getInstance().addScene<HelloWorld>(false);

    // get cocos2d::Scene
    auto scene = SceneManager::getInstance().getScene<HelloWorld>()->getScene();

    // run
    director->runWithScene(scene);

    dlog("walcome to oreore library!");
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
