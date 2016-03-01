#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* GameOverScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = GameOverScene::create();

    scene->addChild(layer);

    return scene;
}


bool GameOverScene::init()
{    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto backgroundSprite = Sprite::create("pic/Background.png");
    backgroundSprite->setPosition( Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    this->addChild(backgroundSprite);
    
    // set Gameover Scene to Retry GameScene
    auto retryItem = MenuItemImage::create("pic/Retry Button.png", "pic/Play Button Clicked.png", CC_CALLBACK_1(GameOverScene::GoToGameScene, this) );
    
    retryItem->setPosition( Point( visibleSize.width /2 + origin.x , visibleSize.height / 4 * 3 ));
    
    // set GameOver Scene to MenuScene
    auto mainMenuItem = MenuItemImage::create("pic/Menu Button.png", "pic/Menu Button Clicked.png", CC_CALLBACK_1(GameOverScene::GoToMainMenuScene, this) );
    
    mainMenuItem->setPosition( Point( visibleSize.width /2 + origin.x , visibleSize.height / 4 ));
    
    auto menu = Menu::create( retryItem, mainMenuItem, NULL );
    menu->setPosition( Point::ZERO);
    this->addChild( menu );
    

    return true;
}

void GameOverScene::GoToMainMenuScene( cocos2d::Ref *sender )
{
    auto scene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene( TransitionFade::create( TRANSITION_TIME, scene));
    
}
void GameOverScene::GoToGameScene( cocos2d::Ref *sender )
{
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene( TransitionFade::create( TRANSITION_TIME, scene));
}

