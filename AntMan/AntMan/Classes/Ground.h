#ifndef __GROUND_H__
#define __GROUND_H__

#include "cocos2d.h"

class Ground
{
public:
    
    Ground();
    void SpawnGround( cocos2d::Layer *layer);
    void createGround( cocos2d::Layer *layer);
    
    bool speedup = false;
    
private:
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Sprite *ground1, *ground2, *ground3;
};

#endif // __GROUND_H__
