#include "GameLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameOverScene.h"
#include "Definitions.h"
#include "SimpleAudioEngine.h"
#include "GameTransferScene.h"


USING_NS_CC;
using namespace CocosDenshion;

using namespace cocostudio::timeline;

unsigned int global_score;
int map_index;
unsigned int global_energy;

Scene* GameLayer::createScene(int init_score, int scene_index, int init_energy)
{
    global_score = init_score;
    map_index = scene_index;
    global_energy = init_energy;
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
        if ( !Layer::init() )
    {
        return false;
    }
    

//    //加载一个蓝色背景当装饰
//    LayerColor *blueSky = LayerColor::create(Color4B(100, 100, 250, 255));
//    this->addChild(blueSky);
    
    //加载地图
    CCLOG("%i\n", map_index);
    
    if ( map_index == -1 ) {
        _map = TMXTiledMap::create("tutorial/tutorial.tmx");
    }
    else {
        switch( map_index % 5 ) {
            case 0:
                 _map =TMXTiledMap::create("update/level3/level3.tmx");
                break;
            case 1:
                _map =TMXTiledMap::create("update/level3.1/level3.1.tmx");
                break;
            case 2:
                _map =TMXTiledMap::create("update/level1/level1.tmx");
                break;
//            case 0:
//                _map = TMXTiledMap::create("level3/level3.tmx");  // 3
//                break;
//            case 1:
//                _map = TMXTiledMap::create("level11/level1.tmx");  // 1
//                break;
            case 3:
                _map = TMXTiledMap::create("level2/level2.tmx");  // 2
                break;
            case 4:
                _map = TMXTiledMap::create("level4/level4.tmx");  // 4
                break;
            default:
                break;
        }
        
    }
    _map->setAnchorPoint(Point::ZERO);
    this->addChild(_map);
    
    //加载背景音乐
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->playBackgroundMusic("music/background.mp3", true);
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
    
    // player 初始化
    // SpriteFrameCache::getInstance()->addSpriteFramesWithFile( "Antman.plist" );
    // _player = Sprite::createWithSpriteFrameName( "antman1.png" );
    
    size = Director::getInstance()->getWinSize();
    _player =  Player::create("antman1.png");
    _player->setAnchorPoint(Point::ZERO);
    _player->setPosition( Point(100,150) );
    
    animate1 = Animate::create(Player::get_role_animation());
    action1 = RepeatForever::create(animate1);
 
    _player->runAction(action1);
    _map->addChild(_player, 15);
    
    //游戏按钮初始化
    // MenuItemImage* jump = MenuItemImage::create( "start_n.png", "start_s.png" ,CC_CALLBACK_1(GameLayer::jump_callback, this));
    MenuItemImage* speedup = MenuItemImage::create( "sprintbefore.png", "sprintpress.png" ,CC_CALLBACK_1(GameLayer::speed_up_callback, this));
    speedup->setPosition( size.width * 0.9f, size.height * 0.15f );
    speedup->setScale( 0.5f );
    
    MenuItemImage* small = MenuItemImage::create( "sizebefore.png", "sizepress.png",CC_CALLBACK_1(GameLayer::Shrink_Magnify, this));
    small->setPosition( size.width * 0.1f, size.height * 0.15f );
    small->setScale( 0.5f );
    
    
    MenuItemImage* pause = MenuItemImage::create( "sizebefore.png", "sizepress.png",CC_CALLBACK_1(GameLayer::Pause, this));
    pause->setPosition( size.width * 0.9f, size.height * 0.9f );
    pause->setScale( 0.5f );
    
    
    Menu* m = Menu::create( speedup ,small, pause, nullptr );
    m->setPosition(Point::ZERO );
    addChild( m , 10 );
    
    //创建一个触摸监听器，这里使用单点触摸事件
    auto touchListener = EventListenerTouchOneByOne::create();
    //设置吞噬为true，不让触摸往下传递
    touchListener->setSwallowTouches( true );
    //和回调函数绑定
    touchListener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegin, this);
    //添加监听器到事件分发器中
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority( touchListener, this);

    //地图参数初始化
    _walls = _map->getLayer("floor");
    _hazards=_map->getLayer("fire");
    _coins=_map->getLayer("gold");
    _fly=_map->getLayer("fly");
    _gate=_map->getLayer("gate");
    _hint=_map->getLayer("hint");

    //飛行icon初始化
    item = new Items(this);
    // item->SpawnFly(this);
    // this->schedule( schedule_selector( GameLayer::RenewFly ), (1.0/60.0) );
    
    //參數初始化
    if ( map_index < 0 ) {
        GROUND_SPEED = GROUND_SPEED_INI;
    }
    else
        GROUND_SPEED = GROUND_SPEED_INI+map_index/2.0f;
    _bgMoveSpeed = GROUND_SPEED;
    score=global_score;
    isSpeeding=false;
    isInvincible=false;
    touchHazardFirst=true;
    touchFlyFirst=false;
    _player->_big=true;
    
    laserInit();
    touchLaserFirst=false;
    fly_time=0.0f;
    item->energy=global_energy;
    item->showEnergy(this);
    hint_count= 0;
    touchHintPrev = false;
    pauseflag=false;
    
    showScore();
    
    // Initial duration bar
    EnergyBar->setPosition( Point(5*30 , visibleSize.height-30));
    EnergyBar->setVisible(false);
    this->addChild(EnergyBar);
    
    //绑定更新函数
    this->scheduleUpdate();

    return true;
}

