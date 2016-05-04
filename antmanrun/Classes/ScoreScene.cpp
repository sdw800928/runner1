#include "MainMenuScene.h"
#include "ScoreScene.h"
#include "Definitions.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;


Scene* ScoreScene::createScene()
{
    auto scene = Scene::create();
    auto layer = ScoreScene::create();
    scene->addChild(layer);
    return scene;
}

//Initial 
bool ScoreScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    //setbackGround
    auto backgroundSprite = Sprite::create("pic/Background.png");
    backgroundSprite->setPosition( Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(backgroundSprite);
    
    //setTitle
   // auto titleSprite = Sprite::create("pic/Title.png");
   // titleSprite->setPosition( Point( visibleSize.width/2 + origin.x,
    //visibleSize.height - titleSprite->getContentSize( ).height));
    //this->addChild(titleSprite);
    

    //setPlayItem
    auto playItem = MenuItemImage::create("pic/menu.png", "pic/menupress.png",
                                          CC_CALLBACK_1(ScoreScene::GoToMainMenuScene, this));
    playItem->setPosition(Point(visibleSize.width*1/8 + origin.x, visibleSize.height*7/8 + origin.y));

    //setMenu
    auto menu = Menu::create( playItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    
    for ( auto s : arr_label ) {
        for ( auto t : s )
            t->removeFromParent();
    }
    arr_label.clear();
    
    
    getHighestHistorySorce();
    
    for ( int i = 0; i < 5; i++ ) {
        showScore(score_arr[i], i );
    }
    
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgmScore.mp3", true);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
    
    
    return true;
}

// JumpToGameScene
void ScoreScene::GoToMainMenuScene( cocos2d::Ref *sender)
{
    auto scene = MainMenuScene::createScene();
    Director::getInstance( )->replaceScene( TransitionFade::create(TRANSITION_TIME, scene));
}

bool ScoreScene::isHaveSaveFile()//判断存档是否存在
{
    if(!CCUserDefault::sharedUserDefault()->getBoolForKey("isHaveSaveFileXml"))//通过设置的bool型标志位判断，如果不存在
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey("isHaveSaveFileXml", true);//写入bool判断位
        CCUserDefault::sharedUserDefault()->setIntegerForKey("HighestScore",0);//写入初始分数0
        CCUserDefault::sharedUserDefault()->flush();//设置完一定要调用flush，才能从缓冲写入io
        return false;
    }
    else
    {
        return true;
    }
}

void ScoreScene::getHighestHistorySorce()
{
    if (isHaveSaveFile())//如果存在存档
    {
        score_arr[0] = CCUserDefault::sharedUserDefault()->getIntegerForKey("FirstScore",0);
        score_arr[1] = CCUserDefault::sharedUserDefault()->getIntegerForKey("SecondScore",0);
        score_arr[2] = CCUserDefault::sharedUserDefault()->getIntegerForKey("ThirdScore",0);
        score_arr[3] = CCUserDefault::sharedUserDefault()->getIntegerForKey("FourthScore",0);
        score_arr[4] = CCUserDefault::sharedUserDefault()->getIntegerForKey("FifthScore",0);
    }
}

int ScoreScene::showScore(int score, int rank) {
    
    float score_positionX = visibleSize.width*3/4;
    
    // if score = 0
    if ( score == 0 ) {
        label = Sprite::create("pic/0.png");
        tmp_label_arr.push_back(label);
        // arr_label.push_back(tmp_label_arr);
        label->setPosition( Point(score_positionX + origin.x, visibleSize.height*3/4 - rank*100 + origin.y));
        this->addChild(label);
    }
    
    while( score ) {
        int digit = score%10;
        switch ( digit ) {
            case 0:
                label = Sprite::create("pic/0.png");
                break;
            case 1:
                label = Sprite::create("pic/1.png");
                break;
            case 2:
                label = Sprite::create("pic/2.png");
                break;
            case 3:
                label = Sprite::create("pic/3.png");
                break;
            case 4:
                label = Sprite::create("pic/4.png");
                break;
            case 5:
                label = Sprite::create("pic/5.png");
                break;
            case 6:
                label = Sprite::create("pic/6.png");
                break;
            case 7:
                label = Sprite::create("pic/7.png");
                break;
            case 8:
                label = Sprite::create("pic/8.png");
                break;
            case 9:
                label = Sprite::create("pic/9.png");
                break;
                
        }
        tmp_label_arr.push_back(label);
        label->setPosition( Point(score_positionX + origin.x, visibleSize.height*3/4 - rank*100 + origin.y));
        this->addChild(label);
        score_positionX -= 50;
        score /= 10;
    }
    arr_label.push_back(tmp_label_arr);

}

