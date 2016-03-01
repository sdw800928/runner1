#ifndef __SCORE_H__
#define __SCORE_H__

#include "cocos2d.h"

class Items
{
public:
    
    Items(cocos2d::Layer *layer);
    
    void SpawnCoin( cocos2d::Layer *layer);
    void ShowNode(cocos2d::Layer *layer, int nodeNum );
    void DeleteNode( cocos2d::Sprite *node);
    void SpawnFly(cocos2d::Layer *layer);
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    cocos2d::Sprite *EnergyNode1 = cocos2d::Sprite::create("pic/Ball.png");
    cocos2d::Sprite *EnergyNode2 = cocos2d::Sprite::create("pic/Ball.png");
    cocos2d::Sprite *EnergyBar = cocos2d::Sprite::create("pic/bar.png");
};

#endif // __SCORE_H__
