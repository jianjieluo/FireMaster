#include "cocos2d.h"
#include "FireMaster.h"
#include "BlueTank.h"
#include "YellowTank.h"
#include "SimpleAudioEngine.h"
#include "AppDelegate.h"
#include "Global.h"

#include <string>

using namespace ui;
using namespace CocosDenshion;
USING_NS_CC;

void FireMaster::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* FireMaster::createScene() {
    srand((unsigned)time(NULL));
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);ddd
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
    //add yellow tank
    yellowTank = YellowTank::create();
    this->addChild(yellowTank, 1);
    //add blue tank
    blueTank = BlueTank::create();
    this->addChild(blueTank, 1);

	return true;
}

void FireMaster::addSprite() {
	//add background
	auto bgSprite = Sprite::create("imges/bg2.png");
	bgSprite->setPosition(visibleSize / 2);
	bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width, visibleSize.height / bgSprite->getContentSize().height);
	this->addChild(bgSprite, 0);

	//explosion动画展示，到时候删
	//auto explosion = Sprite::createWithSpriteFrameName("explosion1.png");
	//explosion->setAnchorPoint(Point(0.5, 0.5));
	//explosion->setPosition(visibleSize.width/2, visibleSize.height/2);
	//auto explosionAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("explosionAnimation"));
	//explosion->runAction(RepeatForever::create(explosionAnimate));
	//this->addChild(explosion, 1);

	//add obstacle
	obstacle = Sprite::createWithSpriteFrameName("obstacle.png");
	obstacle->setAnchorPoint(Point(0.5, 0.5));
	obstacle->setPosition(visibleSize.width / 2, 120);
	obstacle->setScale(1.3, 1.4);

    auto obstaclebody = PhysicsBody::createBox(obstacle->getContentSize(), PhysicsMaterial(100.0f, 0.0f, 1.0f));
    obstaclebody->setCategoryBitmask(0xFFFFFFFF);
    obstaclebody->setCollisionBitmask(0xFFFFFFFF);
    obstaclebody->setContactTestBitmask(0xFFFFFFFF);
    obstaclebody->setDynamic(false);
    obstacle->setPhysicsBody(obstaclebody);

	this->addChild(obstacle, 1);

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

	//add powerBullet_Btn1
	powerBullet_Btn1 = Button::create("imges/tank_bullet4.png", "imges/tank_bullet4.png");
	powerBullet_Btn1->setPosition(Vec2(visibleSize.width / 12, visibleSize.height * 8.1 / 10 ));
	powerBullet_Btn1->addTouchEventListener(CC_CALLBACK_1(FireMaster::powerBullet_Btn1_click, this));
	this->addChild(powerBullet_Btn1, 1);
    this->schedule(schedule_selector(FireMaster::updateTurnUI), 0.1);

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
}


void FireMaster::updateTurnUI(float ft)
{

    auto curr_turn = std::to_string(Global::turn);
    auto side = ((Global::turn % 2) == 0) ? "right" : "left";
    auto newstr = "Round:" + curr_turn + " " + side;
    //auto newstr = "Turn: 0";
    turnUI->setString(newstr);
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
