#include "cocos2d.h"
#include "FireMaster.h"
#include "BlueTank.h"
#include "YellowTank.h"
#include "SimpleAudioEngine.h"
#include "AppDelegate.h"
#include "replayView.h"
#include "Global.h"
#include "Debug.h"
#include <string>
#include <math.h>
#include <ctime>
#include <cstdlib>

using namespace ui;
using namespace CocosDenshion;
USING_NS_CC;

void FireMaster::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* FireMaster::createScene() {
    srand((unsigned)time(NULL));
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setAutoStep(true);
    scene->getPhysicsWorld()->setGravity(Vec2(0, -300.0f));
    auto layer = FireMaster::create();
    layer->setPhysicsWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool FireMaster::init()
{
	//super init first
	if (!Layer::init())
	{
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	//add yellow tank
	yellowTank = YellowTank::create();
	this->addChild(yellowTank, 1);
	//add blue tank
	blueTank = BlueTank::create();
	this->addChild(blueTank, 1);
	currTank = blueTank;


	addSprite();
    Global::bullets.clear();

    m_checkingRects[4].setRect(yellowTank->getBoundingBox().origin.x, yellowTank->getBoundingBox().origin.y,
        yellowTank->getBoundingBox().size.width, yellowTank->getBoundingBox().size.height - 40);
    m_checkingRects[5].setRect(blueTank->getBoundingBox().origin.x, blueTank->getBoundingBox().origin.y,
        blueTank->getBoundingBox().size.width, blueTank->getBoundingBox().size.height - 40);

    // draw debug rectangle
#ifdef DEBUG
    Color4F white(1, 1, 1, 1);

    auto rectNode = DrawNode::create();
    rectNode->drawRect(m_checkingRects[0].origin, m_checkingRects[0].origin + m_checkingRects[0].size, white);
    this->addChild(rectNode, 2);

    auto rectNode2 = DrawNode::create();
    rectNode2->drawRect(m_checkingRects[1].origin, m_checkingRects[1].origin + m_checkingRects[1].size, white);
    this->addChild(rectNode2, 2);

    auto rectNode3 = DrawNode::create();
    rectNode3->drawRect(m_checkingRects[2].origin, m_checkingRects[2].origin + m_checkingRects[2].size, white);
    this->addChild(rectNode3, 2);

    auto ground = DrawNode::create();
    ground->drawRect(m_checkingRects[3].origin, m_checkingRects[3].origin + m_checkingRects[3].size, white);
    this->addChild(ground, 2);

    auto ytank = DrawNode::create();
    ytank->drawRect(m_checkingRects[4].origin, m_checkingRects[4].origin + m_checkingRects[4].size, white);
    this->addChild(ytank, 2);

    auto btank = DrawNode::create();
    btank->drawRect(m_checkingRects[5].origin, m_checkingRects[5].origin + m_checkingRects[5].size, white);
    this->addChild(btank, 2);
#endif

	//添加调度器
	this->schedule(schedule_selector(FireMaster::updateBulletRotation), 0.1);
	this->schedule(schedule_selector(FireMaster::timer), 0.1);
	this->schedule(schedule_selector(FireMaster::updateBulletVelocity), 0.1);

	srand(unsigned(time(0))); //设置随机数种子

	nextTurn();

	return true;
}

void FireMaster::addSprite() {
	//add background
	auto bgSprite = Sprite::create("imges/bg2.png");
	bgSprite->setPosition(visibleSize / 2);
	bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width, visibleSize.height / bgSprite->getContentSize().height);
	this->addChild(bgSprite, 0);
    m_checkingRects[3].setRect(0, 0, visibleSize.width, 40);

	//add obstacle
	obstacle = Sprite::createWithSpriteFrameName("obstacle.png");
	obstacle->setAnchorPoint(Point(0.5, 0.5));
	obstacle->setPosition(visibleSize.width / 2, 120);
	obstacle->setScale(1.3, 1.4);

    //第二个矩形的起点
    float orignalx2 = visibleSize.width / 2 - 50;
    float orignaly2 = 100;
    //把两个矩形加到数组里面
    m_checkingRects[0].setRect(obstacle->getBoundingBox().origin.x, obstacle->getBoundingBox().origin.y, 188, 87);
    m_checkingRects[1].setRect(obstacle->getBoundingBox().origin.x + 20, obstacle->getBoundingBox().origin.y, 137, 130);
    m_checkingRects[2].setRect(obstacle->getBoundingBox().origin.x + 66, obstacle->getBoundingBox().origin.y, 60, 185);

	this->addChild(obstacle, 1);
    this->schedule(schedule_selector(FireMaster::updateCollision), 0.01);

    //add UI
	topUI = Sprite::createWithSpriteFrameName("topUI.png");
	topUI->setAnchorPoint(Point(0.5, 0.5));
	topUI->setPosition(visibleSize.width / 2, visibleSize.height - 80);
	topUI->setScale(1.4, 1.3);
	this->addChild(topUI, 4);

#ifdef DEBUG
    turnUI = Label::createWithTTF("Round:0 right", "fonts/arial.ttf", 36);
    turnUI->setAnchorPoint(Point(0.5, 0.5));
    turnUI->setPosition(visibleSize.width / 2, visibleSize.height - 180);
    //turnUI->setScale(1.4, 1.3);
    this->addChild(turnUI, 1);

    this->schedule(schedule_selector(FireMaster::updateTurnUI), 0.1);
#endif

	//add powerBullet_Btn1
	powerBullet_Btn1 = Button::create("imges/tank_bullet4.png", "imges/tank_bullet4.png");
	powerBullet_Btn1->setPosition(Vec2(visibleSize.width / 12, visibleSize.height * 8.1 / 10 ));
	powerBullet_Btn1->addClickEventListener(CC_CALLBACK_1(FireMaster::powerBullet_Btn1_click, this));
	this->addChild(powerBullet_Btn1, 1);


	//add fix_Btn1
	fix_Btn1 = Button::create("imges/tanks_crateRepair.png", "imges/tanks_crateRepair.png");
	fix_Btn1->setPosition(Vec2(visibleSize.width * 17 / 96, visibleSize.height * 8.1 / 10));
	fix_Btn1->setScale(0.5);
	fix_Btn1->addClickEventListener(CC_CALLBACK_1(FireMaster::fix_Btn1_click, this));
	this->addChild(fix_Btn1, 1);

	//add defence_Btn1
	defence_Btn1 = Button::create("imges/tanks_crateArmor.png", "imges/tanks_crateArmor.png");
	defence_Btn1->setPosition(Vec2(visibleSize.width * 25 / 96 , visibleSize.height * 8.1 / 10));
	defence_Btn1->setScale(0.5);
	defence_Btn1->addClickEventListener(CC_CALLBACK_1(FireMaster::defence_Btn1_click, this));
	this->addChild(defence_Btn1, 1);

	//add triAttack_Btn1
	triAttack_Btn1 = Button::create("imges/tanks_crateAmmo.png", "imges/tanks_crateAmmo.png");
	triAttack_Btn1->setPosition(Vec2(visibleSize.width * 33 / 96, visibleSize.height * 8.1 / 10));
	triAttack_Btn1->setScale(0.5);
	triAttack_Btn1->addClickEventListener(CC_CALLBACK_1(FireMaster::triAttack_Btn1_click, this));
	this->addChild(triAttack_Btn1, 1);

	//add powerBullet_Btn2
	powerBullet_Btn2 = Button::create("imges/tank_bullet4.png", "imges/tank_bullet4.png");
	powerBullet_Btn2->setPosition(Vec2(visibleSize.width *11 / 12, visibleSize.height * 8.1 / 10));
	powerBullet_Btn2->setFlipX(true);
	powerBullet_Btn2->addClickEventListener(CC_CALLBACK_1(FireMaster::powerBullet_Btn2_click, this));
	this->addChild(powerBullet_Btn2, 1);

	//add fix_Btn2
	fix_Btn2 = Button::create("imges/tanks_crateRepair.png", "imges/tanks_crateRepair.png");
	fix_Btn2->setPosition(Vec2(visibleSize.width * 79 / 96, visibleSize.height * 8.1 / 10));
	fix_Btn2->setScale(0.5);
	fix_Btn2->addClickEventListener(CC_CALLBACK_1(FireMaster::fix_Btn2_click, this));
	this->addChild(fix_Btn2, 1);

	//add defence_Btn2
	defence_Btn2 = Button::create("imges/tanks_crateArmor.png", "imges/tanks_crateArmor.png");
	defence_Btn2->setPosition(Vec2(visibleSize.width * 71 / 96, visibleSize.height * 8.1 / 10));
	defence_Btn2->setScale(0.5);
	defence_Btn2->addClickEventListener(CC_CALLBACK_1(FireMaster::defence_Btn2_click, this));
	this->addChild(defence_Btn2, 1);

	//add triAttack_Btn2
	triAttack_Btn2 = Button::create("imges/tanks_crateAmmo.png", "imges/tanks_crateAmmo.png");
	triAttack_Btn2->setPosition(Vec2(visibleSize.width * 63 / 96, visibleSize.height * 8.1 / 10));
	triAttack_Btn2->setScale(0.5);
	triAttack_Btn2->addClickEventListener(CC_CALLBACK_1(FireMaster::triAttack_Btn2_click, this));
	this->addChild(triAttack_Btn2, 1);

	//add hp1
	hp1 = Progress::create("progressBg.png","blood.png");
	hp1->setPosition(Vec2(215, 569));
	hp1->setScaleX(11.5);
	hp1->setScaleY(1.5);
	hp1->setMidpoint(Point(1, 0.5));
	this->addChild(hp1, 3);
	hp1->setProgress(100);

	//add hp2
	hp2 = Progress::create("progressBg.png", "blood.png");
	hp2->setPosition(Vec2(745, 569));
	hp2->setScaleX(11.5);
	hp2->setScaleY(1.5);
	this->addChild(hp2, 3);
	hp2->setProgress(100);

	//add windpower1
	wind1 = Progress::create("progressBg.png", "wind.png");
	wind1->setAnchorPoint(Point(1, 0.5));
	wind1->setPosition(visibleSize.width / 2 + 1, 506.5);
	wind1->setScaleX(3.5);
	wind1->setScaleY(1.5);
	wind1->setMidpoint(Point(1, 0.5));
	this->addChild(wind1, 3);
	hp1->setMidpoint(Point(1, 0.5));
	wind1->setProgress(0);

	//add windpower2
	wind2 = Progress::create("progressBg.png", "wind.png");
	wind2->setAnchorPoint(Point(0, 0.5));
	wind2->setPosition(visibleSize.width / 2 - 1, 506.5);
	wind2->setScaleX(3.5);
	wind2->setScaleY(1.5);
	this->addChild(wind2, 3);
	wind2->setProgress(0);
	
	//添加wait clock
	waitClock = ProgressTimer::create(Sprite::create("imges/waitClock.png"));
	waitClock->setType(ProgressTimer::Type::RADIAL);
	waitClock->setMidpoint(Point(0.5, 0.5));
	waitClock->setScale(1.2);
	waitClock->setVisible(false);
	this->addChild(waitClock, 3, "waitClock");

	//添加defence1防护罩
	defence1 = Sprite::createWithSpriteFrameName("defence.png");
	defence1->setAnchorPoint(Point(0.5, 0.5));
	defence1->setPosition(blueTank->getPosition().x, blueTank->getPosition().y + 10);
	defence1->setScale(1.5);
	defence1->setVisible(false);
	this->addChild(defence1, 2);

	//添加defence2防护罩
	defence2 = Sprite::createWithSpriteFrameName("defence.png");
	defence2->setAnchorPoint(Point(0.5, 0.5));
	defence2->setPosition(yellowTank->getPosition().x, yellowTank->getPosition().y + 10);
	defence2->setScale(1.5);
	defence2->setVisible(false);
	this->addChild(defence2, 2);
}

void FireMaster::refreshRandomWindPower() {
	int start = -4;
	int end = 4;
	windPower = CCRANDOM_0_1() * (end - start + 1) + start;  //产生一个从start到end间的随机数
}

void FireMaster::updateBulletVelocity(float ft) {
	int F = round(windPower);
	for (auto bullet : Global::bullets) {
		if (bullet != NULL) {	
			auto vX = bullet->getPhysicsBody()->getVelocity().x;
			auto vY = bullet->getPhysicsBody()->getVelocity().y;
			//具体加速度参数到时候再改
			bullet->getPhysicsBody()->setVelocity(Vec2(vX + F * Global::windPower, vY));
		}
	}
}

//进入下一回合
void FireMaster::nextTurn()
{
	if (Global::isGameover) return;

	++Global::turn;
	int side = Global::turn % 2;

    // 设置信号量保证只有进入到下一个回合的时候另一个tank的信号量才允许点击
    if (side == 1) {
        blueTank->isInTurn = false;
		currTank = yellowTank;
    }
    else {
        yellowTank->isInTurn = false;
		currTank = blueTank;
    }
	
	//设置风向
	refreshRandomWindPower();
	CCString* ns = CCString::createWithFormat("windPower = %f", windPower);
	CCLOG(ns->getCString());
	
	//设置风向UI
	wind1->setProgress(0);
	wind2->setProgress(0);
	if (windPower < 0) {
		wind1->setProgress(round(-windPower) * 25);
	}
	else if (windPower > 0) 
	{
		wind2->setProgress(round(windPower) * 25);
	}


	waitClock->setPercentage(100);
	//获取应放置的位置
	Vec2 pos = side == 0 ? yellowTank->getPosition() : blueTank->getPosition();
	waitClock->setPosition((Vec2(pos.x, pos.y + 100)));
	waitClock->setVisible(true);
}

//debug模式下才会开启
void FireMaster::updateTurnUI(float ft)
{
	if (Global::isGameover) return;
    auto curr_turn = std::to_string(Global::turn);
    auto side = ((Global::turn % 2) == 0) ? "right" : "left";
    auto newstr = "Round:" + curr_turn + " " + side;
    //auto newstr = "Turn: 0";
    turnUI->setString(newstr);
}

void FireMaster::updateBulletRotation(float t) {

	if (Global::isGameover) return;

	for (auto bullet : Global::bullets) {
		if (bullet != NULL) {
			//向量标准化
			auto vPoint = ccpNormalize(bullet->getPhysicsBody()->getVelocity());
			//算出旋转的弧度
			auto radians = atan2f(vPoint.y, vPoint.x);
			//将弧度转换成角度  
			float degree = CC_RADIANS_TO_DEGREES(radians);
			bullet->setRotation(-degree); //由于cocos2dx的setRotation顺时针旋转，取个负号
		}
	}
}

void FireMaster::updateCollision(float ft)
{
	if (Global::isGameover) return;

    if (!Global::bullets.empty()) { 
        auto b = Global::bullets.front();
        auto bpos = b->getPosition();
        auto bbox = b->getBoundingBox();

#ifdef BULLETDEBUG
        Color4F white(1, 1, 1, 1);
        auto rectNode = DrawNode::create();
        rectNode->drawRect(bbox.origin, bbox.origin + bbox.size, white);
        this->addChild(rectNode, 2);
#endif

		//出界判断, 运行在界外因为风力吹回来
        if (bpos.x < -100 || bpos.y < 0 || bpos.x > visibleSize.width + 100) {
            b->removeFromParentAndCleanup(true);
            Global::bullets.pop_front();
			if (Global::bullets.empty()) {
				nextTurn();
			}
            return;
        }

        bool isCrashwithBg = m_checkingRects[0].intersectsRect(bbox) || m_checkingRects[1].intersectsRect(bbox)
            || m_checkingRects[2].intersectsRect(bbox) || m_checkingRects[3].intersectsRect(bbox);
        bool isHitOpponent = (Global::turn % 2 == 1) ? m_checkingRects[4].intersectsRect(bbox) : m_checkingRects[5].intersectsRect(bbox);

        if (isCrashwithBg || isHitOpponent) {
			//爆炸音效
			SimpleAudioEngine::getInstance()->playEffect("music/explosion.wav");

			//爆炸范围
			Rect rect = Rect(bpos.x - 125, bpos.y - 125, 250, 250);
			//判断子弹和坦克中心点距离
			float distance;

			//炸到了yellowtank
			if (m_checkingRects[4].intersectsRect(rect)) {
				distance =sqrt(
					pow(bpos.x - yellowTank->getPosition().x, 2)
					+ pow(bpos.y - yellowTank->getPosition().y, 2)
					);

				CCString* ns = CCString::createWithFormat("bx = %f, tx = %f, by = %f, ty = %f", bpos.x, yellowTank->getPosition().x, bpos.y, yellowTank->getPosition().y);
				CCLOG(ns->getCString());

				float currentHp = hp2->getPercentage();

				if (distance <= 210) {
					if (!defence2->isVisible()) {
						float subHp = ((260 - distance) / 210) * b->getHurtness();

						ns = CCString::createWithFormat("distance = %f, hurt = %f", distance, subHp);
						CCLOG(ns->getCString());

						currentHp -= subHp;
					}
					else {
						defence2->setVisible(false);
					}
			   }
           
				//测试gameover
				//currentHp = 0;

				hp2->setProgress(currentHp);

				if (currentHp <= 0) {
					Global::winSide = 0;
					Global::isGameover = true;
					Gameover();
				}
			}

			//炸到了bluetank
			if (m_checkingRects[5].intersectsRect(rect)) {

				distance = sqrt(
					pow(bpos.x - blueTank->getPosition().x, 2)
					+ pow(bpos.y - blueTank->getPosition().y, 2)
				);

				CCString* ns = CCString::createWithFormat("bx = %f, tx = %f, by = %f, ty = %f", bpos.x, blueTank->getPosition().x, bpos.y, blueTank->getPosition().y);
				CCLOG(ns->getCString());
				

				float currentHp = hp1->getPercentage();

				if (distance <= 210) {
					if (!defence1->isVisible()) {
						float subHp = ((260 - distance) / 210) * b->getHurtness();

						ns = CCString::createWithFormat("distance = %f, hurt = %f", distance, subHp);
						CCLOG(ns->getCString());

						currentHp -= subHp;
					}
					else {
						defence1->setVisible(false);
					}
				}

				//测试gameover
				//currentHp = 0;

				hp1->setProgress(currentHp <= 0 ? 0 : currentHp);

				if (currentHp <= 0) {
					Global::winSide = 1;
					Global::isGameover = true;
					Gameover();
				}
			}

            b->removeFromParentAndCleanup(true);
            Global::bullets.pop_front();

            // run explosion here and clean the bullet
            auto explosion = Sprite::createWithSpriteFrameName("explosion1.png");
            explosion->setAnchorPoint(Point(0.5, 0.5));
            explosion->setPosition(bpos);
            this->addChild(explosion, 2);

            auto seq = Sequence::create(Animate::create(AnimationCache::getInstance()->getAnimation("explosionAnimation")),
                CallFunc::create(
                    [explosion, this]() {
                        explosion->removeFromParentAndCleanup(true);
						if (Global::bullets.empty()) {
							nextTurn();
						}
            }), nullptr);

            explosion->runAction(seq);
        }
    }
}

//游戏结束
void FireMaster::Gameover() {

	auto pos = Global::winSide == 1 ? blueTank->getPosition() : yellowTank->getPosition();

	for (int i = 0; i < 3; ++i) {
		booms[i] = Sprite::createWithSpriteFrameName("explosion1.png");
		booms[i]->setAnchorPoint(Point(0.5, 0.5));
		this->addChild(booms[i], 4);
		booms[i]->setVisible(false);
	}
	booms[0]->setPosition(pos.x - 20, pos.y + 20);
	booms[1]->setPosition(pos.x + 10, pos.y + 5);
	booms[2]->setPosition(pos.x - 20, pos.y + 15);

	auto exAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("explosionAnimation"));

	auto remove0 = CallFunc::create([&]() {
		this->removeChild(booms[0], true);
	});
	auto remove1 = CallFunc::create([&]() {
		this->removeChild(booms[1], true);
	});
	auto remove2 = CallFunc::create([&]() {
		this->removeChild(booms[2], true);
	});

	Sequence* s[3];
	s[0] = Sequence::create(CallFunc::create([&]() {
		booms[0]->setVisible(true);
		//爆炸音效
		SimpleAudioEngine::getInstance()->playEffect("music/explosion.wav");
	}), exAnimate, remove0, nullptr);
	s[1] = Sequence::create(DelayTime::create(0.1f), CallFunc::create([&]() {
		booms[1]->setVisible(true);
		//爆炸音效
		SimpleAudioEngine::getInstance()->playEffect("music/explosion.wav");
	}), exAnimate, remove1, nullptr);
	s[2] = Sequence::create(DelayTime::create(0.2f), CallFunc::create([&]() {
		booms[2]->setVisible(true);
		//爆炸音效
		SimpleAudioEngine::getInstance()->playEffect("music/explosion.wav");
	}), exAnimate, remove2, nullptr);

	for (int i = 0; i < 3; ++i)
		booms[i]->runAction(s[i]);

	cocos2d::Director::getInstance()->getEventDispatcher()->removeAllEventListeners();

	auto str = Sequence::create(DelayTime::create(0.6f), CallFunc::create([&]() {
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.6f, replayView::createScene()));
	}), nullptr);

	this->runAction(str);
}


