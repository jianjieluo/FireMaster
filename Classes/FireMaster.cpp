#include "cocos2d.h"
#include "FireMaster.h"
#include "BlueTank.h"
#include "YellowTank.h"
#include "SimpleAudioEngine.h"
#include "AppDelegate.h"
#include "Global.h"
#include "Debug.h"
#include <string>

using namespace ui;
using namespace CocosDenshion;
USING_NS_CC;

void FireMaster::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* FireMaster::createScene() {
    srand((unsigned)time(NULL));
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
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

	addSprite();
    Global::bullets.clear();
    //add yellow tank
    yellowTank = YellowTank::create();
    this->addChild(yellowTank, 1);
    //add blue tank
    blueTank = BlueTank::create();
    this->addChild(blueTank, 1);

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
	this->schedule(schedule_selector(FireMaster::timer), 0.1);

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

    turnUI = Label::createWithTTF("Round:0 right", "fonts/arial.ttf", 36);
    turnUI->setAnchorPoint(Point(0.5, 0.5));
    turnUI->setPosition(visibleSize.width / 2, visibleSize.height - 180);
    //turnUI->setScale(1.4, 1.3);
    this->addChild(turnUI, 1);
#ifdef DEBUG
    this->schedule(schedule_selector(FireMaster::updateTurnUI), 0.1);
#endif

	//add powerBullet_Btn1
	powerBullet_Btn1 = Button::create("imges/tank_bullet4.png", "imges/tank_bullet4.png");
	powerBullet_Btn1->setPosition(Vec2(visibleSize.width / 12, visibleSize.height * 8.1 / 10 ));
	powerBullet_Btn1->addTouchEventListener(CC_CALLBACK_1(FireMaster::powerBullet_Btn1_click, this));
	this->addChild(powerBullet_Btn1, 1);


	//add fix_Btn1
	fix_Btn1 = Button::create("imges/tanks_crateRepair.png", "imges/tanks_crateRepair.png");
	fix_Btn1->setPosition(Vec2(visibleSize.width * 17 / 96, visibleSize.height * 8.1 / 10));
	fix_Btn1->setScale(0.5);
	fix_Btn1->addTouchEventListener(CC_CALLBACK_1(FireMaster::fix_Btn1_click, this));
	this->addChild(fix_Btn1, 1);

	//add defence_Btn1
	defence_Btn1 = Button::create("imges/tanks_crateArmor.png", "imges/tanks_crateArmor.png");
	defence_Btn1->setPosition(Vec2(visibleSize.width * 25 / 96 , visibleSize.height * 8.1 / 10));
	defence_Btn1->setScale(0.5);
	defence_Btn1->addTouchEventListener(CC_CALLBACK_1(FireMaster::defence_Btn1_click, this));
	this->addChild(defence_Btn1, 1);

	//add triAttack_Btn1
	triAttack_Btn1 = Button::create("imges/tanks_crateAmmo.png", "imges/tanks_crateAmmo.png");
	triAttack_Btn1->setPosition(Vec2(visibleSize.width * 33 / 96, visibleSize.height * 8.1 / 10));
	triAttack_Btn1->setScale(0.5);
	triAttack_Btn1->addTouchEventListener(CC_CALLBACK_1(FireMaster::triAttack_Btn1_click, this));
	this->addChild(triAttack_Btn1, 1);

	//add powerBullet_Btn2
	powerBullet_Btn2 = Button::create("imges/tank_bullet4.png", "imges/tank_bullet4.png");
	powerBullet_Btn2->setPosition(Vec2(visibleSize.width *11 / 12, visibleSize.height * 8.1 / 10));
	powerBullet_Btn2->setFlipX(true);
	powerBullet_Btn2->addTouchEventListener(CC_CALLBACK_1(FireMaster::powerBullet_Btn2_click, this));
	this->addChild(powerBullet_Btn2, 1);

	//add fix_Btn2
	fix_Btn2 = Button::create("imges/tanks_crateRepair.png", "imges/tanks_crateRepair.png");
	fix_Btn2->setPosition(Vec2(visibleSize.width * 79 / 96, visibleSize.height * 8.1 / 10));
	fix_Btn2->setScale(0.5);
	fix_Btn2->addTouchEventListener(CC_CALLBACK_1(FireMaster::fix_Btn2_click, this));
	this->addChild(fix_Btn2, 1);

	//add defence_Btn2
	defence_Btn2 = Button::create("imges/tanks_crateArmor.png", "imges/tanks_crateArmor.png");
	defence_Btn2->setPosition(Vec2(visibleSize.width * 71 / 96, visibleSize.height * 8.1 / 10));
	defence_Btn2->setScale(0.5);
	defence_Btn2->addTouchEventListener(CC_CALLBACK_1(FireMaster::defence_Btn2_click, this));
	this->addChild(defence_Btn2, 1);

	//add triAttack_Btn2
	triAttack_Btn2 = Button::create("imges/tanks_crateAmmo.png", "imges/tanks_crateAmmo.png");
	triAttack_Btn2->setPosition(Vec2(visibleSize.width * 63 / 96, visibleSize.height * 8.1 / 10));
	triAttack_Btn2->setScale(0.5);
	triAttack_Btn2->addTouchEventListener(CC_CALLBACK_1(FireMaster::triAttack_Btn2_click, this));
	this->addChild(triAttack_Btn2, 1);

	//add hp1
	auto hp1 = Progress::create("progressBg.png","blood.png");
	hp1->setPosition(Vec2(215, 569));
	hp1->setScaleX(11.5);
	hp1->setScaleY(1.5);
	hp1->setMidpoint(Point(1, 0.5));
	this->addChild(hp1, 3);
	hp1->setProgress(50);


	//add hp2
	auto hp2 = Progress::create("progressBg.png", "blood.png");
	hp2->setPosition(Vec2(745, 569));
	hp2->setScaleX(11.5);
	hp2->setScaleY(1.5);
	this->addChild(hp2, 3);
	hp2->setProgress(40);

	//add windpower1
	auto wind1 = Progress::create("progressBg.png", "wind.png");
	wind1->setAnchorPoint(Point(1, 0.5));
	wind1->setPosition(visibleSize.width / 2 + 1, 506.5);
	wind1->setScaleX(3.5);
	wind1->setScaleY(1.5);
	wind1->setMidpoint(Point(1, 0.5));
	this->addChild(wind1, 3);
	hp1->setMidpoint(Point(1, 0.5));
	wind1->setProgress(0);

	//add windpower2
	auto wind2 = Progress::create("progressBg.png", "wind.png");
	wind2->setAnchorPoint(Point(0, 0.5));
	wind2->setPosition(visibleSize.width / 2 - 1, 506.5);
	wind2->setScaleX(3.5);
	wind2->setScaleY(1.5);
	this->addChild(wind2, 3);
	wind2->setProgress(0);
	
    //add fix_Btn1
    fix_Btn1 = Button::create("imges/tanks_crateRepair.png", "imges/tanks_crateRepair.png");
    fix_Btn1->setPosition(Vec2(visibleSize.width * 17 / 96, visibleSize.height * 8.1 / 10));
    fix_Btn1->setScale(0.5);
    fix_Btn1->addTouchEventListener(CC_CALLBACK_1(FireMaster::fix_Btn1_click, this));
    this->addChild(fix_Btn1, 1);

    //add defence_Btn1
    defence_Btn1 = Button::create("imges/tanks_crateArmor.png", "imges/tanks_crateArmor.png");
    defence_Btn1->setPosition(Vec2(visibleSize.width * 25 / 96, visibleSize.height * 8.1 / 10));
    defence_Btn1->setScale(0.5);
    defence_Btn1->addTouchEventListener(CC_CALLBACK_1(FireMaster::defence_Btn1_click, this));
    this->addChild(defence_Btn1, 1);

    //add triAttack_Btn1
    triAttack_Btn1 = Button::create("imges/tanks_crateAmmo.png", "imges/tanks_crateAmmo.png");
    triAttack_Btn1->setPosition(Vec2(visibleSize.width * 33 / 96, visibleSize.height * 8.1 / 10));
    triAttack_Btn1->setScale(0.5);
    triAttack_Btn1->addTouchEventListener(CC_CALLBACK_1(FireMaster::triAttack_Btn1_click, this));
    this->addChild(triAttack_Btn1, 1);

    //add powerBullet_Btn2
    powerBullet_Btn2 = Button::create("imges/tank_bullet4.png", "imges/tank_bullet4.png");
    powerBullet_Btn2->setPosition(Vec2(visibleSize.width * 11 / 12, visibleSize.height * 8.1 / 10));
    powerBullet_Btn2->setFlipX(true);
    powerBullet_Btn2->addTouchEventListener(CC_CALLBACK_1(FireMaster::powerBullet_Btn2_click, this));
    this->addChild(powerBullet_Btn2, 1);

    //add fix_Btn2
    fix_Btn2 = Button::create("imges/tanks_crateRepair.png", "imges/tanks_crateRepair.png");
    fix_Btn2->setPosition(Vec2(visibleSize.width * 79 / 96, visibleSize.height * 8.1 / 10));
    fix_Btn2->setScale(0.5);
    fix_Btn2->addTouchEventListener(CC_CALLBACK_1(FireMaster::fix_Btn2_click, this));
    this->addChild(fix_Btn2, 1);

    //add defence_Btn2
    defence_Btn2 = Button::create("imges/tanks_crateArmor.png", "imges/tanks_crateArmor.png");
    defence_Btn2->setPosition(Vec2(visibleSize.width * 71 / 96, visibleSize.height * 8.1 / 10));
    defence_Btn2->setScale(0.5);
    defence_Btn2->addTouchEventListener(CC_CALLBACK_1(FireMaster::defence_Btn2_click, this));
    this->addChild(defence_Btn2, 1);

    //add triAttack_Btn2
    triAttack_Btn2 = Button::create("imges/tanks_crateAmmo.png", "imges/tanks_crateAmmo.png");
    triAttack_Btn2->setPosition(Vec2(visibleSize.width * 63 / 96, visibleSize.height * 8.1 / 10));
    triAttack_Btn2->setScale(0.5);
    triAttack_Btn2->addTouchEventListener(CC_CALLBACK_1(FireMaster::triAttack_Btn2_click, this));
    this->addChild(triAttack_Btn2, 1);


	//add waitClock
	
	waitClock = ProgressTimer::create(Sprite::create("imges/waitClock.png"));
	waitClock->setPosition((Vec2(visibleSize.width /2, visibleSize.height /2)));
	waitClock->setType(ProgressTimer::Type::RADIAL);
	waitClock->setMidpoint(Point(0.5, 0.5));
	waitClock->setScale(1.2);
	waitClock->setPercentage(100);
	this->addChild(waitClock, 3);
	
}

