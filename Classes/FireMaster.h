#ifndef __FIRE_MASTER_H__
#define __FIRE_MASTER_H__

#include "cocos2d.h"
#include "FireMaster.h"
#include "Global.h"
#include "BlueTank.h"
#include "YellowTank.h"
#include "ui/CocosGUI.h"

using namespace std;
USING_NS_CC;

class FireMaster : public Layer {

public:
	static cocos2d::Scene* createScene();
	virtual bool init();

    void setPhysicsWorld(PhysicsWorld * world);
    void addSprite();

    void powerBullet_Btn1_click(Ref * sender);
    void fix_Btn1_click(Ref * sender);
    void defence_Btn1_click(Ref * sender);
    void triAttack_Btn1_click(Ref * sender);
    void powerBullet_Btn2_click(Ref * sender);
    void fix_Btn2_click(Ref * sender);
    void defence_Btn2_click(Ref * sender);
    void triAttack_Btn2_click(Ref * sender);

    void nextTurn();
    void updateTurnUI(float ft);

	CREATE_FUNC(FireMaster);

private:
    Size visibleSize;
    Vec2 origin;

    BlueTank *blueTank;
    YellowTank* yellowTank;

	Sprite *obstacle;
	Sprite *topUI;
	ui::Button *powerBullet_Btn1, *fix_Btn1, *defence_Btn1, *triAttack_Btn1, *powerBullet_Btn2, *fix_Btn2, *defence_Btn2, *triAttack_Btn2;
    bool istouch;


private:
    PhysicsWorld* m_world;
    int m_turn;
    cocos2d::Label* turnUI;
};

#endif // __FIRE_MASTER_H__