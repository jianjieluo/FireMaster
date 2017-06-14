#include "cocos2d.h"
#include "FireMaster.h"

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

}

