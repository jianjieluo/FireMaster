#include "ChoosePlayerScene.h"
#include "FireMaster.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
USING_NS_CC;

using namespace ui;
using namespace cocostudio::timeline;

Scene* choosePlayerScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = choosePlayerScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool choosePlayerScene::init()
{
    //super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    num_need_choose = 0;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	//add background
	auto bgSprite = Sprite::create("imges/bg2.png");
	bgSprite->setPosition(visibleSize / 2);
	bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width, visibleSize.height / bgSprite->getContentSize().height);
	this->addChild(bgSprite, 0);
    
    // add reminder
    

    // add characters, 先用直接赋值的方式代替
    leftTankType = Global::TankType::blue;
    rightTankType = Global::TankType::yellow;
    
	//add start button
	auto startBtn = Button::create("imges/startButton3.png", "imges/startButton2.png");
	startBtn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height /2 + 30));
	startBtn->addClickEventListener(CC_CALLBACK_1(choosePlayerScene::StartBtn_click, this));
	startBtn->setScale(0.9);
	this->addChild(startBtn, 1);

    return true;
}

void choosePlayerScene::StartBtn_click(Ref * sender) {
    if (num_need_choose == 0) {
        SimpleAudioEngine::getInstance()->playEffect("music/click.wav", false);
	    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.8f, FireMaster::createScene(leftTankType, rightTankType)));
    }
}