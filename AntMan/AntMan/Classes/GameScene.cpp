#include "GameScene.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "Definitions.h"
#include "GameOverScene.h"

USING_NS_CC;

//******************** createScene
Scene* GameScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL);
    
    // create gravity
    scene->getPhysicsWorld()->setGravity( Vec2(0, ANTMAN_FALLING_ACC));
    
    // set physicsWorld
    auto layer = GameScene::create();
    layer->SetPhysicsWorld( scene -> getPhysicsWorld());

    scene->addChild(layer);
    return scene;
}

//******************* initial
bool GameScene::init()
{    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    initialGame();
    
    //set background
    auto backgroundSprite = Sprite::create("pic/Background.png");
    backgroundSprite->setPosition( Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(backgroundSprite);

    //set edgeBody
    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    edgeBody->setCollisionBitmask(WORLD_COLLISION_BITMASK);
    edgeBody->setContactTestBitmask(true);
    
    // set left edgeBody
    auto leftEdgeBox = PhysicsBody::createBox(Size(1, visibleSize.height));
    leftEdgeBox->setCollisionBitmask(DIE_COLLISION_BITMASK);
    leftEdgeBox->setContactTestBitmask(true);
    leftEdgeBox->setDynamic(false);
    
    // set down edgeBody
    auto downEdgeBox = PhysicsBody::createBox(Size(visibleSize.width, 1));
    downEdgeBox->setCollisionBitmask(DIE_COLLISION_BITMASK);
    downEdgeBox->setContactTestBitmask(true);
    downEdgeBox->setDynamic(false);
    
    //set edgeNode
    auto edgeNode = Node::create();
    edgeNode->setPosition( Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y) );
    edgeNode->setPhysicsBody(edgeBody);
    this->addChild(edgeNode);
    
    //set left edgeNode
    auto leftedgeNode = Node::create();
    leftedgeNode->setPosition( Point( 1+ origin.x, visibleSize.height/2 + origin.y) );
    leftedgeNode->setPhysicsBody(leftEdgeBox);
    this->addChild(leftedgeNode);
    
    //set down edgeNode
    auto downedgeNode = Node::create();
    downedgeNode->setPosition( Point(visibleSize.width/2 + origin.x, 1 + origin.y) );
    downedgeNode->setPhysicsBody(downEdgeBox);
    this->addChild(downedgeNode);
    
    //set ground
    ground.createGround(this);
    this->schedule( schedule_selector( GameScene::SpawnGround ));

    //set gameButton
    cocos2d::ui::Button *btn = cocos2d::ui::Button::create( "pic/Play Button.png", "pic/Play Button Clicked.png" );
    btn->setPosition(Vec2(100,100));
    btn->addTouchEventListener(CC_CALLBACK_2(GameScene::buttonEvent, this));
    this->addChild(btn);
 
    //set spawn schedule
    this->schedule( schedule_selector( GameScene::SpawnObstacle ), PIPE_SPAWN_FREQUENCY*visibleSize.width );
    this->schedule( schedule_selector( GameScene::SpawnCoin ), SCORE_SPAWN_FREQUENCY*visibleSize.width );
    this->schedule( schedule_selector( GameScene::SpawnFly ), FLY_SPAWN_FREQUENCY*visibleSize.width );

    // create antMan
    antMan = new AntMan(this);
    
    // create items
    items = new Items(this);

    // set up collision listener
    auto contactListener = EventListenerPhysicsContact::create( );
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    
    // set up touch listener
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches( true );
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegin, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority( touchListener, this);

    
     this->schedule(schedule_selector(GameScene::speedupfunction), 1.0/60.0f);
    
    return true;
}

//********************Initial game

void GameScene::initialGame()
{
    score = 0;
    jump = 1;
    speed = 3.0f;
    speedup = false;
    invincible = false;
    invincible_time = 0.0f;
    tapCount = 0;
    onGround = false;
    fly_time = 0.0f;
}

//update happens in every dt
void GameScene:: speedupfunction( float dt )
{
    //SpeedUp
    if ( speedup ) {
        invincible_time = invincible_time - 0.01f;
    }
    //CCLOG("s: %f", invincible_time);
    //Back to Normal
    if ( invincible_time <= 0 ) {
        CCDirector::sharedDirector()->getScheduler()->setTimeScale(1);
        speedup = false;
        invincible_time = 0.0f;
        invincible = false;
        speed = 3.0f;
        ground.speedup = false;
        //score = 0;
    }
    
    if ( antMan->inwater ) {
        fly_time -= 0.01f;
    }
    if ( fly_time <= 0 ) {
        antMan->inwater = false;
        fly_time = 0.0f;
        tapCount = 0;
    }
}

