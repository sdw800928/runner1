#include "Ground.h"
#include "Definitions.h"
#include <iostream>

USING_NS_CC;

Ground::Ground() {
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
}

void Ground::createGround( cocos2d::Layer *layer )
{
    ground1 = Sprite::create( "pic/ground.png" );
    ground2 = Sprite::create( "pic/ground.png" );
    ground3 = Sprite::create( "pic/ground.png" );

    // create ground body
    auto groundBody1 = PhysicsBody::createBox( ground1->getContentSize());
    auto groundBody2 = PhysicsBody::createBox( ground2->getContentSize());
    auto groundBody3 = PhysicsBody::createBox( ground3->getContentSize());
    
    // fix the ground
    groundBody1->setDynamic(false);
    groundBody2->setDynamic(false);
    groundBody3->setDynamic(false);
    
    // set collision
    groundBody1->setCollisionBitmask(GROUND_COLLISION_BITMASK);
    groundBody1->setContactTestBitmask(true);
    
    groundBody2->setCollisionBitmask(GROUND_COLLISION_BITMASK);
    groundBody2->setContactTestBitmask(true);
    
    groundBody3->setCollisionBitmask(GROUND_COLLISION_BITMASK);
    groundBody3->setContactTestBitmask(true);
    
    // provide physic body for ground
    ground1->setPhysicsBody( groundBody1);
    ground2->setPhysicsBody( groundBody2);
    ground3->setPhysicsBody( groundBody3);
    
    // set position
    ground1->setPosition( Point( origin.x + ground1->getContentSize().width * 0.5 , origin.y+10 ) );
    ground2->setPosition( Point( origin.x + ground1->getContentSize().width * 1.5 , origin.y+10 ) );
    ground3->setPosition( Point( origin.x + ground1->getContentSize().width * 2.5 , origin.y+10 ) );
    
    // show ground
    layer->addChild(ground1);
    layer->addChild(ground2);
    layer->addChild(ground3);
}


void  Ground::SpawnGround( cocos2d::Layer *layer )
{
    
    float gspeed = GROUND_SPEED;
    if (speedup == false)
        gspeed = GROUND_SPEED;
    else
        gspeed = GROUND_SPEED * 3;
    
    if (ground1->getPositionX() <= origin.x - ground1->getContentSize().width * 0.5)
        ground1->setPosition(Point(ground1->getContentSize().width * 2.5 - gspeed, origin.y + 10));
    else
        ground1->setPosition(Point(ground1->getPositionX() - gspeed, origin.y + 10));
    
    if (ground2->getPositionX() <= origin.x - ground1->getContentSize().width * 0.5)
        ground2->setPosition(Point(ground2->getContentSize().width * 2.5 - gspeed, origin.y + 10));
    else
        ground2->setPosition(Point(ground2->getPositionX() - gspeed, origin.y + 10));
    
    if (ground3->getPositionX() <= origin.x - ground1->getContentSize().width * 0.5)
        ground3->setPosition(Point(ground3->getContentSize().width * 2.5 - gspeed, origin.y + 10));
    else
        ground3->setPosition(Point(ground3->getPositionX() - gspeed, origin.y + 10));
    
}




