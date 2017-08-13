#ifndef __INITSCENE_H__
#define __INITSCENE_H__

#include "cocos2d.h"

class initScene : public cocos2d::Layer {
 public:
  // there's no 'id' in cpp, so we recommend returning the class instance
  // pointer
  static cocos2d::Scene* createScene();

  // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of
  // returning 'id' in cocos2d-iphone
  virtual bool init();

  // implement the "static create()" method manually
  CREATE_FUNC(initScene);

  void StartBtn_click(Ref* sender);
  void introductionBtn_click(Ref* sender);
};
#endif  // __INITSCENE_H__