//********************Spawn Items

void GameScene::SpawnGround(float dt)
{
    ground.SpawnGround( this );
}

void GameScene::SpawnCoin(float dt)
{
    items->SpawnCoin( this );
}

void GameScene::SpawnFly(float dt)
{
    items->SpawnFly( this );
}

void GameScene::SpawnObstacle(float dt)
{
    // you guys try to implement with Obstacle.s and obstacle.h
    obstacle.SpawnPipe(this);
}

//********************Set up listeners

// checkCollision
bool GameScene::collideWith(cocos2d::PhysicsBody &a, cocos2d::PhysicsBody &b, int mask1, int mask2)
{
    return  (mask1 == a.getCollisionBitmask() && mask2 == b.getCollisionBitmask())
        || (mask1 == b.getCollisionBitmask() && mask2 == a.getCollisionBitmask());
    
}

// getItem from two collision bodys, which don't have the mask collision mask
Sprite* GameScene::getItemBody(cocos2d::PhysicsBody &a, cocos2d::PhysicsBody &b,
                               int mask, cocos2d::PhysicsContact &contact)
{
    return (mask == a.getCollisionBitmask()) ?
           (Sprite*)contact.getShapeB()->getBody()->getNode():
           (Sprite*)contact.getShapeA()->getBody()->getNode();
}

//delte item
void GameScene::deleteItem(cocos2d::Sprite &item)
{
    item.setVisible(false);
    item.removeFromParent();
}

