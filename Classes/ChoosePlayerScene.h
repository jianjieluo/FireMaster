#ifndef CHOOSEPLAYERSCENE_H_
#define CHOOSEPLAYERSCENE_H_

#include "Global.h"
#include "cocos2d.h"

class choosePlayerScene : public cocos2d::Layer {
 public:
  // there's no 'id' in cpp, so we recommend returning the class instance
  // pointer
  static cocos2d::Scene* createScene();

  // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of
  // returning 'id' in cocos2d-iphone
  virtual bool init();

  // virtual bool onTouchBegan(Touch *touch, Event *unused_event);

  // implement the "static create()" method manually
  CREATE_FUNC(choosePlayerScene);

  void update(float ft);

  void choose_click(Ref* sender, Global::TankType t_type);
  void ok_click(Ref* sender);

 private:
  Global::TankType leftTankType, rightTankType, currTankType;
  int num_need_choose;

  Label *tipsUI, *detailUI;
};
#endif  // CHOOSEPLAYERSCENE_H_
