#include "AntMan.h"
#include "Definitions.h"


USING_NS_CC;

AntMan::AntMan(cocos2d::Layer *layer ) {
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    // set pic and postion
    antMan = Sprite::create( "pic/Ball.png" );
    antMan->setPosition( Point( visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y ) );
    
    // set physics body
    auto antManBody = PhysicsBody::createCircle( antMan->getContentSize().width/2 );
    antMan->setPhysicsBody(antManBody);
    
    // set collision
    antManBody->setCollisionBitmask(ANTMAN_COLLISION_BITMASK);
    antManBody->setContactTestBitmask(true);

    antManBody->getShape(0)->setMass(0);
    antManBody->getShape(0)->setRestitution(0.0f);
    layer->addChild(antMan);

/*
    // set pic and postion
    antMan2 = Sprite::create( "pic/Ball_bigger.png" );
    antMan2->setPosition( Point( visibleSize.width/4 + origin.x, 100 + origin.y ) );
    
    // set physics body
    auto antManBody2 = PhysicsBody::createCircle( antMan2->getContentSize().width/2 );
    antMan2->setPhysicsBody(antManBody2);
    
    // set collision
    antManBody2->setCollisionBitmask(ANTMAN_COLLISION_BITMASK);
    antManBody2->setContactTestBitmask(true);
    
    layer->addChild(antMan2);
*/
}

//Jump
void AntMan::Jump() {
    
    Vect force;
    
    if ( inwater && !big )
        force = Vect(0, ANTMAN_FLY_WITH_FLY_SPEED);
    else
        force = Vect(0, ANTMAN_FLYING_SPEED);
    
    antMan->getPhysicsBody()->setVelocity(force);
    // antMan->setPosition(100, 100);
}

void AntMan::RushDown() {
    Vect force = Vect(0, -ANTMAN_RUSHING_SPEED);
    antMan->getPhysicsBody()->setVelocity(force);
}
//AntMan transform
void AntMan::Shrink_Magnify( cocos2d::Layer *layer )
{
    float x = antMan->getPositionX();
    float y = antMan->getPositionY();
    
    Vect previousV = antMan->getPhysicsBody()->getVelocity();
    antMan->removeFromParent();
    
    // transform 
    if ( big == true ) {
        antMan = Sprite::create( "pic/Ball.png" );
        big = false;
        antMan->setPosition(Point(x, y));
    }
    else {
        antMan = Sprite::create( "pic/Ball_bigger.png" );
        big = true;
        antMan->setPosition(Point(x, y+ antMan->getContentSize().height/2));
    }
    
    // set physic body is just radius of the "Ball.png", not the whole ball size
    auto antManBody = PhysicsBody::createCircle( antMan->getContentSize().width/2 );
    
    // set collision
    antManBody->setCollisionBitmask(ANTMAN_COLLISION_BITMASK);
    antManBody->setContactTestBitmask(true);
    antManBody->getShape(0)->setRestitution(0);
    antManBody->getShape(0)->setMass(0);
    antManBody->setVelocity(previousV);
    antMan->setPhysicsBody( antManBody);
    
    layer->addChild( antMan, 100);
}

