#ifndef __SCORE_H__
#define __SCORE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class Items
{
public:
    
    Items(cocos2d::Layer *layer);
    
    void SpawnFly(cocos2d::Layer *layer);
    bool isCollsitionWithItems( cocos2d::Node* node, cocos2d::TMXTiledMap *_map, bool big );
    
    void showEnergy( cocos2d::Layer *layer );
    
    // score and energy
    int score = 0;
    int energy = 0;

    bool speedup = false;
    bool isflying = false;
    
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    // spider Node
    cocos2d::Sprite *flyNode = cocos2d::Sprite::create("pic/spider_icon.png");
    bool flyflag = true;
    
    cocos2d::LabelTTF *label = NULL;
    
};

#endif // __SCORE_H__
