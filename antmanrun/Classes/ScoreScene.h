#ifndef __SCORE_SCENE_H__
#define __SCORE_SCENE_H__

#include "cocos2d.h"
#include <vector>

class ScoreScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(ScoreScene);
    
private:
    void GoToMainMenuScene( cocos2d::Ref *sender);
    
    bool isHaveSaveFile();
    void getHighestHistorySorce();
    int showScore(int score, int rank) ;
    
    int score_arr[5] = {0, 0, 0, 0, 0};
    // std::vector<cocos2d::Sprite*> arr_label1;
    std::vector< std::vector<cocos2d::Sprite*>> arr_label;
    cocos2d::Sprite *label = NULL;
    std::vector<cocos2d::Sprite*> tmp_label_arr;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
};

#endif // __SCORE_SCENE_H__
