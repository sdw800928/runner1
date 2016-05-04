
#ifndef Player_h
#define Player_h
#include "cocos2d.h"
class Player : public cocos2d::Sprite //player类 从sprite继承
{
public:
    //当前速度
    cocos2d::Point _velocity;
    //期望位置
    cocos2d::Point _desiredPosition;
    //是否在地面
    bool _onGround;
    //当前跳跃数
    int _jump;
    //是否可以起跳
    bool _jumpable;
    //判断变大变小
    bool _big;
    //以图片初始化
    virtual bool initWithFile(const char *pszFilename);
    
   //创建player对象
    static Player* create(const char *pszFileName);
    //创建player动画
    static cocos2d::Animation* get_role_animation();
    //生成碰撞盒
    cocos2d::Rect collisionBoundingBox();
    //player update
    void update(float delta, bool isFlying, bool pauseflag, bool isSpeeding, float _bgMoveSpeed);
    
    //创建 Speed_Up player动画
    static cocos2d::Animation* get_role_animation_Sprint();
    //创建 Die player动画
    static cocos2d::Animation* get_role_animation_Die();
}
;
#endif 
