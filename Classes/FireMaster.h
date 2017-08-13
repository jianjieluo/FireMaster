#ifndef __FIRE_MASTER_H__
#define __FIRE_MASTER_H__

#include "BaseTank.h"
#include "BlueTank.h"
#include "Global.h"
#include "Progress.h"
#include "YellowTank.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <string>

using namespace std;
USING_NS_CC;

class FireMaster : public Layer {
 public:
  static cocos2d::Scene* createScene(const Global::TankType leftTankType,
                                     const Global::TankType rightTankType);
  static FireMaster* create(const Global::TankType leftTankType,
                            const Global::TankType rightTankType);
  // virtual bool init();
  bool initWithTwoTankType(const Global::TankType leftTankType,
                           const Global::TankType rightTankType);

  void setPhysicsWorld(PhysicsWorld* world);
  void updateTurnUI(float ft);
  void updateCollision(float ft);
  void updateBulletRotation(float ft);
  void updateBulletVelocity(float ft);
  void refreshRandomWindPower();

  void timer(float a);

  void nextTurn();
  float windPower;

 private:
  Sprite *defence1, *defence2;

  Sprite* booms[3];
  Progress *wind1, *wind2;

  Size visibleSize;
  Vec2 origin;

  BaseTank *currTank, *leftTank, *rightTank;

  Sprite* obstacle;
  Sprite* topUI;
  ui::Button *powerBullet_Btn1, *fix_Btn1, *defence_Btn1, *triAttack_Btn1,
      *powerBullet_Btn2, *fix_Btn2, *defence_Btn2, *triAttack_Btn2;
  ProgressTimer *pT1, *pT2, *wind;
  bool istouch;

  void addSprite();

  void Gameover();

  void powerBullet_Btn1_click(Ref* sender);
  void fix_Btn1_click(Ref* sender);
  void defence_Btn1_click(Ref* sender);
  void triAttack_Btn1_click(Ref* sender);

  void powerBullet_Btn2_click(Ref* sender);
  void fix_Btn2_click(Ref* sender);
  void defence_Btn2_click(Ref* sender);
  void triAttack_Btn2_click(Ref* sender);
  ProgressTimer* waitClock;

 private:
  PhysicsWorld* m_world;
  int m_turn;
  cocos2d::Label* turnUI;

  Rect m_checkingRects[4];

  Progress *hp1, *hp2;
};

#endif  // __FIRE_MASTER_H__