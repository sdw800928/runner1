#include "MainMenuScene.h"
#include "OptionScene.h"
#include "Definitions.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;


Scene* OptionScene::createScene()
{
    auto scene = Scene::create();
    auto layer = OptionScene::create();
    scene->addChild(layer);
    return scene;
}

//Initial 
bool OptionScene::init()
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

    //setPlayItem
    auto menuItem = MenuItemImage::create("pic/menu.png", "pic/menupress.png",
                                          CC_CALLBACK_1(OptionScene::GoToMainMenuScene, this));
    menuItem->setPosition(Point(visibleSize.width*1/8 + origin.x, visibleSize.height*7/8 + origin.y));
    
    //AddSoundEffectItem
    auto addSoundItem = MenuItemImage::create("sizebefore.png", "sizepress.png",
                                          CC_CALLBACK_1(OptionScene::addSEVolume, this));
    addSoundItem->setPosition(Point(visibleSize.width*3/4 + origin.x, visibleSize.height*3/5 + origin.y));
    addSoundItem->setScale( 0.5f );
    
    //MinusSoundEffectItem
    auto minusSoundItem = MenuItemImage::create("sizebefore.png", "sizepress.png",
                                          CC_CALLBACK_1(OptionScene::minusSEVolume, this));
    minusSoundItem->setPosition(Point(visibleSize.width*1/4 + origin.x, visibleSize.height*3/5 + origin.y));
    minusSoundItem->setScale( 0.5f );
    
    //AddBGMItem
    auto addBGMItem = MenuItemImage::create("sizebefore.png", "sizepress.png",
                                              CC_CALLBACK_1(OptionScene::addBGMVolume, this));
    addBGMItem->setPosition(Point(visibleSize.width*3/4 + origin.x, visibleSize.height*1/5 + origin.y));
    addBGMItem->setScale( 0.5f );
    
    //MinusBGMItem
    auto minusBGMItem = MenuItemImage::create("sizebefore.png", "sizepress.png",
                                                CC_CALLBACK_1(OptionScene::minusBGMVolume, this));
    minusBGMItem->setPosition(Point(visibleSize.width*1/4 + origin.x, visibleSize.height*1/5 + origin.y));
    minusBGMItem->setScale( 0.5f );
    
    //setMenu
    auto menu = Menu::create( menuItem, addSoundItem, minusSoundItem, addBGMItem, minusBGMItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    auto bgmLabel = Sprite::create( "pic/option.png" );
    bgmLabel->setPosition( Point( visibleSize.width*1/2 + origin.x, visibleSize.height*2/5 + origin.y ) );
    //bgmLabel->setVisible(false);
    this->addChild( bgmLabel );
    
    auto soundEffectLabel = Sprite::create( "pic/option.png" );
    soundEffectLabel->setPosition( Point( visibleSize.width*1/2 + origin.x, visibleSize.height*4/5 + origin.y ) );
    //soundEffectLabel->setVisible(false);
    this->addChild( soundEffectLabel );
    
    bgm=9;
    se=5;
    
    showBGMVolume();
    showSoundEffectVolume();
    
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/background.mp3", true);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(bgm/10.0f);
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(se/10.0f);
    
    return true;
}

// JumpToGameScene
void OptionScene::GoToMainMenuScene( cocos2d::Ref *sender)
{
    auto scene = MainMenuScene::createScene();
    Director::getInstance( )->replaceScene( TransitionFade::create(TRANSITION_TIME, scene));
}

void OptionScene::addBGMVolume( cocos2d::Ref *sender) {
    if ( bgm < 10 )
        bgm++;
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(bgm/10.0f);
    showBGMVolume();

}

void OptionScene::minusBGMVolume( cocos2d::Ref *sender) {
    if ( bgm > 0 )
        bgm--;
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(bgm/10.0f);
    showBGMVolume();
}

void OptionScene::addSEVolume( cocos2d::Ref *sender) {
    if ( se < 10 )
        se++;
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(se/10.0f);
    showSoundEffectVolume();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/getcoin.mp3");
}

void OptionScene::minusSEVolume( cocos2d::Ref *sender) {
    if ( se > 0 )
        se--;
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(se/10.0f);
    showSoundEffectVolume();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/getcoin.mp3");
}

void OptionScene::showBGMVolume() {
    
    if ( labelbgm != NULL )
        labelbgm->removeFromParent();
    //创建标签
    labelbgm = CCLabelTTF::create(std::to_string(bgm), "Arial", 48);
    //设置标签文字颜色
    labelbgm->setColor(ccc3(255, 255, 255));
    //设置标签位置
    labelbgm->setPosition(ccp(visibleSize.width*1/2, visibleSize.height*1/5));
    
    //设置锚点为左下角
    // label->setAnchorPoint(CCPointZero);
    
    //设置标签的横向对齐方式为向左对齐，这样标签内容增加，只会向右增加
    labelbgm->setHorizontalAlignment(kCCTextAlignmentLeft);
    
    //添加为子节点
    this->addChild(labelbgm,1);
}

void OptionScene::showSoundEffectVolume() {
    
    if ( labelse != NULL )
        labelse->removeFromParent();
    //创建标签
    labelse = CCLabelTTF::create(std::to_string(se), "Arial", 48);
    //设置标签文字颜色
    labelse->setColor(ccc3(255, 255, 255));
    //设置标签位置
    labelse->setPosition(ccp(visibleSize.width*1/2, visibleSize.height*3/5));
    
    //设置锚点为左下角
    // label->setAnchorPoint(CCPointZero);
    
    //设置标签的横向对齐方式为向左对齐，这样标签内容增加，只会向右增加
    labelse->setHorizontalAlignment(kCCTextAlignmentLeft);
    
    //添加为子节点
    this->addChild(labelse,1);
}