// speedup callback
void GameLayer::speed_up_callback( Ref* ref ) {
    
    if ( !pauseflag ) {
        // if ( isSpeeding )return;
        if ( item->energy >= 5 ) {
            
            // _player->stopAction(action1);
            _player->stopAllActions();
            animate1 = Animate::create(Player::get_role_animation_Sprint());
            action1 = RepeatForever::create(animate1);
            _player->runAction(action1);
            
            
            isSpeeding = true;
            isInvincible = true;
            
            _bgMoveSpeed = GROUND_SPEED*3;
        
            item->energy=0;
            item->showEnergy(this);
        
            this->scheduleOnce( schedule_selector( GameLayer::stop_speed_up_callback), 1.5f );
            this->scheduleOnce( schedule_selector( GameLayer::stop_invincible_callback), 5.5f );
            
            _player->setPositionY(_player->getPositionY()+100);
            
            EnergyBar->setVisible(true);
            EnergyBar->setScale(100,1);
        
            auto scaleTo = ScaleTo::create(1.5f, 0.0, 1.0f);
            EnergyBar->runAction(scaleTo);
            
        }
    }
    /*
    if ( coin->energy >= 5 ) {
        coin->speedup = true;
        CCLOG("%i\n",coin->energy);
        
        coin->energy = 0;
        coin->showEnergy(this);
        
        isSpeeding = true;
        _bgMoveSpeed = GROUND_SPEED*3;
        
        this->scheduleOnce( schedule_selector( HelloWorld::start_speed_up_callback), 4.0f );
        
        EnergyBar->setVisible(true);
        EnergyBar->setScale(100,1);
        
        auto scaleTo = ScaleTo::create(4.0f, 0.0, 1.0f);
        EnergyBar->runAction(scaleTo);
    }
    */
}

void GameLayer::stop_speed_up_callback(float dt) {
    
    if(!pauseflag)
        _bgMoveSpeed = GROUND_SPEED;
    isSpeeding=false;
    
    EnergyBar->setVisible(false);
    
    // _player->stopAction(action2);
    _player->stopAllActions();
    animate1 = Animate::create(Player::get_role_animation());
    action1 = RepeatForever::create(animate1);
    _player->runAction(action1);
    // _map->addChild(_player, 15);
}

void GameLayer::stop_invincible_callback( float dt ) {
    
    isInvincible=false;
}


/*
//跳跃回调函数
void GameLayer::jump_callback( Ref* ref ){
    
    //每次触发跳跃 跳跃数增加 右碰撞重置
    _player->_jump++;
    _player->_jumpable=true;
    right=false;

}
*/

void GameLayer::update(float dt)
{
    _player->update(dt, item->isflying, pauseflag, isSpeeding, _bgMoveSpeed);  //更新player期望位置
    
    if ( isInvincible == false )
        this->handleHazardCollisions(_player);  //陷阱处理
    
    if ( isSpeeding == false )
        this->checkForAndResolveCollisions(_player);//碰撞检测和位置修复
    
    this->handleCoinCollisions(_player);//金币处理
    
    this->FlyCollisions(_player);   //飛行物件處理
    
    this->handleGateCollisions(_player);
    
    this->handleHintCollisions(_player);
    
    //if ( touchFlyFirst == false ) {
    //    this->handleFlyCollisions(_player); //飛行物件處理
    //}
    
    if ( touchLaserFirst == false)
        collisionWithLaser(_player);
    
    _player->setPosition(_player->_desiredPosition); //更新player位置

    auto worldSpace=_map->convertToWorldSpace(_player->getPosition()); //player掉到屏幕外判定
    if(worldSpace.x <= -_player->getContentSize().width){
        if ( touchHazardFirst == true ) {
            touchHazardFirst=false;
            // sleep(0.5);
            auto scene = GameOverScene::createScene(score, map_index);
            Director::getInstance()->replaceScene(TransitionFade::create( TRANSITION_TIME, scene ));
            // touchHazardFirst=false;
        }
    }
    _map->setPositionX( _map->getPositionX() - _bgMoveSpeed );
    
    if ( fly_time > 0.0f ) {
        fly_time -= 1.0f/60.0f;
        if ( fly_time <= 0.0f ) {
            removefly(dt);
            fly_time=0.0f;
        }
    }
    
    
    
}

