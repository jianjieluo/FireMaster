#include "BlueTank.h"
#include "Bullet.h"
#include "Global.h"

USING_NS_CC;

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

    // set 物理属性，链接刚体
}

void BlueTank::addEvents() {
    this->addTouchListener();
}

void BlueTank::addTouchListener()
{
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    // 开始按下的回调函数
    listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        cocos2d::Vec2 p = touch->getLocation();
        cocos2d::Rect rect = this->getBoundingBox();

        if (rect.containsPoint(p))
        {
            m_power = 0;
            m_istouch = true;//按钮按下
            this->schedule(schedule_selector(BlueTank::updatePowerbar), 0.1);//蓄力时间判断，每隔0.1秒调度一次

                                                                               // 按下的时候添加力度进度条到场景里面去
                                                                               // 创建蓄力条，先用label代替
            powerbar = Label::createWithTTF("0", "fonts/arial.ttf", 20);
            // 相对于坦克来设置对应的powerbar位置
            powerbar->setPosition(this->getPosition().x, this->getPosition().y + 100);
            this->getParent()->addChild(this->powerbar);

            return true; // to indicate that we have consumed it.
        }
        return false; // we did not consume this event, pass thru.
    };

    // 结束按下的回调函数
    listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        // 把蓄力条给去掉
        this->unschedule(schedule_selector(BlueTank::updatePowerbar));
        this->getParent()->removeChild(powerbar);

        CCLOG("launch _power:%f", m_power);
        BlueTank::touchEvent(touch);
        m_istouch = false;
    };

    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);
}

void BlueTank::touchEvent(cocos2d::Touch* touch)
{
    this->runAttack();
    CCLOG("touched BlueTank");
}

void BlueTank::updatePowerbar(float ft)
{
    if (m_istouch)
    {
        // 按照增长速度来蓄力，并且反馈在UI上面
        m_power += m_pressv;
        auto temp = std::to_string(m_power);
        powerbar->setString(temp);
    }
}

void BlueTank::runAttack()
{
    auto fireAnimate = CallFunc::create([&]() {
        // 在这里添加那个发射的boom的动画，设置好位置，添加，运行动画，移除
    });

    auto launch = CallFunc::create([&]() {
        // 在这里添加子弹生成,同时设置好物理的刚体属性，旋转发射角度，水平和垂直初速度等等，
        // 利用m_power设置好子弹的杀伤力。在FireMaster场景类里面进行调度检测碰撞。
    });

    auto attackAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("blueTankAttackAnimation"));

	auto s = Sequence::create(attackAnimate, fireAnimate, launch, nullptr);
    this->runAction(s);
}