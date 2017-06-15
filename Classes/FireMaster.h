#ifndef __FIRE_MASTER_H__
#define __FIRE_MASTER_H__

#include "cocos2d.h"
#include "FireMaster.h"

using namespace std;
USING_NS_CC;

class FireMaster : public Layer {
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(FireMaster);

private:
	Size visibleSize;
	Vec2 origin;
	Sprite *yellowTank, *blueTank;
	Sprite *obstacle;
	Sprite *topUI;
	void addSprite();
	void tank_bullet4_1_click(Ref * sender);
	void tanks_crateRepair_1_click(Ref * sender);
	void tanks_crateArmor_1_click(Ref * sender);
	void tanks_crateAmmo_1_click(Ref * sender);
	
	void tank_bullet4_2_click(Ref * sender);
	void tanks_crateRepair_2_click(Ref * sender);
	void tanks_crateArmor_2_click(Ref * sender);
	void tanks_crateAmmo_2_click(Ref * sender);
};

#endif // __FIRE_MASTER_H__