//工具函数
//根据position返回对应的瓦片信息
Point GameLayer::tileCoordForPosition(cocos2d::Point position)
{
    float x = floor(position.x / _map->getTileSize().width); //位置x值/地图一块tile的宽度即可得到x坐标
    float levelHeightInPixels = _map->getMapSize().height * _map->getTileSize().height; //地图的实际高度
    float y = floor((levelHeightInPixels - position.y)/_map->getTileSize().height);  //地图的原点在左上角，与cocos2d-x是不同的(2dx原点在左下角)
    return Point(x, y);
}

//根据瓦片位置生成相应瓦片的碰撞盒
Rect GameLayer::tileRectFromTileCoords(cocos2d::Point tileCoords)
{
    float levelHeightInPixels = _map->getMapSize().height * _map->getTileSize().height; //地图的实际高度
    //把地图坐标tileCoords转化为实际游戏中的坐标
    Point origin = Point(tileCoords.x * _map->getTileSize().width, levelHeightInPixels - ((tileCoords.y+1)*_map->getTileSize().height));
    return Rect::Rect(origin.x, origin.y, _map->getTileSize().width, _map->getTileSize().height);
}

//获取相交矩形
Rect GameLayer::intersectsRect( Rect rect1, Rect rect2 ) //计算相交矩形
{
    float xa1 = rect1.getMinX();
    float ya1 = rect1.getMinY();
    
    
    float xa2 = rect1.getMaxX();
    float ya2 = rect1.getMaxY();
    
    
    float xb1 = rect2.getMinX();
    float yb1 = rect2.getMinY();
    
    
    float xb2 = rect2.getMaxX();
    float yb2 = rect2.getMaxY();
    
    
    float xc1,yc1;
    float xc2,yc2;
    
    
    xc1 = std::max(xa1,xb1);
    yc1 = std::max(ya1,yb1);
    
    
    xc2 = std::min(xa2,xb2);
    yc2 = std::min(ya2,yb2);
    
    
    return Rect::Rect(xc1,yc1,xc2-xc1,yc2-yc1);

}

//生成用于检测player碰撞的瓦片集
void GameLayer::getSurroundingTilesAtPosition(Point position, TMXLayer* layer,ValueMap* gids)

