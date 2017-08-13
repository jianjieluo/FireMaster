#include "YellowTank.h"
#include "Bullet.h"
#include "FireMaster.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;
using namespace cocos2d;    

YellowTank::YellowTank() {}

YellowTank::~YellowTank() {}

YellowTank* YellowTank::create()
{
    YellowTank* pSprite = new YellowTank();

    if (pSprite->initWithSpriteFrameName("yellowAttack1.png"))
    {
        pSprite->autorelease();
        pSprite->addEvents();
        return pSprite;
    }

    CC_SAFE_DELETE(pSprite);
    return nullptr;
}

void YellowTank::setDefaultProperty()
{
    this->setCurrBulletName(default_bullet_name);
	this->setBulletCount(1);
}

int YellowTank::getBulletHurt()  const {
    return (this->curr_bullet_name == "powerAttackUI.png") ? Global::bigBullet : Global::smallBullet;
}
Vec2 YellowTank::getBulletVelocity()  const {
    // 此处根据具体坦克的不同可以返回不同的值，在这里m_power其实已经没有太大的作用了
    // 注意，这里统一x，y均为整数，方向的不同已经在基类里面体现了
    return Vec2(m_power * 25, m_power * 20);
}
Animate* YellowTank::getAttackAnimate()  const {
    return Animate::create(AnimationCache::getInstance()->getAnimation("yellowTankAttackAnimation"));
}
Animate* YellowTank::getAfterAttackAnimate()  const {
    return Animate::create(AnimationCache::getInstance()->getAnimation("yellowTankAfterAttackAnimation"));
}
const Rect YellowTank::getTankBoundingBox() const {
    return Rect(this->getBoundingBox().origin.x, this->getBoundingBox().origin.y,
        this->getBoundingBox().size.width,
        this->getBoundingBox().size.height - 40);
}