#include "BlueTank.h"
#include "Bullet.h"
#include "FireMaster.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

using namespace CocosDenshion;
using namespace cocos2d;

BlueTank::BlueTank() {}

BlueTank::~BlueTank() {}

BlueTank* BlueTank::create()
{
    BlueTank* pSprite = new BlueTank();

    if (pSprite->initWithSpriteFrameName("blueAttack1.png"))
    {
        pSprite->autorelease();
        pSprite->initOptions();
        pSprite->addEvents();
        return pSprite;
    }

    CC_SAFE_DELETE(pSprite);
    return nullptr;
}

void BlueTank::initOptions()
{
    // do things here like setTag(), setPosition(), any custom logic.
    auto tankScaleSize = 0.7;

    this->setAnchorPoint(Point(0.5, 0.5));
    this->setPosition(100, 100);
    this->setScale(tankScaleSize);

    this->setDefaultProperty();
    isInTurn = false;
}


// 纯虚函数接口实现
void BlueTank::setDefaultProperty()
{
    this->setCurrBulletName(default_bullet_name);
	this->setBulletCount(1);
}

int BlueTank::getBulletHurt()  const {
    return (this->curr_bullet_name == "tank_bullet4.png") ? Global::bigBullet : Global::smallBullet;
}
Vec2 BlueTank::getBulletVelocity()  const {
    // 此处根据具体坦克的不同可以返回不同的值
    // 注意，这里统一x，y均为整数，方向的不同已经在基类里面体现了
    return Vec2(m_power * 25, m_power * 20);
}
Animate* BlueTank::getAttackAnimate()  const {
    return Animate::create(AnimationCache::getInstance()->getAnimation("blueTankAttackAnimation"));
}
Animate* BlueTank::getAfterAttackAnimate()  const {
    return Animate::create(AnimationCache::getInstance()->getAnimation("blueTankAfterAttackAnimation"));
}