{
    Point plPos = this->tileCoordForPosition(position); //1 返回此处的tile坐标
   
    if(_player->_big){
          plPos+=Point(-1,-4);   //取player图形的左上位置
    //存gid的数组

        for (int i=0; i<30; i++)
        {
            int c = i % 5;   //相当于当前i所处的列
            int r = (int)(i/5); //相当于当前i所处的行
    
            Point tilePos = Point(plPos.x + c, plPos.y + r);
            
            
            if (tilePos.y > (_map->getMapSize().height-1))
            {
                if ( touchHazardFirst == true ) {
                    touchHazardFirst=false;
                    // sleep(0.5);
                    auto scene = GameOverScene::createScene(score, map_index);
                    Director::getInstance()->replaceScene(TransitionFade::create( TRANSITION_TIME, scene ));
                    // touchHazardFirst=false;
                }
            }
            
            //4 取出包围tile的gid
            int tgid = layer->getTileGIDAt(tilePos);
            //5
            Rect tileRect = this->tileRectFromTileCoords(tilePos);  //包围盒
            float x = tileRect.origin.x;  //位置
            float y = tileRect.origin.y;
            //取出这个tile的各个属性，放到valuemap里
    
            ValueMap tileDict;
            tileDict["gid"]= tgid;
             tileDict["x"]= x;
             tileDict["y"]= y;
             tileDict["tilePos_x"]= tilePos.x;
             tileDict["tilePos_y"]= tilePos.y;
    
            
            //设置碰撞检测的顺序 下、上、左、右、左上、右上、左下、右下
            switch (i) {
                case 0:
                    gids[4]=tileDict;
                    break;
                case 2:
                     gids[1]=tileDict;
                    break;
                case 4:
                     gids[5]=tileDict;
                    break;
                case 10:
                     gids[2]=tileDict;
                    break;
                case 19:
                     gids[3]=tileDict;
                    break;
                case 25:
                     gids[6]=tileDict;
                    break;
                case 27:
                     gids[0]=tileDict;
                    break;
                case 29:
                     gids[7]=tileDict;
                    break;
                    
                default:
                    break;
            }
        }
    }else{
        plPos+=Point(-1,-2);
      
        
        for (int i=0; i<16; i++)
        {
            int c = i % 4;
            int r = (int)(i/4);
            Point tilePos = Point(plPos.x + c, plPos.y + r);
            
                    if (tilePos.y > (_map->getMapSize().height-1))
                    {
                        if ( touchHazardFirst == true ) {
                            touchHazardFirst=false;
                            // sleep(0.5);
                            auto scene = GameOverScene::createScene(score, map_index);
                            Director::getInstance()->replaceScene(TransitionFade::create( TRANSITION_TIME, scene ));
                            // touchHazardFirst=false;
                        }
                    }
 
            int tgid = layer->getTileGIDAt(tilePos);
            Rect tileRect = this->tileRectFromTileCoords(tilePos);
            float x = tileRect.origin.x;
            float y = tileRect.origin.y;
            
            ValueMap tileDict;
            tileDict["gid"]= tgid;
            tileDict["x"]= x;
            tileDict["y"]= y;
            tileDict["tilePos_x"]= tilePos.x;
            tileDict["tilePos_y"]= tilePos.y;
            
            
            switch (i) {
                case 0:
                    gids[4]=tileDict;
                    break;
                case 1:
                    gids[1]=tileDict;
                    break;
                case 2:
                    gids[5]=tileDict;
                    break;
                case 8:
                    gids[2]=tileDict;
                    break;
                case 11:
                    gids[3]=tileDict;
                    break;
                case 12:
                    gids[6]=tileDict;
                    break;
                case 14:
                    gids[0]=tileDict;
                    break;
                case 15:
                    gids[7]=tileDict;
                    break;
                    
                default:
                    break;
            }
        }

    }
//    //3 我们的目的是想取出环绕在精灵四周的8个tile，这里就从上至下每行三个取9个tile（中间一个不算）仔细画画图就知代码的意义
//    for (int i=0; i<9; i++)
//    {
//        int c = i % 3;   //相当于当前i所处的列
//        int r = (int)(i/3); //相当于当前i所处的行
//        
//        Point tilePos = Point(plPos.x + (c-1), plPos.y + (r-1));
////        if (tilePos.y > (_map->getMapSize().height-1))
////        {
////            sleep(0.5);
////            auto scene = GameOverScene::createScene(0);
////            Director::getInstance()->replaceScene(TransitionFade::create( TRANSITION_TIME, scene ));
////
////        }
//
//        //4 取出包围tile的gid
//        int tgid = layer->getTileGIDAt(tilePos);
//        //5
//        Rect tileRect = this->tileRectFromTileCoords(tilePos);  //包围盒
//        float x = tileRect.origin.x;  //位置
//        float y = tileRect.origin.y;
//        //取出这个tile的各个属性，放到CCDictionary里
//        
//        ValueMap tileDict;
//        tileDict["gid"]= tgid;
//         tileDict["x"]= x;
//         tileDict["y"]= y;
//         tileDict["tilePos_x"]= tilePos.x;
//         tileDict["tilePos_y"]= tilePos.y;
//    
//       
//        switch (i) {
//            case 0:
//                gids[4]=tileDict;
//                break;
//            case 1:
//                 gids[1]=tileDict;
//                break;
//            case 2:
//                 gids[5]=tileDict;
//                break;
//            case 3:
//                 gids[2]=tileDict;
//                break;
//            case 5:
//                 gids[3]=tileDict;
//                break;
//            case 6:
//                 gids[6]=tileDict;
//                break;
//            case 7:
//                 gids[0]=tileDict;
//                break;
//            case 8:
//                 gids[7]=tileDict;
//                break;
//                
//            default:
//                break;
//        }
//    }
   
}

//返回当前层被player覆盖的瓦片集
void GameLayer::getOverlapTilesAtPosition(Point position, TMXLayer* layer,ValueMap* gids)

{
    Point plPos = this->tileCoordForPosition(position); //1 返回此处的tile坐标
    int size,col;
    if(_player->_big){
        plPos+=Point(0,-4);  //取player图形的左上
        size=15;          //设置player图形覆盖瓦片的数量
        col=3;
    }else{
        plPos+=Point(0,-2);
        size=6;
        col=2;

    }
    
        for (int i=0; i<size; i++)
        {
            int c = i % col;
            int r = (int)(i/col);
            
            Point tilePos = Point(plPos.x + c, plPos.y + r);

           
            int tgid = layer->getTileGIDAt(tilePos);
           
            Rect tileRect = this->tileRectFromTileCoords(tilePos);  //包围盒
            float x = tileRect.origin.x;  //位置
            float y = tileRect.origin.y;

            
            ValueMap tileDict;
            tileDict["gid"]= tgid;
            tileDict["x"]= x;
            tileDict["y"]= y;
            tileDict["tilePos_x"]= tilePos.x;
            tileDict["tilePos_y"]= tilePos.y;
            
            gids[i]=tileDict;

        }
}

