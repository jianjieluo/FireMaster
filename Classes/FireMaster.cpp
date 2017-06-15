#include "cocos2d.h"
#include "FireMaster.h"
#include "ui/CocosGUI.h"
using namespace ui;
Scene* FireMaster::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = FireMaster::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
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
	return true;
}

void FireMaster::addSprite() {
	//add background
	auto bgSprite = Sprite::create("imges/bg2.png");
	bgSprite->setPosition(visibleSize / 2);
	bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width, visibleSize.height / bgSprite->getContentSize().height);
	this->addChild(bgSprite, 0);

	auto tankScaleSize = 0.7; //设置坦克缩放大小
	//add yellow tank
	yellowTank = Sprite::createWithSpriteFrameName("yellowAttack1.png");
	yellowTank->setAnchorPoint(Point(0.5, 0.5));
	yellowTank->setPosition(visibleSize.width - 100, 100);
	yellowTank->setScale(tankScaleSize);
	yellowTank->setFlipX(true);
	//车动画，到时候删，现在先在这运行看看效果
	auto yTankAttackAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("yellowTankAttackAnimation"));
	yellowTank->runAction(RepeatForever::create(yTankAttackAnimate));
	this->addChild(yellowTank, 1);

	//explosion动画展示，到时候删
	auto explosion = Sprite::createWithSpriteFrameName("explosion1.png");
	explosion->setAnchorPoint(Point(0.5, 0.5));
	explosion->setPosition(visibleSize.width/2, visibleSize.height/2);
	auto explosionAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("explosionAnimation"));
	explosion->runAction(RepeatForever::create(explosionAnimate));
	this->addChild(explosion, 1);

	//add blue tank
	blueTank = Sprite::createWithSpriteFrameName("blueAttack1.png");
	blueTank->setAnchorPoint(Point(0.5, 0.5));
	blueTank->setPosition(100, 100);
	blueTank->setScale(tankScaleSize);
	this->addChild(blueTank, 1); 

	//add obstacle
	obstacle = Sprite::createWithSpriteFrameName("obstacle.png");
	obstacle->setAnchorPoint(Point(0.5, 0.5));
	obstacle->setPosition(visibleSize.width / 2, 120);
	obstacle->setScale(1.3, 1.4);
	this->addChild(obstacle, 1);

    //add UI
	topUI = Sprite::createWithSpriteFrameName("topUI.png");
	topUI->setAnchorPoint(Point(0.5, 0.5));
	topUI->setPosition(visibleSize.width / 2, visibleSize.height - 80);
	topUI->setScale(1.4, 1.3);
	this->addChild(topUI, 1);

	//add tank_bullet4_1
    auto tank_bullet4_1 = Button::create("imges/tank_bullet4.png", "imges/tank_bullet4.png");
	tank_bullet4_1->setPosition(Vec2(visibleSize.width / 12, visibleSize.height * 8.1 / 10 ));
	tank_bullet4_1->addTouchEventListener(CC_CALLBACK_1(FireMaster::tank_bullet4_1_click, this));
	this->addChild(tank_bullet4_1, 1);

	//add tanks_crateRepair_1
	auto tanks_crateRepair_1 = Button::create("imges/tanks_crateRepair.png", "imges/tanks_crateRepair.png");
	tanks_crateRepair_1->setPosition(Vec2(visibleSize.width * 17 / 96, visibleSize.height * 8.1 / 10));
	tanks_crateRepair_1->setScale(0.5);
	tanks_crateRepair_1->addTouchEventListener(CC_CALLBACK_1(FireMaster::tanks_crateRepair_1_click, this));
	this->addChild(tanks_crateRepair_1, 1);

	//add tanks_crateArmor_1
	auto tanks_crateArmor_1 = Button::create("imges/tanks_crateArmor.png", "imges/tanks_crateArmor.png");
	tanks_crateArmor_1->setPosition(Vec2(visibleSize.width * 25 / 96 , visibleSize.height * 8.1 / 10));
	tanks_crateArmor_1->setScale(0.5);
	tanks_crateArmor_1->addTouchEventListener(CC_CALLBACK_1(FireMaster::tanks_crateArmor_1_click, this));
	this->addChild(tanks_crateArmor_1, 1);

	//add tanks_crateAmmo_1
	auto tanks_crateAmmo_1 = Button::create("imges/tanks_crateAmmo.png", "imges/tanks_crateAmmo.png");
	tanks_crateAmmo_1->setPosition(Vec2(visibleSize.width * 33 / 96, visibleSize.height * 8.1 / 10));
	tanks_crateAmmo_1->setScale(0.5);
	tanks_crateAmmo_1->addTouchEventListener(CC_CALLBACK_1(FireMaster::tanks_crateAmmo_1_click, this));
	this->addChild(tanks_crateAmmo_1, 1);

	//add tank_bullet4_2
	auto tank_bullet4_2 = Button::create("imges/tank_bullet4.png", "imges/tank_bullet4.png");
	tank_bullet4_2->setPosition(Vec2(visibleSize.width *11 / 12, visibleSize.height * 8.1 / 10));
	tank_bullet4_2->setFlipX(true);
	tank_bullet4_2->addTouchEventListener(CC_CALLBACK_1(FireMaster::tank_bullet4_1_click, this));
	this->addChild(tank_bullet4_2, 1);

	//add tanks_crateRepair_2
	auto tanks_crateRepair_2 = Button::create("imges/tanks_crateRepair.png", "imges/tanks_crateRepair.png");
	tanks_crateRepair_2->setPosition(Vec2(visibleSize.width * 79 / 96, visibleSize.height * 8.1 / 10));
	tanks_crateRepair_2->setScale(0.5);
	tanks_crateRepair_2->addTouchEventListener(CC_CALLBACK_1(FireMaster::tanks_crateRepair_1_click, this));
	this->addChild(tanks_crateRepair_2, 1);

	//add tanks_crateArmor_2
	auto tanks_crateArmor_2 = Button::create("imges/tanks_crateArmor.png", "imges/tanks_crateArmor.png");
	tanks_crateArmor_2->setPosition(Vec2(visibleSize.width * 71 / 96, visibleSize.height * 8.1 / 10));
	tanks_crateArmor_2->setScale(0.5);
	tanks_crateArmor_2->addTouchEventListener(CC_CALLBACK_1(FireMaster::tanks_crateArmor_1_click, this));
	this->addChild(tanks_crateArmor_2, 1);

	//add tanks_crateAmmo_2
	auto tanks_crateAmmo_2 = Button::create("imges/tanks_crateAmmo.png", "imges/tanks_crateAmmo.png");
	tanks_crateAmmo_2->setPosition(Vec2(visibleSize.width * 63 / 96, visibleSize.height * 8.1 / 10));
	tanks_crateAmmo_2->setScale(0.5);
	tanks_crateAmmo_2->addTouchEventListener(CC_CALLBACK_1(FireMaster::tanks_crateAmmo_1_click, this));
	this->addChild(tanks_crateAmmo_2, 1);
}

void FireMaster::tank_bullet4_1_click(Ref * sender)
{
}

void FireMaster::tanks_crateRepair_1_click(Ref * sender)
{
}

void FireMaster::tanks_crateArmor_1_click(Ref * sender)
{
}

void FireMaster::tanks_crateAmmo_1_click(Ref * sender)
{
}





void FireMaster::tank_bullet4_2_click(Ref * sender)
{
}

void FireMaster::tanks_crateRepair_2_click(Ref * sender)
{
}

void FireMaster::tanks_crateArmor_2_click(Ref * sender)
{
}

void FireMaster::tanks_crateAmmo_2_click(Ref * sender)
{
}