void FireMaster::updateTurnUI(float ft)
{
    auto curr_turn = std::to_string(Global::turn);
    auto side = ((Global::turn % 2) == 0) ? "right" : "left";
    auto newstr = "Round:" + curr_turn + " " + side;
    //auto newstr = "Turn: 0";
    turnUI->setString(newstr);
}

void FireMaster::updateCollision(float ft)
{
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


        if (bpos.x < 0 || bpos.y < 0 || bpos.x > visibleSize.width) {
            b->removeFromParentAndCleanup(true);
            Global::bullets.pop_front();
            ++Global::turn;
            return;
        }

        bool isCrashwithBg = m_checkingRects[0].intersectsRect(bbox) || m_checkingRects[1].intersectsRect(bbox)
            || m_checkingRects[2].intersectsRect(bbox) || m_checkingRects[3].intersectsRect(bbox);
        bool isHitOpponent = (Global::turn % 2 == 1) ? m_checkingRects[4].intersectsRect(bbox) : m_checkingRects[5].intersectsRect(bbox);

        if (isCrashwithBg || isHitOpponent) {

            b->removeFromParentAndCleanup(true);
            Global::bullets.pop_front();

            // run explosion here and clean the bullet
            auto explosion = Sprite::createWithSpriteFrameName("explosion1.png");
            explosion->setAnchorPoint(Point(0.5, 0.5));
            explosion->setPosition(bpos);
            this->addChild(explosion, 2);

            auto seq = Sequence::create(Animate::create(AnimationCache::getInstance()->getAnimation("explosionAnimation")),
                CallFunc::create(
                    [explosion]() {
                explosion->removeFromParentAndCleanup(true);
                ++Global::turn;
            }), nullptr);

            explosion->runAction(seq);
        }
    }
}

