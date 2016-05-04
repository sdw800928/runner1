#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameTransferScene.h"
#include "GameLayer.h"
#include "Definitions.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;

//using namespace cocostudio::timeline;

unsigned int TransferScore;
unsigned int myMapIndex;
unsigned int TransferEnergy;


Scene* GameTransferScene::createScene(int score, int map_index, int energy )
{
    TransferScore = score;
    myMapIndex = map_index;
    TransferEnergy = energy;

    auto scene = Scene::create();
    
    auto layer = GameTransferScene::create();

    scene->addChild(layer);

    return scene;
}


bool GameTransferScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    this->scheduleOnce(schedule_selector( GameTransferScene::GoToGameScene), DISPLAY_TIME_SPLASH_SCENE);
    
    auto backgroundSprite = Sprite::create("pic/Splash Screen.png");
    backgroundSprite->setPosition( Point( visibleSize.width/2 + origin.x, visibleSize.height/2+origin.y ));
    
    this->addChild(backgroundSprite);
    
    return true;
}

void GameTransferScene::GoToGameScene( float dt )
{
    auto scene = GameLayer::createScene(TransferScore, myMapIndex, TransferEnergy);
    Director::getInstance()->replaceScene( TransitionFade::create( TRANSITION_TIME, scene));
}


