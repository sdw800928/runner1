#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "AntMan.h"
#include "Ground.h"
#include "Items.h"
#include "Obstacle.h"
#include <ui/CocosGUI.h>

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameScene);
    
private:
    
    // *****************Functions
    
    // Initial the world
    cocos2d::PhysicsWorld *sceneWorld;
    void initialGame();
    void SetPhysicsWorld( cocos2d::PhysicsWorld *world ) { sceneWorld = world; };
    void speedupfunction( float dt );
    
    // Spwan items
    void SpawnObstacle( float dt);
    void SpawnGround( float dt);
    void SpawnCoin( float dt);
    void SpawnFly( float dt);
    
    //********listeners
    
    //collide
    bool collideWith(cocos2d::PhysicsBody &a, cocos2d::PhysicsBody &b, int mask1, int mask2);
    cocos2d::Sprite* getItemBody(cocos2d::PhysicsBody &a, cocos2d::PhysicsBody &b,
                                 int mask, cocos2d::PhysicsContact &contact);
    void deleteItem(cocos2d::Sprite &item);
    bool onContactBegin( cocos2d::PhysicsContact &contact );
    bool onContactSeperate( cocos2d::PhysicsContact &contact );
    
    //onTouch
    bool onTouchBegin( cocos2d::Touch *touch, cocos2d::Event *event );
    void singleTap(float deltaTime);
    
    // Set up button
    void buttonEvent( cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type );

    //*****************Variables
    
    //*****Game parts
    AntMan *antMan;
    Ground ground;
    Items *items;
    Obstacle obstacle;
    //*****Game attributes
    int jump;
    int score;
    int tapCount;
    bool speedup;
    bool invincible;
    bool onGround;
    float speed;
    float invincible_time;
    float fly_time;
    

    
};

#endif
