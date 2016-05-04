#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "cocos2d.h"
#include <vector>

class GameOverScene : public cocos2d::Layer
{
public:

    static cocos2d::Scene* createScene(int score, int index);

    virtual bool init();

    CREATE_FUNC(GameOverScene);
    
private:
    void GoToMainMenuScene( cocos2d::Ref *sender );
    void GoToGameScene( cocos2d::Ref *sender );
    void ReviveGameScene( cocos2d::Ref *sender );
    void showScore( cocos2d::Layer *layer, int score );
    
    bool isHaveSaveFile();
    void getHighestHistorySorce();
    
    // static int myScore;
    // cocos2d::LabelTTF *label = NULL;
    cocos2d::Sprite *label = NULL;
    std::vector<cocos2d::Sprite*> arr_label;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    int score_arr[5] = {0, 0, 0, 0, 0};
};

#endif // __HELLOWORLD_SCENE_H__
