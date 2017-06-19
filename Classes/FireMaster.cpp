#include "cocos2d.h"
#include "FireMaster.h"
#include "SimpleAudioEngine.h"
#include "AppDelegate.h"

using namespace CocosDenshion;

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
	//this->addChild(bgSprite, 0);

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
	this->addChild(topUI, 4);

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
	//测试
	hp1->setProgress(50);


	//add hp2
	auto hp2 = Progress::create("progressBg.png", "blood.png");
	hp2->setPosition(Vec2(745, 569));
	hp2->setScaleX(11.5);
	hp2->setScaleY(1.5);
	this->addChild(hp2, 3);
	//测试
	hp2->setProgress(40);

	//add windpower1
	auto wind1 = Progress::create("progressBg.png", "wind.png");
	wind1->setAnchorPoint(Point(1, 0.5));
	wind1->setPosition(visibleSize.width / 2 + 1, 506.5); //+1 去除中间裂痕
	wind1->setScaleX(3.5);
	wind1->setScaleY(1.5);
	wind1->setMidpoint(Point(1, 0.5));
	this->addChild(wind1, 3);
	hp1->setMidpoint(Point(1, 0.5));
	//测试
	wind1->setProgress(0);

	//add windpower2
	auto wind2 = Progress::create("progressBg.png", "wind.png");
	wind2->setAnchorPoint(Point(0, 0.5));
	wind2->setPosition(visibleSize.width / 2 - 1, 506.5);
	wind2->setScaleX(3.5);
	wind2->setScaleY(1.5);
	this->addChild(wind2, 3);
	//测试
	wind2->setProgress(0);
	
	
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




