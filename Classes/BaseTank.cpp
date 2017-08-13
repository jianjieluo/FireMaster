#include "BaseTank.h"
#include "Bullet.h"
#include "FireMaster.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;
using namespace cocos2d;

BaseTank::BaseTank() { isInTurn = false; }

BaseTank::~BaseTank() {}

void BaseTank::addEvents() { this->addTouchListener(); }

void BaseTank::addTouchListener() {
  auto listener = cocos2d::EventListenerTouchOneByOne::create();
  listener->setSwallowTouches(true);

  // 开始按下的回调函数
  listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event) {
    cocos2d::Vec2 p = touch->getLocation();
    cocos2d::Rect rect = this->getBoundingBox();

    if (rect.containsPoint(p)) {
      if (Global::turn % 2 == this->getSide() && Global::bullets.empty() &&
          !isInTurn) {
        m_power = 0;
        m_istouch = true;  //按钮按下
        isInTurn = true;
        this->schedule(schedule_selector(BaseTank::updatePowerbar),
                       0.1);  //蓄力时间判断，每隔0.1秒调度一次

        //按下取消等待计时器
        auto clockUI = this->getParent()->getChildByName("waitClock");
        if (clockUI != NULL) {
          clockUI->setVisible(false);
        }

        // 按下的时候添加力度进度条到场景里面去
        // 创建蓄力条
        powerbar = Progress::create("progressBg.png", "blood.png");
        powerbar->setScaleX(3);
        powerbar->setScaleY(1.5);
        powerbar->setRotation(180);
        powerbar->setProgress(0);
        // 相对于坦克来设置对应的powerbar位置
        powerbar->setPosition(this->getPosition().x,
                              this->getPosition().y + 100);
        this->getParent()->addChild(this->powerbar);

        return true;  // to indicate that we have consumed it.
      }
    }
    return false;  // we did not consume this event, pass thru.
  };

  // 结束按下的回调函数
  listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event) {
    if (Global::turn % 2 == this->getSide()) {
      // 把蓄力条给去掉
      this->unschedule(schedule_selector(BaseTank::updatePowerbar));
      this->getParent()->removeChild(powerbar);

      //发射音效
      SimpleAudioEngine::getInstance()->playEffect("music/attack.wav");

      CCLOG("launch _power:%f", m_power);
      BaseTank::touchEvent(touch);
      m_istouch = false;
    }
  };

  cocos2d::Director::getInstance()
      ->getEventDispatcher()
      ->addEventListenerWithFixedPriority(listener, 30);
}

void BaseTank::touchEvent(cocos2d::Touch* touch) {
  this->runAttack();
  if (this->getSide() % 2 == 0) {
    CCLOG("touched Left Tank");
  } else {
    CCLOG("touched Right Tank");
  }
}

void BaseTank::updatePowerbar(float ft) {
  if (m_istouch) {
    // 按照增长速度来蓄力，并且反馈在UI上面
    m_power += m_pressv;
    powerbar->setProgress(5 * m_power);
  }
}

void BaseTank::setCurrBulletName(const std::string& t_name) {
  curr_bullet_name = t_name;
}
void BaseTank::setBulletCount(const int n) { bullet_count = n; }
void BaseTank::setSide(const int t_side) { m_side = t_side; }
const int BaseTank::getSide() const { return m_side; }

void BaseTank::runAttack() {
  auto fireAnimate = CallFunc::create([&]() {
    // 在这里添加那个发射的boom的动画，设置好位置，添加，运行动画，移除
    auto boom = Sprite::createWithSpriteFrameName("fire1.png");
    boom->setPosition(this->getPosition().x - 30, this->getPosition().y + 40);

    this->getParent()->addChild(boom, 2, "boom");  // 设置一个名字，便于追踪

    // 去掉爆炸后的效果
    auto s = Sequence::create(
        Animate::create(
            AnimationCache::getInstance()->getAnimation("fireAnimation")),
        CallFunc::create([&]() {
          this->getParent()
              ->getChildByName("boom")
              ->removeAllChildrenWithCleanup(true);
          this->getParent()->getChildByName("boom")->removeFromParentAndCleanup(
              true);
          // this->getParent()->getChildByName("boom")->removeFromParent();
        }),
        nullptr);
    boom->runAction(s);
  });

  auto launch = CallFunc::create([&]() {
    // 在这里添加子弹生成,同时设置好物理的刚体属性，旋转发射角度，水平和垂直初速度等等，
    // 利用m_power设置好子弹的杀伤力。在FireMaster场景类里面进行调度检测碰撞。
    auto b = Bullet::create(this->curr_bullet_name, this->getBulletHurt());
    // 子弹相关位置,0是左边，1是右边
    if (this->getSide() == 0) {
      b->setPosition(this->getPosition().x + 20, this->getPosition().y + 40);
      b->setRotation(-50.0f);
      auto a = this->getBulletVelocity();
      b->getPhysicsBody()->setVelocity(this->getBulletVelocity());
    } else if (this->getSide() == 1) {
      b->setPosition(this->getPosition().x - 30, this->getPosition().y + 40);
      b->setRotation(230.0f);
      auto a = this->getBulletVelocity();
      b->getPhysicsBody()->setVelocity(
          Vec2(-(this->getBulletVelocity().x), this->getBulletVelocity().y));
    }

    Global::bullets.push_back(b);
    this->getParent()->addChild(b, 1);
  });

  auto setDefaultLamda =
      CallFunc::create([&]() { this->setDefaultProperty(); });

  if (bullet_count == 1) {
    auto s = Sequence::create(
        this->getAttackAnimate(), fireAnimate, launch, DelayTime::create(0.5f),
        this->getAfterAttackAnimate(), setDefaultLamda, nullptr);
    this->runAction(s);
  } else if (bullet_count == 3) {
    auto s = Sequence::create(
        this->getAttackAnimate(), fireAnimate, launch, DelayTime::create(0.5f),
        fireAnimate, launch, DelayTime::create(0.5f), fireAnimate, launch,
        DelayTime::create(0.5f), this->getAfterAttackAnimate(), setDefaultLamda,
        nullptr);
    this->runAction(s);
  }
}
