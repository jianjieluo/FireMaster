#pragma once

#include "cocos2d.h"

class Bullet : public cocos2d::Sprite {
 public:
  Bullet();
  ~Bullet();
  static Bullet* create(const std::string& filename, int t_hurt);

  void initOptions();

  // »ù±¾²Ù×÷
  int getHurtness() const;
  void setHurtness(const int t_hurt);

 private:
  int m_hurt;
};