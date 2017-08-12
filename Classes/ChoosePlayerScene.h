#ifndef CHOOSEPLAYERSCENE_H_
#define CHOOSEPLAYERSCENE_H_

#include "cocos2d.h"
#include "Global.h"

class choosePlayerScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(choosePlayerScene);

	void StartBtn_click(Ref * sender);

private:
    Global::TankType leftTankType, rightTankType;
    int num_need_choose;
};
#endif // CHOOSEPLAYERSCENE_H_
