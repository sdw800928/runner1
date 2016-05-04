#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameOverScene.h"
#include "GameLayer.h"
#include "MainMenuScene.h"
#include "Definitions.h"
#include "SimpleAudioEngine.h"
#include <algorithm>    // std::sort
#include <vector>       // std::vector


USING_NS_CC;

using namespace cocostudio::timeline;

unsigned int myScore;
int level_index;

Scene* GameOverScene::createScene(int score, int index)
{
    level_index = index;
    myScore = score;
    CCLOG("%i\n",myScore);

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
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    auto backgroundSprite = Sprite::create("pic/deadbg.png");

    backgroundSprite->setPosition( Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//backgroundSprite->setScaleX(visibleSize.width);
    this->addChild(backgroundSprite);
    
    // add gameover antman
    auto sleepAntManSprite = Sprite::create("pic/sleepant.png");
    
    sleepAntManSprite->setPosition( Point(visibleSize.width/4 + origin.x, visibleSize.height/4 + origin.y));
    //backgroundSprite->setScaleX(visibleSize.width);
    this->addChild(sleepAntManSprite, 100);
    
    // set Gameover Scene to Retry GameScene
    auto retryItem = MenuItemImage::create("pic/tryagain.png", "pic/tryagainpress.png", CC_CALLBACK_1(GameOverScene::GoToGameScene, this) );
    
    retryItem->setPosition( Point( visibleSize.width *3/4, visibleSize.height *3/5 ));
    
    // set GameOver Scene to MenuScene
    auto mainMenuItem = MenuItemImage::create("pic/menu.png", "pic/menupress.png", CC_CALLBACK_1(GameOverScene::GoToMainMenuScene, this) );
    
    mainMenuItem->setPosition( Point( visibleSize.width *3/4, visibleSize.height / 5 ));
    
    auto menu = Menu::create( retryItem, mainMenuItem, NULL );
    
    if ( myScore >= 30 ) {
        // set Gameover Scene to Revive GameScene
        auto reviveItem = MenuItemImage::create("pic/tryagain.png", "pic/tryagainpress.png", CC_CALLBACK_1(GameOverScene::ReviveGameScene, this) );
    
        reviveItem->setPosition( Point( visibleSize.width *3/4, visibleSize.height *2/5 ));
    
    
        menu = Menu::create( retryItem, mainMenuItem, reviveItem, NULL );
    }

    menu->setPosition( Point::ZERO);
    this->addChild( menu );
    
    showScore(this, myScore);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/Die.mp3");

    return true;
}

void GameOverScene::ReviveGameScene( cocos2d::Ref *sender ) {
    
    auto scene = GameLayer::createScene( myScore-30, level_index, 0);
    Director::getInstance()->replaceScene( TransitionFade::create( TRANSITION_TIME, scene));
}

void GameOverScene::GoToMainMenuScene( cocos2d::Ref *sender )
{
    auto scene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene( TransitionFade::create( TRANSITION_TIME, scene));
    
}
void GameOverScene::GoToGameScene( cocos2d::Ref *sender )
{
    auto scene = GameLayer::createScene(0,0,0);
    if ( level_index == -1 )
        scene = GameLayer::createScene(0,-1,0);
    // auto scene = GameLayer::createScene(0,0,0);
    Director::getInstance()->replaceScene( TransitionFade::create( TRANSITION_TIME, scene));
}

void GameOverScene::showScore( cocos2d::Layer *layer, int score ) {
    
    getHighestHistorySorce();

    
    if ( score_arr[4] < score )
        score_arr[4] = score;
    
    // sort
    for ( int i = 0; i < 5; i++) {
        int max =-1;
        int max_index = 0;
        for ( int j = i; j < 5; j++) {
            if ( max < score_arr[j] ) {
                max = score_arr[j];
                max_index = j;
            }
        }
        int tmp = score_arr[max_index];
        score_arr[max_index] = score_arr[i];
        score_arr[i] = tmp;
    }
    
    CCUserDefault::sharedUserDefault()->setIntegerForKey("FirstScore",score_arr[0]);//修改存档
    CCUserDefault::sharedUserDefault()->setIntegerForKey("SecondScore",score_arr[1]);//修改存档
    CCUserDefault::sharedUserDefault()->setIntegerForKey("ThirdScore",score_arr[2]);//修改存档
    CCUserDefault::sharedUserDefault()->setIntegerForKey("FourthScore",score_arr[3]);//修改存档
    CCUserDefault::sharedUserDefault()->setIntegerForKey("FifthScore",score_arr[4]);//修改存档
    
//    CCUserDefault::sharedUserDefault()->setIntegerForKey("FirstScore", 0);//修改存档
//    CCUserDefault::sharedUserDefault()->setIntegerForKey("SecondScore", 0);//修改存档
//    CCUserDefault::sharedUserDefault()->setIntegerForKey("ThirdScore", 0);//修改存档
//    CCUserDefault::sharedUserDefault()->setIntegerForKey("FourthScore", 0);//修改存档
//    CCUserDefault::sharedUserDefault()->setIntegerForKey("FifthScore", 0);//修改存档

    
    /*
     if (score>highestHistoryScore)
     {
     CCUserDefault::sharedUserDefault()->setIntegerForKey("HighestScore",score);//修改存档
     highestHistoryScore=score//最高分重新设定...后面还有其他东西
     }
     */
    
    ////////////////////////////////////////////////////////////////////
    float score_positionX = visibleSize.width*3/4;
    for ( auto s : arr_label ) {
        s->removeFromParent();
    }
    arr_label.clear();
    
    // if score = 0
    if ( score == 0 ) {
        label = Sprite::create("pic/0.png");
        arr_label.push_back(label);
        label->setPosition( Point(score_positionX + origin.x, visibleSize.height*4/5 + origin.y));
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
        arr_label.push_back(label);
        label->setPosition( Point(score_positionX + origin.x, visibleSize.height*4/5 + origin.y));
        this->addChild(label);
        score_positionX -= 50;
        score /= 10;
    }
    
    
    /*
    
    if ( label != NULL )
        label->removeFromParent();
    //创建标签
    label = CCLabelTTF::create(std::to_string(myScore), "Arial", 48);
    //设置标签文字颜色
    label->setColor(ccc3(255,255,255));
    //设置标签位置
    label->setPosition(ccp(visibleSize.width*3/4,visibleSize.height*3/4));
    
    //设置锚点为左下角
    // label->setAnchorPoint(CCPointZero);
    
    //设置标签的横向对齐方式为向左对齐，这样标签内容增加，只会向右增加
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    
    //添加为子节点
    layer->addChild(label,1);
     
     */
}

bool GameOverScene::isHaveSaveFile()//判断存档是否存在
{
    if(!CCUserDefault::sharedUserDefault()->getBoolForKey("isHaveSaveFileXml"))//通过设置的bool型标志位判断，如果不存在
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey("isHaveSaveFileXml", true);//写入bool判断位
        CCUserDefault::sharedUserDefault()->setIntegerForKey("FirstScore",0);//写入初始分数0
        CCUserDefault::sharedUserDefault()->flush();
        CCUserDefault::sharedUserDefault()->setIntegerForKey("SecondScore",0);//写入初始分数0
        CCUserDefault::sharedUserDefault()->flush();
        CCUserDefault::sharedUserDefault()->setIntegerForKey("ThirdScore",0);//写入初始分数0
        CCUserDefault::sharedUserDefault()->flush();
        CCUserDefault::sharedUserDefault()->setIntegerForKey("FourthScore",0);//写入初始分数0
        CCUserDefault::sharedUserDefault()->flush();
        CCUserDefault::sharedUserDefault()->setIntegerForKey("FifthScore",0);//写入初始分数0
        
        CCUserDefault::sharedUserDefault()->flush();//设置完一定要调用flush，才能从缓冲写入io
        return false;
    }
    else
    {
        return true;
    }
}

void GameOverScene::getHighestHistorySorce()
{
    if (isHaveSaveFile())//如果存在存档
    {
        // GameOverScene::highestHistoryScore=CCUserDefault::sharedUserDefault()->getIntegerForKey("HighestScore",0);//读取历史最高分
        
        score_arr[0] = CCUserDefault::sharedUserDefault()->getIntegerForKey("FirstScore",0);
        score_arr[1] = CCUserDefault::sharedUserDefault()->getIntegerForKey("SecondScore",0);
        score_arr[2] = CCUserDefault::sharedUserDefault()->getIntegerForKey("ThirdScore",0);
        score_arr[3] = CCUserDefault::sharedUserDefault()->getIntegerForKey("FourthScore",0);
        score_arr[4] = CCUserDefault::sharedUserDefault()->getIntegerForKey("FifthScore",0);
        
    }
}

