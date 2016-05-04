#include "SplashScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;


Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music/Point.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music/Jump.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music/Die.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music/SElaunch.mp3");
    // CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music/SEholdfire.mp3");
    
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.4);
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    this->scheduleOnce(schedule_selector( SplashScene::GoToMainMenuScene), DISPLAY_TIME_SPLASH_SCENE);
    
    auto backgroundSprite = Sprite::create("pic/Splash Screen.png");
    backgroundSprite->setPosition( Point( visibleSize.width/2 + origin.x, visibleSize.height/2+origin.y ));
    
    this->addChild(backgroundSprite);

    return true;
}

void SplashScene::GoToMainMenuScene(float dt)
{
    auto scene = MainMenuScene::createScene();
    
    Director::getInstance( )->replaceScene( TransitionFade::create(TRANSITION_TIME, scene));
    
}