#include "Obstacle.h"
#include "Definitions.h"

USING_NS_CC;

Obstacle::Obstacle() {
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
}


void  Obstacle::SpawnPipe( cocos2d::Layer *layer )
{
    // build pipe barrier based on the "pipe.png", create the body
    auto Pipe = Sprite::create( "pic/Pipe.png" );
    
    auto PipeBody = PhysicsBody::createBox( Pipe->getContentSize());
     PipeBody->setDynamic(false);
    // PipeBody->setGravityEnable(false);
    PipeBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
    PipeBody->setContactTestBitmask(true);
    //PipeBody->getShape(0)->setMass(10000);
    //PipeBody->getShape(0)->setDensity(100000);
    PipeBody->getShape(0)->setRestitution(0.0f);
    Pipe->setPhysicsBody( PipeBody);
    
    

    // random = 0.35 ~ 0.65, in order to random create the position of y
    auto random = CCRANDOM_0_1();
    
    if ( random < LOWER_SCREEN_PIPE_THRESHOLD ) {
        random = LOWER_SCREEN_PIPE_THRESHOLD;
    }
    else if ( random > UPPER_SCREEN_PIPE_THRESHOLD ) {
        random = UPPER_SCREEN_PIPE_THRESHOLD;
    }
    // auto topPipePosition = ( random * visibleSize.height ) + ( Pipe->getContentSize().height / 2 );

    Pipe->setPosition( Point( visibleSize.width + Pipe->getContentSize().width/2 + origin.x, -random * Pipe->getContentSize().height ) );
    
    layer->addChild( Pipe);
    // make pipe move
    auto PipeAction = MoveBy::create( PIPE_MOVEMENT_SPEED * visibleSize.width, Point( -visibleSize.width * 1.5, 0 ) );
    Pipe->runAction( PipeAction);
    //Pipe->getPhysicsBody()->setVelocity(Vect(-200,0));
}