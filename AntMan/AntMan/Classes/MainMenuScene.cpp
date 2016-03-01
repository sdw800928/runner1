#include "MainMenuScene.h"
#include "GameScene.h"
#include "Definitions.h"

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
    auto titleSprite = Sprite::create("pic/Title.png");
    titleSprite->setPosition( Point( visibleSize.width/2 + origin.x,
                                     visibleSize.height - titleSprite->getContentSize( ).height)
                                    );
    this->addChild(titleSprite);
    
    //setPlayItem
    auto playItem = MenuItemImage::create("pic/Play Button.png", "pic/Play Button Clicked.png",
                                          CC_CALLBACK_1(MainMenuScene::GoToGameScene, this));
    playItem->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    //setMenu
    auto menu = Menu::create(playItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    return true;
}

// JumpToGameScene
void MainMenuScene::GoToGameScene( cocos2d::Ref *sender)
{
    auto scene = GameScene::createScene();
    Director::getInstance(  )->replaceScene( TransitionFade::create(TRANSITION_TIME, scene));
}
