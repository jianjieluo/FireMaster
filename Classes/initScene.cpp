#include "initScene.h"
#include "ChoosePlayerScene.h"
#include "FireMaster.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "introductionPage.h"
#include "ui/CocosGUI.h"

using namespace CocosDenshion;
USING_NS_CC;

using namespace ui;
using namespace cocostudio::timeline;

Scene* initScene::createScene() {
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
bool initScene::init() {
  // super init first
  if (!Layer::init()) {
    return false;
  }

  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  // add background
  auto bgSprite = Sprite::create("imges/bg1.png");
  bgSprite->setPosition(visibleSize / 2);
  bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width,
                     visibleSize.height / bgSprite->getContentSize().height);
  this->addChild(bgSprite, 0);

  // add start button
  auto startBtn =
      Button::create("imges/startButton3.png", "imges/startButton2.png");
  startBtn->setPosition(
      Vec2(visibleSize.width / 2, visibleSize.height / 2 + 30));
  startBtn->addClickEventListener(
      CC_CALLBACK_1(initScene::StartBtn_click, this));
  startBtn->setScale(0.9);
  this->addChild(startBtn, 1);

  // add introduction button
  auto introductionBtn =
      Button::create("imges/howToPlayBtn2.png", "imges/howToPlayBtn.png");
  introductionBtn->setPosition(
      Vec2(visibleSize.width / 2, visibleSize.height / 2 - 80));
  introductionBtn->addClickEventListener(
      CC_CALLBACK_1(initScene::introductionBtn_click, this));
  introductionBtn->setScale(1.3, 1.2);
  this->addChild(introductionBtn, 1);

  // play bgm
  SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.mp3", true);
  SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.1f);
  return true;
}

void initScene::StartBtn_click(Ref* sender) {
  SimpleAudioEngine::getInstance()->playEffect("music/click.wav", false);
  Director::getInstance()->replaceScene(
      TransitionCrossFade::create(0.8f, choosePlayerScene::createScene()));
}

void initScene::introductionBtn_click(Ref* sender) {
  SimpleAudioEngine::getInstance()->playEffect("music/click.wav", false);
  Director::getInstance()->replaceScene(
      TransitionCrossFade::create(0.8f, introductionPage::createScene()));
}