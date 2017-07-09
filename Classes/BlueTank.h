#pragma once

#include "cocos2d.h"
#include <string>
#include "Global.h"
#include "Progress.h"

class BlueTank : public cocos2d::Sprite
{
public:
    //可以和上一层场景交互的变量
    bool isInTurn;

public:
    BlueTank();
    ~BlueTank();
    static BlueTank* create();

    // 初始化函数，设置物理信息
    void initOptions();

    //总体添加事件
    void addEvents();

    //添加监听和触发事件
    void addTouchListener();
    void touchEvent(cocos2d::Touch* touch);

    //调度函数
    //更新力度的函数
    void updatePowerbar(float ft);

    //个体行为
    void runAttack();
    void setDefaultProperty();
    void setCurrBulletName(const std::string & t_name) {
        curr_bullet_name = t_name;
    }
    void setBulletCount(const int n) {
        bullet_count = n;
    }

private:
    // const 表示该精灵的固有属性
    // 按下的时候力度增长的速率
    const std::string default_bullet_name = "bullet2.png";
    const float m_pressv = Global::pressv;
    const int m_basic_hurt = Global::smallBullet;
    // 蓄的力量，用来和m_basic_hurt一起作为初速度的参考值，用来更新力度条
    float m_power;

    // 蓄力系统的相关设置
    bool m_istouch;
    // 蓄力UI
	Progress* powerbar;

    std::string curr_bullet_name;
    int bullet_count;
};