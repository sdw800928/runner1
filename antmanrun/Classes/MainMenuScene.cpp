#include "MainMenuScene.h"
#include "GameLayer.h"
#include "Definitions.h"
#include "ScoreScene.h"
#include "OptionScene.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;


Scene* MainMenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainMenuScene::create();
    scene->addChild(layer);
    return scene;
}

//Initial 
bool MainMenuScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //setbackGround
    auto backgroundSprite = Sprite::create("pic/Background.png");
    backgroundSprite->setPosition( Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(backgroundSprite);
    
    //setTitle
   // auto titleSprite = Sprite::create("pic/Title.png");
   // titleSprite->setPosition( Point( visibleSize.width/2 + origin.x,
    //visibleSize.height - titleSprite->getContentSize( ).height));
    //this->addChild(titleSprite);
    
    //setTutorialItem
    auto tutorialItem = MenuItemImage::create("GameAssets/tutorial.png", "GameAssets/tutorialpress.png",
                                          CC_CALLBACK_1(MainMenuScene::GoToTutorialScene, this));
    tutorialItem->setPosition(Point(visibleSize.width*3/4 + origin.x, visibleSize.height/2+200 -25 + origin.y));
    
    //setPlayItem
    auto playItem = MenuItemImage::create("pic/play.png", "pic/playpress.png",
                                          CC_CALLBACK_1(MainMenuScene::GoToGameScene, this));
    playItem->setPosition(Point(visibleSize.width*3/4 + origin.x, visibleSize.height/2+100 -25 + origin.y));
    
    //setOptionItem
    auto optionItem = MenuItemImage::create("pic/option.png", "pic/optionpress.png",
                                          CC_CALLBACK_1(MainMenuScene::GoToOptionScene, this));
    optionItem->setPosition(Point(visibleSize.width*3/4 + origin.x, visibleSize.height/2 -25 + origin.y));
    
    //setScoreItem
    auto scoreItem = MenuItemImage::create("pic/Score.png", "pic/Scorepress.png",
                                          CC_CALLBACK_1(MainMenuScene::GoToScoreScene, this));
    scoreItem->setPosition(Point(visibleSize.width*3/4 + origin.x, visibleSize.height/2-125 + origin.y));
    
    //setMenu
    auto menu = Menu::create(tutorialItem, playItem, optionItem, scoreItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgmstartgame.mp3", true);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
    
    return true;
}

// JumpToGameScene
void MainMenuScene::GoToGameScene( cocos2d::Ref *sender)
{
    auto scene = GameLayer::createScene(0,0,0);
    Director::getInstance(  )->replaceScene( TransitionFade::create(TRANSITION_TIME, scene));
}

void MainMenuScene::GoToTutorialScene( cocos2d::Ref *sender) {
    
    auto scene = GameLayer::createScene(0,-1,0);
    Director::getInstance(  )->replaceScene( TransitionFade::create(TRANSITION_TIME, scene));
}

void MainMenuScene::GoToScoreScene( cocos2d::Ref *sender) {
    
    auto scene = ScoreScene::createScene();
    Director::getInstance(  )->replaceScene( TransitionFade::create(TRANSITION_TIME, scene));
}

void MainMenuScene::GoToOptionScene( cocos2d::Ref *sender) {
    
    auto scene = OptionScene::createScene();
    Director::getInstance(  )->replaceScene( TransitionFade::create(TRANSITION_TIME, scene));
}
