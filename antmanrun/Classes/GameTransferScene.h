#ifndef __GAME_TRANSFER_SCENE_H__
#define __GAME_TRANSFER_SCENE_H__

#include "cocos2d.h"

class GameTransferScene : public cocos2d::Layer
{
public:

    static cocos2d::Scene* createScene(int score, int map_index, int energy);

    virtual bool init();

    CREATE_FUNC(GameTransferScene);
    
private:
    void GoToGameScene( float dt );
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
};

#endif // __GAME_TRANSFER_SCENE_H__
