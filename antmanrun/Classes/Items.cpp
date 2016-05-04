#include "Items.h"
#include "Definitions.h"
#include <string>

USING_NS_CC;

Items::Items(cocos2d::Layer *layer) {
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
}

void Items::SpawnFly(cocos2d::Layer *layer) {
    /*
    // crate fly body
    Sprite *pointNode = Sprite::create( "pic/Ball_bigger.png" );
    
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
    */
    

    flyNode->setPosition( Point( visibleSize.width+100, ( visibleSize.height/3 )) );
    
    layer->addChild(flyNode);
    
}

bool Items::isCollsitionWithItems( Node* node, cocos2d::TMXTiledMap *_map, bool big ){
    
    
    auto player= _map->convertToWorldSpace(node->getPosition()); //player掉到屏幕外判定
    Rect otherRect = node->boundingBox();
    otherRect.origin.x = player.x;
    otherRect.origin.y = player.y;
    
    
    if ( otherRect.intersectsRect( flyNode->boundingBox() )  )
    {
        // CCLog("collision");
        if ( flyflag ) {
            flyNode->setVisible(false);
            flyflag = false;
            // if ( big == false ) {
                isflying =true;
            // }
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/Point.mp3");
        }
        return true;
    }
    return false;
}

void Items::showEnergy( cocos2d::Layer *layer ) {
    
    if ( label != NULL )
        label->removeFromParent();
    //创建标签
    label = CCLabelTTF::create(std::to_string(energy), "Arial", 36);
    //设置标签文字颜色
    label->setColor(ccc3(255, 0 , 0));
    //设置标签位置
    label->setPosition(ccp(visibleSize.width*0.8, visibleSize.height*0.9));
    
    //设置锚点为左下角
    // label->setAnchorPoint(CCPointZero);
    
    //设置标签的横向对齐方式为向左对齐，这样标签内容增加，只会向右增加
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    
    //添加为子节点
    layer->addChild(label,1);
}
