#ifndef __OPTION_SCENE_H__
#define __OPTION_SCENE_H__

#include "cocos2d.h"

class OptionScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(OptionScene);
    
private:
    int bgm;
    int se;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    cocos2d::LabelTTF *labelbgm = NULL;
    cocos2d::LabelTTF *labelse = NULL;
    
    void GoToMainMenuScene( cocos2d::Ref *sender);
    void showBGMVolume();
    void showSoundEffectVolume();
    
    void addBGMVolume(cocos2d::Ref *sender);
    void minusBGMVolume(cocos2d::Ref *sender);
    void addSEVolume(cocos2d::Ref *sender);
    void minusSEVolume(cocos2d::Ref *sender);
};

#endif // __OPTION_SCENE_H__
