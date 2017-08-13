#ifndef BLUETANK_H_
#define BLUETANK_H_

#include "BaseTank.h"

class BlueTank : public BaseTank {
 public:
  BlueTank();
  ~BlueTank();

  static BlueTank* create();

  // ��ʼ������������������Ϣ
  virtual void setDefaultProperty();

  virtual int getBulletHurt() const;
  virtual Vec2 getBulletVelocity() const;
  virtual Animate* getAttackAnimate() const;
  virtual Animate* getAfterAttackAnimate() const;
  virtual const Rect getTankBoundingBox() const;

 private:
  // 不同的坦克可以使用不同的子弹图片
  const std::string default_bullet_name = "bullet2.png";
};

#endif