#ifndef BASETANK_H_
#define BASETANK_H_

#include <string>
#include "Global.h"
#include "Progress.h"
#include "cocos2d.h"

class BaseTank : public cocos2d::Sprite {
 public:
  // 用来防止疯狂连按的
  bool isInTurn;

 public:
  BaseTank();
  ~BaseTank();

  void addEvents();

  void addTouchListener();
  void touchEvent(cocos2d::Touch* touch);

  void updatePowerbar(float ft);

  void runAttack();
  // 不同的坦克涉及到不同的属性，所以在这里作为纯虚函数，所有的都需要显式实现
  virtual void setDefaultProperty() = 0;
  virtual const Rect getTankBoundingBox() const = 0;

  void setCurrBulletName(const std::string& t_name);
  void setBulletCount(const int n);
  void setSide(const int t_side);

  const int getSide() const;

 protected:
  const float m_pressv = Global::pressv;
  const int m_basic_hurt = Global::smallBullet;
  float m_power;
  int m_side;

  bool m_istouch;

  Progress* powerbar;

  std::string curr_bullet_name;
  int bullet_count;

 protected:
  virtual int getBulletHurt() const = 0;
  virtual Vec2 getBulletVelocity() const = 0;
  virtual Animate* getAttackAnimate() const = 0;
  virtual Animate* getAfterAttackAnimate() const = 0;
};

#endif