//玩家与障碍物碰撞的检测和位置修复
void GameLayer::checkForAndResolveCollisions(Player* player)
{
     this->getSurroundingTilesAtPosition(player->getPosition(), _walls,tiles); //1 生成用于检测player碰撞的瓦片集
    
    //this->getOverlapTilesAtPosition(player->getPosition(), _walls, tiles);
    
    player->_onGround = false;


    for(auto i=0;i<8;i++){
        ValueMap dic=tiles[i];
        Rect playerRect = player->collisionBoundingBox();  //2 玩家的包围盒
        int gid = dic.at("gid").asInt();  //3 从ValueMap中取得玩家附近tile的gid值
        if (gid)
        {
            
            float rect_x = dic.at("x").asFloat();
            float rect_y = dic.at("y").asFloat();
            float width = _map->getTileSize().width;
            float height = _map->getTileSize().height;
            //4 取得这个tile的Rect
            Rect tileRect = Rect::Rect(rect_x, rect_y, width, height);
            if (tileRect.intersectsRect(playerRect)){ //如果玩家包围盒与tile包围盒相撞
                //5 取得相撞部分
                Rect intersection = this->intersectsRect(playerRect, tileRect);
                
                if (i == 0){
                    //tile在正下方 落到了tile上
                player->_desiredPosition =player->_desiredPosition+Point(0,intersection.size.height+0.01f);
                player->_velocity = Point(player->_velocity.x, 0.f);  //取消player的y轴速度
                    player->_onGround = true;   //player处于地面
                    right=false;
                    _player->_jump=0;    //player 跳跃次数归零
                    if ( _bgMoveSpeed != 0.0f )
                        Director::getInstance()->getActionManager()->resumeTarget(_player); //player动画继续
                }else if (i == 1) //头顶到tile
                {
                    if(_player->_onGround){    //如果player在地面上且与上方瓦片发生碰撞， player被卡主
                        right=true;
                    }else{
                    //在上面的tile，要让主角向上移
                    player->_desiredPosition =player->_desiredPosition+Point(0,-intersection.size.height);
                    }
                    player->_velocity = Point(player->_velocity.x, 0.f);
                    
                }
                else if (i == 2)
                {
                    //左边的tile，主角右移
                    player->_desiredPosition =player->_desiredPosition+Point(intersection.size.width,0);
                    
                }
                else if (i == 3)
                {
                    //右边的tile， 左移
                 player->_desiredPosition =player->_desiredPosition+Point(-(intersection.size.width+_bgMoveSpeed),0);
                          right=true;
                    
                }
                else
                {
                    //7 如果碰撞的水平面大于竖直面，说明角色是上下碰撞
                    if (intersection.size.width > intersection.size.height)
                    {
                       _player->_velocity = Point(player->_velocity.x, 0.f);
                          //tile is diagonal, but resolving collision vertically
                    
                        float intersectionHeight;
                        if (i>5) //说明是踩到斜下的砖块,角色应该向上去
                        {
                           player->_onGround = true;
                               _player->_jump=0;
                            right=false;
                            Director::getInstance()->getActionManager()->resumeTarget(_player);
                           intersectionHeight = intersection.size.height+0.01f;
                        }
                        else  //说明是顶到斜上的砖块，角色应该向下托
                        {
                            if(_player->_onGround){
                                right=true;
                            }else{
                            intersectionHeight = -intersection.size.height;
                            }
                        }
                        player->_desiredPosition = player->_desiredPosition+Point(0,intersectionHeight);
                                            }
                    else //如果碰撞的水平面小于竖直面，说明角色是左右撞到
                    {
                         float resolutionWidth;
                        if (i == 6 || i == 4) //角色碰到斜左边的tile 角色应该向右去
                        {
                            resolutionWidth = intersection.size.width;
                        }
                        else //角色碰到斜右边的tile, 角色应该向左去
                        {
                            resolutionWidth = -intersection.size.width;
                            
                            right=true;
                            
                        }
                        player->_desiredPosition = player->_desiredPosition+Point(resolutionWidth,0);
                    
      

                    }
                    
                }
            }
        }
    }

    if (!right) {       //如果player没被卡主 让player向背景的相反方向移动 再屏幕中玩家位置不变
         player->_desiredPosition.x+=_bgMoveSpeed;
        if(_map->convertToWorldSpace(_player->getPosition()).x<=150){
            player->_desiredPosition.x+=1;
        }
    }

}

//玩家同陷阱碰撞的检测和处理
void GameLayer::handleHazardCollisions(Player* player)
{
    this->getSurroundingTilesAtPosition(player->getPosition(), _hazards,tiles);
    //this->getOverlapTilesAtPosition(player->getPosition(), _hazards, tiles);
    for(auto i=0;i<8;i++){
        ValueMap dic=tiles[i];
        Rect playerRect = player->collisionBoundingBox();
        int gid = dic.at("gid").asInt();
        if (gid)
        {
            
            float rect_x = dic.at("x").asFloat();
            float rect_y = dic.at("y").asFloat();
            float width = _map->getTileSize().width;
            float height = _map->getTileSize().height;
         
            Rect tileRect = Rect::Rect(rect_x, rect_y, width, height);
            if (tileRect.intersectsRect(playerRect)){ //陷阱碰撞处理
                if ( touchHazardFirst == true ) {
                    touchHazardFirst=false;
                    // sleep(0.5);
                    
                    _player->stopAllActions();
                    animate1 = Animate::create(Player::get_role_animation_Die());
                    action1 = RepeatForever::create(animate1);
                    _player->runAction(action1);
                    
                    auto scene = GameOverScene::createScene(score, map_index);
                    Director::getInstance()->replaceScene(TransitionFade::create( TRANSITION_TIME*2, scene ));
                    // touchHazardFirst=false;
                }
            }
        }
    }
}

