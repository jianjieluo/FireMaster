#include "initScene.h"
#include "FireMaster.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
USING_NS_CC;

using namespace ui;
using namespace cocostudio::timeline;

Scene* initScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = initScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool initScene::init()
{
    //super init first
    if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	//add background
	auto bgSprite = Sprite::create("imges/bg1.png");
	bgSprite->setPosition(visibleSize / 2);
	bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width, visibleSize.height / bgSprite->getContentSize().height);
	this->addChild(bgSprite, 0);
	//add start button
	auto startBtn = Button::create("imges/startButton3.png", "imges/startButton2.png");
	startBtn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height /2));
	startBtn->addTouchEventListener(CC_CALLBACK_1(initScene::StartBtn_click, this));
	//startBtn->onTouchEnded = CC_CALLBACK_1(initScene::StartBtn_click, this);
	this->addChild(startBtn, 1);
	
	//play bgm
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.mp3", true);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.01f);
    return true;
}

void initScene::StartBtn_click(Ref * sender) {
	Director::getInstance()->replaceScene(TransitionCrossFade::create(0.8f, FireMaster::createScene()));
	SimpleAudioEngine::getInstance()->playEffect("music/click.wav",false);
}
