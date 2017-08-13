#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "Bullet.h"

USING_NS_CC;

class Global {
 public:
  Global();
  ~Global();
  static int turn;
  static std::list<Bullet*> bullets;
  static bool winSide;  // 0 蓝车胜， 1 黄车胜
  static bool isGameover;

  //全局设置游戏参数
  static int smallBullet;
  static int bigBullet;
  // 按下的时候力度增长的速率
  static float pressv;
  static int windPower;
  // Tank types enum class , used to choose different type tank before the game
  // start.
  static const enum class TankType { none, blue, yellow };
};

#endif