//玩家和金币碰撞的检测和处理
void GameLayer::handleCoinCollisions(Player* player)
{
    
    this->getOverlapTilesAtPosition(player->getPosition(), _coins, tiles);
    int size;
    if(_player->_big==false){
        size=6;
    }else{
        size=15;
    }
    for(auto i=0;i<size;i++){
        ValueMap dic=tiles[i];
        Rect playerRect = player->boundingBox();
        
            float rect_x = dic.at("x").asFloat();
            float rect_y = dic.at("y").asFloat();
            float width = _map->getTileSize().width;
            float height = _map->getTileSize().height;
            int gid=dic.at("gid").asInt();
        
            Rect tileRect = Rect::Rect(rect_x, rect_y, width, height);
            if (tileRect.intersectsRect(playerRect)){ //金币碰撞处理 可以用记分
                
                  if (gid ) {
                      
                      // if ( touchScoreFirst ) {
                      score++;
                      showScore();
                      
                      if ( item->energy < 5 && isSpeeding == false) {
                          item->energy++;
                          item->showEnergy(this);
                      }
                          touchScoreFirst = false;
                          
                          Point p=this->tileCoordForPosition(Point(rect_x,rect_y));
                          if(p.x<=_map->getContentSize().width && p.y<=22 && p.y>0){
                              p.y--;
                              _coins->removeTileAt(p);
                              CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/getcoin.mp3");
                              //CCLOG("%i\n", gid);
                              //touchScoreFirst=true;
                          }
                      // }
                  }
            }
    }
}

//玩家同飛行物件碰撞的检测和处理
void GameLayer::FlyCollisions(Player* player)
{
    
    this->getOverlapTilesAtPosition(player->getPosition(), _fly, tiles);
    int size;
    if(_player->_big==false){
        size=4;
    }else{
        size=12;
    }
    for(auto i=0;i<size;i++){
        ValueMap dic=tiles[i];
        Rect playerRect = player->boundingBox();
        
        float rect_x = dic.at("x").asFloat();
        float rect_y = dic.at("y").asFloat();
        float width = _map->getTileSize().width;
        float height = _map->getTileSize().height;
        int gid=dic.at("gid").asInt();
        Rect tileRect = Rect::Rect(rect_x, rect_y, width, height);
        if (tileRect.intersectsRect(playerRect)){ //金币碰撞处理 可以用记分
        
            if (gid ) {
            
                // if ( touchScoreFirst ) {
                // score++;
            
                touchScoreFirst = false;
            
                Point p=this->tileCoordForPosition(Point(rect_x,rect_y));
                if(p.x<=_map->getContentSize().width && p.y<=22 && p.y>0){
                    p.y--;
                    _fly->removeTileAt(p);
                    //touchScoreFirst=true;
                    if ( !_player->_big ) {
                        item->isflying=true;
                        // this->scheduleOnce( schedule_selector( GameLayer::removefly), 6.0f );
                        fly_time=8.0f;
                        
                    }
                }
                // }
            }
        }
    }
}

//玩家和傳送門碰撞的检测和处理

void GameLayer::handleGateCollisions(Player* player) {
    
    this->getOverlapTilesAtPosition(player->getPosition(), _gate, tiles);
    int size;
    if(_player->_big==false){
        size=4;
    }else{
        size=12;
    }
    for(auto i=0;i<size;i++){
        ValueMap dic=tiles[i];
        Rect playerRect = player->boundingBox();
        
        float rect_x = dic.at("x").asFloat();
        float rect_y = dic.at("y").asFloat();
        float width = _map->getTileSize().width;
        float height = _map->getTileSize().height;
        int gid=dic.at("gid").asInt();
        Rect tileRect = Rect::Rect(rect_x, rect_y, width, height);
        if (tileRect.intersectsRect(playerRect)){ //金币碰撞处理 可以用记分
            
            if (gid ) {
                // change scene
                // CCLOG("");
                if ( touchHazardFirst == true ) {
                    touchHazardFirst=false;
                    // sleep(0.5);
                    
                    auto scene = GameTransferScene::createScene(score,++map_index,item->energy);
                    
                    // auto scene = GameLayer::createScene(score,++map_index,item->energy);
                    Director::getInstance()->replaceScene(TransitionFade::create( TRANSITION_TIME, scene ));
                    // touchHazardFirst=false;
                }
            }
        }
    }

}

