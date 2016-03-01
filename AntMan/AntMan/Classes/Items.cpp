#include "Items.h"
#include "Definitions.h"


USING_NS_CC;

Items::Items(cocos2d::Layer *layer) {
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    EnergyNode1->setPosition( Point(1*30 + origin.x, visibleSize.height-30 + origin.y));
    EnergyNode1->setVisible(false);
    layer->addChild(EnergyNode1);
    
    EnergyNode2->setPosition( Point(2*30 + origin.x, visibleSize.height-30 + origin.y));
    EnergyNode2->setVisible(false);
    layer->addChild(EnergyNode2);
    
    EnergyBar->setPosition( Point(5*30 + origin.x, visibleSize.height-30 + origin.y));
    EnergyBar->setVisible(false);
    layer->addChild(EnergyBar);
}

void  Items::SpawnCoin( cocos2d::Layer *layer )
{

    // crate coin body
    Sprite *pointNode = Sprite::create( "pic/Ball.png" );
    auto pointBody = PhysicsBody::createCircle( pointNode->getContentSize().width/2);
    pointBody->setDynamic(false);
    pointBody->setCollisionBitmask( POINT_COLLISION_BITMASK);
    pointBody->setContactTestBitmask(true);
    pointNode->setPhysicsBody(pointBody);
    
    // random = 0.35 ~ 0.65, in order to random create the position of y
    auto random = CCRANDOM_0_1();
    if ( random < LOWER_SCREEN_PIPE_THRESHOLD ) {
        random = LOWER_SCREEN_PIPE_THRESHOLD;
    }
    else if ( random > UPPER_SCREEN_PIPE_THRESHOLD ) {
        random = UPPER_SCREEN_PIPE_THRESHOLD;
    }
    pointNode->setPosition( Point( visibleSize.width + pointNode->getContentSize().width + origin.x, ( random * visibleSize.height )) );
    layer->addChild(pointNode);
    
    // make coin move
    auto pointNodeAction = MoveBy::create( SCORE_MOVEMENT_SPEED * visibleSize.width, Point( -visibleSize.width * 1.5, 0 ) );
    pointNode->runAction(pointNodeAction);
    
}

void Items::ShowNode(cocos2d::Layer *layer, int nodeNum) {
    
    if ( nodeNum%3==1 ) {
        EnergyNode1->setVisible(true);
    }
    else if ( nodeNum%3==2 ) {
        EnergyNode2->setVisible(true);
    }
    else {
        EnergyNode1->setVisible(false);
        EnergyNode2->setVisible(false);
        EnergyBar->setVisible(true);
        EnergyBar->setScale(100,1);
        
        auto scaleTo = ScaleTo::create(15.0f, 0.0, 1.0f);
        EnergyBar->runAction(scaleTo);
    }
}

void Items::SpawnFly(cocos2d::Layer *layer) {
    
    // crate fly body
    Sprite *pointNode = Sprite::create( "pic/Ball_bigger.png" );
    auto pointBody = PhysicsBody::createCircle( pointNode->getContentSize().width/2);
    pointBody->setDynamic(false);
    pointBody->setCollisionBitmask( FLY_COLLISION_BITMASK);
    pointBody->setContactTestBitmask(true);
    pointNode->setPhysicsBody(pointBody);
    
    // random = 0.35 ~ 0.65, in order to random create the position of y
    auto random = CCRANDOM_0_1();
    if ( random < LOWER_SCREEN_PIPE_THRESHOLD ) {
        random = LOWER_SCREEN_PIPE_THRESHOLD;
    }
    else if ( random > UPPER_SCREEN_PIPE_THRESHOLD ) {
        random = UPPER_SCREEN_PIPE_THRESHOLD;
    }
    pointNode->setPosition( Point( visibleSize.width + pointNode->getContentSize().width + origin.x, ( random * visibleSize.height )) );
    layer->addChild(pointNode);
    
    // make coin move
    auto pointNodeAction = MoveBy::create( SCORE_MOVEMENT_SPEED * visibleSize.width, Point( -visibleSize.width * 1.5, 0 ) );
    pointNode->runAction(pointNodeAction);
}





