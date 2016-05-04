#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Player.h"
#include "String.h"
#include "Definitions.h"
#include "Items.h"


class GameLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(int init_score, int scene_index, int init_energy);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    //Player
    Player* _player;
    void update(float dt);
    
    //工具函数
    //根据position返回对应的瓦片信息
    cocos2d::Point tileCoordForPosition(cocos2d::Point position);
    //根据瓦片位置生成相应瓦片的碰撞盒
    cocos2d::Rect tileRectFromTileCoords(cocos2d::Point tileCoords);
    //获取相交矩形
    cocos2d::Rect intersectsRect( cocos2d::Rect rect1, cocos2d::Rect rect2 );
    
    //生成用于检测player碰撞的瓦片集
    void getSurroundingTilesAtPosition(cocos2d::Point position, cocos2d::TMXLayer* layer,cocos2d::ValueMap *gids);
    //返回当前层被player覆盖的瓦片集
    void getOverlapTilesAtPosition(cocos2d::Point position, cocos2d::TMXLayer* layer,cocos2d::ValueMap* gids);

    
    //玩家与障碍物碰撞的检测和位置修复
    void checkForAndResolveCollisions(Player* player);
    //玩家同陷阱碰撞的检测和处理
    void handleHazardCollisions(Player* player);
    //玩家和金币碰撞的检测和处理
    void handleCoinCollisions(Player* player);
    //玩家和飛行物件碰撞的检测和处理
    void FlyCollisions(Player* player);
    //玩家和傳送門碰撞的检测和处理
    void handleGateCollisions(Player* player);
    //玩家和飛行物件碰撞的检测和处理
    // void handleFlyCollisions(Player* player);
    //玩家和提示物件碰撞的检测和处理
    void handleHintCollisions(Player* player);
    
    //跳跃回调函数
    // void jump_callback( Ref* ref );
    //加速回調函數
    void speed_up_callback( Ref* ref );
    void stop_speed_up_callback( float dt );
    void stop_invincible_callback( float dt );
    
    //觸碰螢幕事件 - 跳躍
    bool onTouchBegin( cocos2d::Touch *touch, cocos2d::Event *event );
    
   //player大小控制
    void Shrink_Magnify(Ref* ref );
    
    //player 暫停
    void Pause(Ref* ref );
    
    //玩家是否被卡主
    bool right=false;
    
    //玩家是否在加速
    bool isSpeeding=false;
    cocos2d::Sprite *EnergyBar = cocos2d::Sprite::create("pic/bar.png");
    bool isInvincible=false;
    
    //記錄獲得分數
    int score=0;
    bool touchScoreFirst=true;
    
    void showScore();
    
    //記錄是否碰撞到陷阱
    bool touchHazardFirst=true;
    
    //產生飛行icon, 吃到即可飛行
    Items *item;
    // void RenewFly( float dt);
    void removefly(float dt);
    bool touchFlyFirst=false;
    float fly_time;
    
    // laser
    // Add laser man
    void laserInit();
    void addLaserman( float dt);
    void addLaser( float dt);
    void removeLaser( float dt);
    void collisionWithLaser(Player* player);
    cocos2d::Sprite* _laserman;
    cocos2d::Sprite* _laser;
    cocos2d::Sprite* _laserThin;
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    bool touchLaserFirst=false;
    
    // hint
    int hint_count;
    bool touchHintPrev;
    cocos2d::MenuItemSprite *hintItem;
    cocos2d::Menu *hintMenu;
    void GoToGameScene( cocos2d::Ref *sender);
    
    //animate
    cocos2d::Animate *animate1;
    cocos2d::Action *action1;

    
    CREATE_FUNC(GameLayer);
protected:
    //场景地图
    cocos2d::TMXTiledMap *_map;
    //障碍物层
     cocos2d::TMXLayer *_walls;
    //陷阱层
    cocos2d::TMXLayer* _hazards;
    //金币层
    cocos2d::TMXLayer* _coins;
    
    cocos2d::TMXLayer* _fly;
    cocos2d::TMXLayer* _gate;
    
    //提示層
    cocos2d::TMXLayer* _hint;
    
    //瓦片集合
    cocos2d::ValueMap tiles[15];
    //背景移动速度
    float  GROUND_SPEED;
    float _bgMoveSpeed;
    //手机屏幕尺寸
    cocos2d::Size size;
    bool pauseflag;
    
    cocos2d::LabelTTF *label = NULL;
};

#endif // __HELLOWORLD_SCENE_H__