//UI栏技能点击函数
void FireMaster::powerBullet_Btn1_click(Ref * sender)
{
    powerBullet_Btn1->runAction(FadeOut::create(0.5));
}
void FireMaster::fix_Btn1_click(Ref * sender)
{
    fix_Btn1->runAction(FadeOut::create(0.5));
}
void FireMaster::defence_Btn1_click(Ref * sender)
{
    defence_Btn1->runAction(FadeOut::create(0.5));
}
void FireMaster::triAttack_Btn1_click(Ref * sender)
{
    triAttack_Btn1->runAction(FadeOut::create(0.5));
}
void FireMaster::powerBullet_Btn2_click(Ref * sender)
{
    powerBullet_Btn2->runAction(FadeOut::create(0.5));
}
void FireMaster::fix_Btn2_click(Ref * sender)
{
    fix_Btn2->runAction(FadeOut::create(0.5));
}
void FireMaster::defence_Btn2_click(Ref * sender)
{
    defence_Btn2->runAction(FadeOut::create(0.5));
}
void FireMaster::triAttack_Btn2_click(Ref * sender)
{
    triAttack_Btn2->runAction(FadeOut::create(0.5));
}

  //被调度的timer函数
void FireMaster::timer(float a)
{
	n--;
	waitClock->setPercentage(n);
}