//UI栏技能点击函数
void FireMaster::powerBullet_Btn1_click(Ref * sender)
{
	SimpleAudioEngine::getInstance()->playEffect("music/click.wav");
    if (Global::turn % 2 == 1 && powerBullet_Btn1->isEnabled() && Global::bullets.empty()) {
        powerBullet_Btn1->setEnabled(false);
        powerBullet_Btn1->setVisible(false);
        blueTank->setCurrBulletName("tank_bullet4.png");
    }
}
void FireMaster::fix_Btn1_click(Ref * sender)
{
	SimpleAudioEngine::getInstance()->playEffect("music/click.wav");
    if (Global::turn % 2 == 1 && fix_Btn1->isEnabled() && Global::bullets.empty()) {
        fix_Btn1->setEnabled(false);
        fix_Btn1->setVisible(false);
        float currentHp = hp1->getPercentage();
        currentHp += 20;
        hp1->setProgress(currentHp >= 100 ? 100 : currentHp);

    }
}
void FireMaster::defence_Btn1_click(Ref * sender)
{
	SimpleAudioEngine::getInstance()->playEffect("music/click.wav");
    if (Global::turn % 2 == 1 && defence_Btn1->isEnabled() && Global::bullets.empty()) {
        defence1->setVisible(true);
        defence_Btn1->runAction(FadeOut::create(0.5));
        defence_Btn1->setEnabled(false);
        defence_Btn1->setVisible(false);
    }
}
void FireMaster::triAttack_Btn1_click(Ref * sender)
{
	SimpleAudioEngine::getInstance()->playEffect("music/click.wav");
    if (Global::turn % 2 == 1 && triAttack_Btn1->isEnabled() && Global::bullets.empty()) {
        triAttack_Btn1->setEnabled(false);
        triAttack_Btn1->setVisible(false);
        triAttack_Btn1->runAction(FadeOut::create(0.5));
        blueTank->setBulletCount(3);
    }
}
void FireMaster::powerBullet_Btn2_click(Ref * sender)
{
	SimpleAudioEngine::getInstance()->playEffect("music/click.wav");
    if (Global::turn % 2 == 0 && powerBullet_Btn2->isEnabled() && Global::bullets.empty()) {
        powerBullet_Btn2->setEnabled(false);
        powerBullet_Btn2->setVisible(false);
        powerBullet_Btn2->runAction(FadeOut::create(0.5));
        yellowTank->setCurrBulletName("tank_bullet4.png");
    }
}
void FireMaster::fix_Btn2_click(Ref * sender)
{
	SimpleAudioEngine::getInstance()->playEffect("music/click.wav");
    if (Global::turn % 2 == 0 && fix_Btn2->isEnabled() && Global::bullets.empty()) {
        fix_Btn2->setEnabled(false);
        fix_Btn2->setVisible(false);
        fix_Btn2->runAction(FadeOut::create(0.5));
        float currentHp = hp2->getPercentage();
        currentHp += 20;
        hp2->setProgress(currentHp >= 100 ? 100 : currentHp);
    }
}
void FireMaster::defence_Btn2_click(Ref * sender)
{
	SimpleAudioEngine::getInstance()->playEffect("music/click.wav");
    if (Global::turn % 2 == 0 && defence_Btn2->isEnabled() && Global::bullets.empty()) {
        defence_Btn2->setEnabled(false);
        defence_Btn2->setVisible(false);
        defence2->setVisible(true);
        defence_Btn2->runAction(FadeOut::create(0.5));
    }
}
void FireMaster::triAttack_Btn2_click(Ref * sender)
{  
	SimpleAudioEngine::getInstance()->playEffect("music/click.wav");
    if (Global::turn % 2 == 0 && triAttack_Btn2->isEnabled() && Global::bullets.empty()) {
        triAttack_Btn2->setEnabled(false);
        triAttack_Btn2->setVisible(false);
        triAttack_Btn2->runAction(FadeOut::create(0.5));
        yellowTank->BaseTank::setBulletCount(3);
    }
}

//被调度的timer函数
void FireMaster::timer(float a)
{
	if (waitClock->isVisible() == true) {
		auto m = waitClock->getPercentage();
		/* 调试用
		CCString* ns = CCString::createWithFormat("m = %f", m);
		CCLOG(ns->getCString());
		*/
		if (m > 0) {
			m--;
		}
		else {
			waitClock->setVisible(false);
			nextTurn();
			return;
		}
		waitClock->setPercentage(m);
	}
}

