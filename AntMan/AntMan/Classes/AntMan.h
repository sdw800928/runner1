#ifndef __ANTMAN__H__
#define __ANTMAN__H__

#include "cocos2d.h"

class AntMan
{
public:
    
    AntMan( cocos2d::Layer *layer );
    
    //AntMan take action
    void Jump();
    void RushDown();
    void Shrink_Magnify( cocos2d::Layer *layer );
    
    bool inwater = false;
    bool big = false;
    
private:
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Sprite *antMan;
    
    cocos2d::Sprite *antMan2;
    
};

#endif