//玩家和提示物件碰撞的检测和处理
void GameLayer::handleHintCollisions(Player* player) {
    bool allnottouch=true;
    this->getOverlapTilesAtPosition(player->getPosition(), _hint, tiles);
    int size;
    if(_player->_big==false){
        size=4;
    }else{
        size=12;
    }
    for(auto i=0;i<size;i++){
        ValueMap dic=tiles[i];
        Rect playerRect = player->boundingBox();
        
        float rect_x = dic.at("x").asFloat();
        float rect_y = dic.at("y").asFloat();
        float width = _map->getTileSize().width;
        float height = _map->getTileSize().height;
        int gid=dic.at("gid").asInt();
        Rect tileRect = Rect::Rect(rect_x, rect_y, width, height);
        if (tileRect.intersectsRect(playerRect)){
            
            if (gid ) {
                if ( touchHintPrev == false  ) {
                    if ( map_index == -1 ) {
                        if ( hint_count %7 != 3 && hint_count %7 != 4 ) {
                            pauseflag=true;
                            _bgMoveSpeed = 0.0f;
                            cocos2d::Director::getInstance()->getActionManager()->pauseTarget(player);
                        }
                        
                        switch( hint_count %7) {
                            case 0:
                                hintItem = MenuItemImage::create("Jumphint.png", "Jumphint.png",
                                                             CC_CALLBACK_1(GameLayer::GoToGameScene, this));
                                break;
                            case 1:
                                hintItem = MenuItemImage::create("doublejumphint.png", "doublejumphint.png",
                                                             CC_CALLBACK_1(GameLayer::GoToGameScene, this));
                                break;
                            case 2:
                                hintItem = MenuItemImage::create("shrinkhint.png", "shrinkhint.png",
                                                             CC_CALLBACK_1(GameLayer::GoToGameScene, this));
                                break;
                            case 3: // add laser
                                //hintItem = MenuItemImage::create("sizejump.jpg", "sizejump.jpg",
                                //                             CC_CALLBACK_1(GameLayer::GoToGameScene, this));
                                addLaserman(0);
                                break;
                            case 4:
                                // hintItem = MenuItemImage::create("sizejump.jpg", "sizejump.jpg",
                                //                             CC_CALLBACK_1(GameLayer::GoToGameScene, this));
                                break;
                            case 5:
                                hintItem = MenuItemImage::create("sprinthint.png", "sprinthint.png",
                                                             CC_CALLBACK_1(GameLayer::GoToGameScene, this));
                                break;
                            case 6:
                                hintItem = MenuItemImage::create("flyhint.png", "flyhint.png",
                                                             CC_CALLBACK_1(GameLayer::GoToGameScene, this));
                                break;
                        }
                        
                        if ( hint_count%7 != 3 && hint_count %7 != 4 ) {
                            hintItem->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
                            //setMenu
                            hintMenu = Menu::create(hintItem, NULL);
                            hintMenu->setPosition(Point::ZERO);
                            this->addChild(hintMenu);
                        }
                        hint_count++;
                    }
                    else
                        addLaserman(0);
                }
                touchHintPrev = true;
                allnottouch=false;
            }
            else if(gid==0&&allnottouch==true&&i==size-1)
                touchHintPrev = false;
            
        }
    }
    
}



/*
//玩家和飛行物件碰撞的检测和处理
void GameLayer::handleFlyCollisions(Player* player) {
    
    //if ( touchFlyFirst == false ) {
    //    touchFlyFirst=true;
    //    item->isCollsitionWithItems( player, this, _player->_big);
    //}
    
    if ( item->isCollsitionWithItems( player, _map, _player->_big) ) {
        touchFlyFirst = true;
    }
    
    if( item->isflying==true )
    {
        CCLOG("isflying = true");
        this->scheduleOnce( schedule_selector( GameLayer::removefly), 6.0f );
    }
    
}
*/

bool GameLayer::onTouchBegin(cocos2d::Touch *touch, cocos2d::Event *event) {
    
    if ( pauseflag == true )
        return true;
    
    //每次触发跳跃 跳跃数增加 右碰撞重置
    _player->_jump++;
    _player->_jumpable=true;
    right=false;
    return true;
}

//player大小控制
void GameLayer::Shrink_Magnify(Ref* ref )
{
    if ( pauseflag == false ) {
        if ( _player->_big ) {      //玩家变小
            _player->cocos2d::Node::setScale(0.5);
            _player->_big =false;
        }
        else {
            _player->cocos2d::Node::setScale(1); //玩家变大
            _player->_big =true;
       
        }
        right=false;   //玩家大小改变后 可能不会被卡主 将其重置
    }
}