/*
bool GameScene::onContactSeperate( cocos2d::PhysicsContact &contact ){
    
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();
    
    auto body_1 = (Sprite*)contact.getShapeA()->getBody()->getNode();
    auto body_2 = (Sprite*)contact.getShapeB()->getBody()->getNode();
    
    if(collideWith(*a, *b, ANTMAN_COLLISION_BITMASK, OBSTACLE_COLLISION_BITMASK))
    {
        mycount++;
        CCLOG("1 %i",mycount);
        if ( invincible == false ) {
            // auto scene = GameOverScene::createScene();
            // Director::getInstance()->replaceScene(TransitionFade::create( TRANSITION_TIME, scene ));
            
            Sprite *collision_item = getItemBody(*a, *b, OBSTACLE_COLLISION_BITMASK, contact);
            float ant_x = collision_item->getPhysicsBody()->getVelocity().y;
            //CCLOG("1 %f",ant_x);
            collision_item->getPhysicsBody()->setVelocity(Vect(0,ant_x));
            // ant_x = collision_item->getPositionX();
            // CCLOG("1 %f",ant_x);
            
            
            return false;
        }
        else {
            
            body_2->setVisible(false);
            body_2->removeFromParent();
            
            return false;
        }
    }
}
*/
// handle with collision
bool GameScene::onContactBegin( cocos2d::PhysicsContact &contact )
{
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();
    
    auto body_1 = (Sprite*)contact.getShapeA()->getBody()->getNode();
    auto body_2 = (Sprite*)contact.getShapeB()->getBody()->getNode();

/*
    if(collideWith(*a, *b, OBSTACLE_COLLISION_BITMASK, GROUND_COLLISION_BITMASK))
    {
        return true;
    }
*/
    
    // AntMan Hit leftEdge or downEdge, then die
    if(collideWith(*a, *b, ANTMAN_COLLISION_BITMASK, DIE_COLLISION_BITMASK)) {
        auto scene = GameOverScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create( TRANSITION_TIME, scene ));
        return true;

    }
    
    // AntMan Hit OBSTACLE
    if(collideWith(*a, *b, ANTMAN_COLLISION_BITMASK, OBSTACLE_COLLISION_BITMASK))
    {
        if ( invincible == false ) {
            // auto scene = GameOverScene::createScene();
            // Director::getInstance()->replaceScene(TransitionFade::create( TRANSITION_TIME, scene ));
            
            Sprite *collision_item = getItemBody(*a, *b, OBSTACLE_COLLISION_BITMASK, contact);
            float ant_x = collision_item->getPositionX();
            //float ant_y = collision_item->getPhysicsBody()->getVelocity().y;
            ////CCLOG("1 %f",ant_x);
            collision_item->setPositionX(ant_x-10);
            // ant_x = collision_item->getPositionX();
            // CCLOG("1 %f",ant_x);
            
            
            return true;
        }
        else {
            
            body_2->setVisible(false);
            body_2->removeFromParent();
            
            return false;
        }
    }

    // AntMan Hit coins
    if(collideWith(*a, *b, ANTMAN_COLLISION_BITMASK, POINT_COLLISION_BITMASK)){
        Sprite *collision_item = getItemBody(*a, *b, ANTMAN_COLLISION_BITMASK, contact);
        deleteItem(*collision_item);
        
        if ( !speedup ) {
            score++;
            items->ShowNode(this, score);
        }
        //AntMan speedUp
        if ( !speedup && invincible_time == 0.0f && score % 3==0 ) {
            
            speedup = true;
            CCScheduler* pScheduler = CCDirector::sharedDirector()->getScheduler();
            pScheduler->setTimeScale(speed);
            invincible = true;
            invincible_time = 9.0f;
            ground.speedup = true;
        }
        
        
    }
    
    // AntMan Hit Ground, reset antManBody
    if(collideWith(*a, *b, ANTMAN_COLLISION_BITMASK, GROUND_COLLISION_BITMASK))
    {
        jump = 0;
        onGround = true;
        
        //ANT man hit ground, change speed to 0,0;
        PhysicsBody *antManBody = a->getCollisionBitmask() == ANTMAN_COLLISION_BITMASK? a: b;
        antManBody->setVelocity({20,0});
        return true;
    }
    
    // small AntMan Hit Fly, then fly
    if(collideWith(*a, *b, ANTMAN_COLLISION_BITMASK, FLY_COLLISION_BITMASK))
    {
        Sprite *collision_item = getItemBody(*a, *b, ANTMAN_COLLISION_BITMASK, contact);
        deleteItem(*collision_item);
        
        if ( antMan->big == false ) {
            antMan->inwater = true;
            fly_time = 5.0f;
        }
    }
    
    // Coin Hit EdgeBox, disappear
    if(collideWith(*a, *b, WORLD_COLLISION_BITMASK, POINT_COLLISION_BITMASK))
    {
        Sprite *collision_item = getItemBody(*a, *b, WORLD_COLLISION_BITMASK, contact);
        deleteItem(*collision_item);
    }
    
    // Fly Hit EdgeBox, disappear
    if(collideWith(*a, *b, WORLD_COLLISION_BITMASK, FLY_COLLISION_BITMASK))
    {
        Sprite *collision_item = getItemBody(*a, *b, WORLD_COLLISION_BITMASK, contact);
        deleteItem(*collision_item);
    }
    
    // Obstacle Hit EdgeBox, disappear
    if(collideWith(*a, *b, WORLD_COLLISION_BITMASK, OBSTACLE_COLLISION_BITMASK))
    {
        Sprite *collision_item = getItemBody(*a, *b, WORLD_COLLISION_BITMASK, contact);
        deleteItem(*collision_item);
    }
    
    return false;
}

// touchScreen Listener
bool GameScene::onTouchBegin( cocos2d::Touch *touch, cocos2d::Event *event )
{
    ++tapCount;
    if ( antMan->inwater == false ) {
        //if singleTap jump
        if (tapCount == 1) {
            this->schedule(schedule_selector(GameScene::singleTap), 0.15, 1, 0);
        }
    
        //if double jump rushing down, if ant-man onGround do nothing.
        else {
            if(onGround) return true;
            this->unschedule(schedule_selector(GameScene::singleTap));
            tapCount = 0;
            antMan->RushDown();
        }
    }
    else {
        antMan->Jump( );
    }
    return true;
}

//check if singleTap ant-man will jump
void GameScene::singleTap(float deltaTime)
{
    this->unschedule(schedule_selector(GameScene::singleTap));
    tapCount = 0;
    if(++jump <= 2) antMan->Jump( );
    onGround = false;
    
}

//  touchButton Listener
void GameScene::buttonEvent( cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type )
{
    if ( type == ui::Widget::TouchEventType::BEGAN ) {
        antMan->Shrink_Magnify( this );
    }
}




