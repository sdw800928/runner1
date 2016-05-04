
#include "Player.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Definitions.h"


bool Player::initWithFile(const char *pszFilename)
{
    CCAssert(pszFilename != NULL, "Invalid filename for Player");
    
    //player初始化
    bool bRet = Sprite::initWithFile(pszFilename);
    
    return bRet;
}

Player* Player::create(const char *pszFileName)
{
    Player *pobPlayer = new Player();
    if (pobPlayer && pobPlayer->initWithFile(pszFileName))
    {
        //player 创建成功后初始化
        pobPlayer->_jump=0;
        pobPlayer->_velocity=cocos2d::Point(0.f,-2.0f);
        pobPlayer->autorelease();
        return pobPlayer;
    }
    CC_SAFE_DELETE(pobPlayer);
    return NULL;
}

cocos2d::Animation* Player::get_role_animation(){
    
    auto cache = cocos2d::SpriteFrameCache::getInstance();
    
    cache->removeSpriteFrames();
    
    cache->addSpriteFramesWithFile("Antman.plist");
    
    cocos2d::Vector<cocos2d::SpriteFrame *> arr;
    char str[100] = {0};
    for (int i=1; i<7; i++)   //将动画每一帧存入数组
    {
        sprintf(str, "%d.png", i);
        auto frame_2 = cache->SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        arr.pushBack(frame_2);
    }
    
    // auto animation = cocos2d::Animation::createWithSpriteFrames(arr, 0.12f);//根据数组每1/8秒执行一次动画
    auto animation = cocos2d::Animation::createWithSpriteFrames(arr, 0.08f);
    return animation;
}


cocos2d::Animation* Player::get_role_animation_Sprint(){
    
    auto cache = cocos2d::SpriteFrameCache::getInstance();
    
    cache->removeSpriteFrames();
    
    cache->addSpriteFramesWithFile("GameAssets/sprintant.plist");
    
    cocos2d::Vector<cocos2d::SpriteFrame *> arr;
    char str[100] = {0};
    for (int i=1; i<7; i++)   //将动画每一帧存入数组
    {
        sprintf(str, "%d.png", i);
        auto frame_2 = cache->SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        arr.pushBack(frame_2);
    }
    
    // auto animation = cocos2d::Animation::createWithSpriteFrames(arr, 0.12f);//根据数组每1/8秒执行一次动画
    auto animation = cocos2d::Animation::createWithSpriteFrames(arr, 0.08f);
    return animation;
}

cocos2d::Animation* Player::get_role_animation_Die(){
    
    auto cache = cocos2d::SpriteFrameCache::getInstance();
        
    cache->removeSpriteFrames();
    
    cache->addSpriteFramesWithFile("GameAssets/diecloud.plist");
    
    cocos2d::Vector<cocos2d::SpriteFrame *> arr;
    char str[100] = {0};
    for (int i=1; i<4; i++)   //将动画每一帧存入数组
    {
        CCLOG("die animation: %i\n",i);
        sprintf(str, "%d.png", i);
        auto frame_2 = cache->SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        arr.pushBack(frame_2);
    }
    
    // auto animation = cocos2d::Animation::createWithSpriteFrames(arr, 0.12f);//根据数组每1/8秒执行一次动画
    auto animation = cocos2d::Animation::createWithSpriteFrames(arr, 0.08f);
    return animation;
}


void Player::update(float dt, bool isFlying, bool pauseflag, bool isSpeeding, float _bgMoveSpeed)
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    
    if ( pauseflag == false ) {
        
        cocos2d::Point gravity= cocos2d::Point(0.f,-0.5f);  //设置重力偏移
    
        if ( isSpeeding == false )
            this->_velocity+=gravity;   //更新当前速度
    
        //如果目前正在飛行
        if ( isFlying == true && _jumpable ) {
            cocos2d::Director::getInstance()->getActionManager()->pauseTarget(this);
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/Wing.mp3");
            this->_velocity= cocos2d::Point(0.f,8.f);
            _jumpable=false;
        }
    
        else if ( (_jump==1||_jump==2) && _jumpable) {      //执行一段跳二段跳时 更新高度 停止动画
            cocos2d::Director::getInstance()->getActionManager()->pauseTarget(this);
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/Jump.mp3");
            if ( _big == true )
                this->_velocity= cocos2d::Point(0.f,13.f);
            else
                this->_velocity= cocos2d::Point(0.f,10.f);
            _jumpable=false;
        }
        if ( isSpeeding == false )
            this->_desiredPosition = this->getPosition() + this->_velocity;
        else
            this->_desiredPosition = cocos2d::Point(this->getPositionX()+_bgMoveSpeed, visibleSize.height*3/5);
        
        // CCLOG("position: %f\n", this->getPositionX());
    }
    
    
}

cocos2d::Rect Player::collisionBoundingBox()
{
    
    cocos2d::Rect collisionBox = this->boundingBox();  //生成玩家绑定盒
     
    cocos2d::Point diff = this->_desiredPosition- this->getPosition(); //计算和更新玩家当前位置和期望位置的偏移量
    collisionBox.origin.x += diff.x;
    collisionBox.origin.y += diff.y;
    return collisionBox;
}