/*
void GameLayer::RenewFly(float dt)
{
    item->RenewFlyIcon(this);
}
*/
void GameLayer::removefly(float dt) {
    item->isflying = false;
    touchFlyFirst = false;
}

// laser
void GameLayer::laserInit() {
    
    _laserman = Sprite::create( "pic/beeman.png" );
    _laserman->setPosition( Point( 0,0 ) );
    _laserman->setVisible(false);
    addChild( _laserman ,10 );
    //this->schedule( schedule_selector( GameLayer::addLaserman ), (20) );
    
    _laser = Sprite::create( "pic/laser.png" );
    _laser->setPosition( Point( 0,-500 ) );
    _laser->setVisible(false);
    addChild( _laser ,9 );
    
    _laserThin = Sprite::create( "pic/laser_thin.png" );
    _laserThin->setPosition( Point( 0,-500 ) );
    _laserThin->setVisible(false);
    addChild( _laserThin ,9 );
}

void GameLayer::addLaserman( float dt) {
    
    // random = 0.35 ~ 0.65, in order to random create the position of y
    auto random = CCRANDOM_0_1();
    if ( random < 0.375 ) {
        random = 0.375;
    }
    else if ( random > 0.75 ) {
        random = 0.75;
    }
    // pointNode->setPosition( Point( visibleSize.width + pointNode->getContentSize().width + origin.x, ( random * visibleSize.height )) );
    
    random = _player->getPositionY()+100;
    _laserman->setPosition( Point( visibleSize.width - 50,  random ) );
    _laserThin->setPosition( visibleSize.width/2-125, _laserman->getPositionY()+10);
    // _laserman->setPosition( Point( visibleSize.width - 50,  random * visibleSize.height ) );
    
    _laserman->setVisible(true);
    
    _laserThin->setVisible(true);
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/SElaunch.mp3");
    
    this->scheduleOnce( schedule_selector( GameLayer::addLaser), 2.7f );
    this->scheduleOnce( schedule_selector( GameLayer::removeLaser), 3.0f );
    
    
}

void GameLayer::addLaser( float dt) {
    _laser->setPosition( visibleSize.width/2-125, _laserman->getPositionY()+10);
    _laser->setVisible(true);
    _laserThin->setVisible(false);
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/SElaunch.mp3");
}

void GameLayer::removeLaser( float dt) {
    _laserman->setVisible(false);
    _laser->setVisible(false);
    _laser->setPosition( Point( 0,-500 ) );
}

void GameLayer::collisionWithLaser(Player* player) {
    
    auto playerposition= _map->convertToWorldSpace(player->getPosition()); //player掉到屏幕外判定
    Rect otherRect = player->boundingBox();
    otherRect.origin.x = playerposition.x;
    otherRect.origin.y = playerposition.y;
    
    if ( otherRect.intersectsRect( _laser->boundingBox() ) && isInvincible == false )
    {
        touchLaserFirst=true;
        CCLOG("collision");
        if ( touchHazardFirst == true ) {
            touchHazardFirst = false;
            
            _player->stopAllActions();
            animate1 = Animate::create(Player::get_role_animation_Die());
            action1 = RepeatForever::create(animate1);
            _player->runAction(action1);
            
            // CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/Die.mp3");
            auto scene = GameOverScene::createScene(score, map_index);
            Director::getInstance()->replaceScene(TransitionFade::create( TRANSITION_TIME*2, scene ));
            // touchHazardFirst = false;
        }
        
    }
    
    
}

// JumpToGameScene
void GameLayer::GoToGameScene( cocos2d::Ref *sender)
{
    pauseflag=false;
    hintItem->removeFromParent();
    hintMenu->removeFromParent();
    _bgMoveSpeed = GROUND_SPEED;
    Director::getInstance()->getActionManager()->resumeTarget(_player); //player动画继续
    
}

void GameLayer::Pause(Ref* ref ) {
    
    if ( pauseflag == false ) {
        pauseflag=true;
        _bgMoveSpeed = 0.0f;
        cocos2d::Director::getInstance()->getActionManager()->pauseTarget(_player);
    }
    else {
        pauseflag=false;
        _bgMoveSpeed = GROUND_SPEED;
        Director::getInstance()->getActionManager()->resumeTarget(_player); //player动画继续
    }
}

void GameLayer::showScore() {
    
    if ( label != NULL )
        label->removeFromParent();
    //创建标签
    label = CCLabelTTF::create(std::to_string(score), "Arial", 36);
    //设置标签文字颜色
    label->setColor(ccc3(255, 255, 255));
    //设置标签位置
    label->setPosition(ccp(visibleSize.width*1/2, visibleSize.height*0.9));
    
    //设置锚点为左下角
    // label->setAnchorPoint(CCPointZero);
    
    //设置标签的横向对齐方式为向左对齐，这样标签内容增加，只会向右增加
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    
    //添加为子节点
    this->addChild(label,1);
}

