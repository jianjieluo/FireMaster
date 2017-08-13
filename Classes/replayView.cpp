#include "replayView.h"
#include "ChoosePlayerScene.h"
#include "FireMaster.h"
#include "Global.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace CocosDenshion;
USING_NS_CC;

using namespace ui;
using namespace cocostudio::timeline;

Scene* replayView::createScene() {
  // 'scene' is an autorelease object
  auto scene = Scene::create();

  // 'layer' is an autorelease object
  auto layer = replayView::create();

  // add layer as a child to scene
  scene->addChild(layer);

  // return the scene
  return scene;
}

bool replayView::init() {
  // super init first
  if (!Layer::init()) {
    return false;
  }

  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  // add background
  Sprite* bgSprite;

  //判断获胜方创建对应背景
  if (Global::winSide) {
    bgSprite = Sprite::create("imges/yellowTankWinBg.png");
  } else {
    bgSprite = Sprite::create("imges/blueTankWinBg.png");
  }
  bgSprite->setPosition(visibleSize / 2);
  bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width,
                     visibleSize.height / bgSprite->getContentSize().height);
  this->addChild(bgSprite, 0);

  // add replay button
  auto replayBtn =
      Button::create("imges/replayBtn1.png", "imges/replayBtn2.png");
  replayBtn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 7));
  replayBtn->addClickEventListener(
      CC_CALLBACK_1(replayView::ReplayBtn_click, this));
  replayBtn->setScale(0.9);
  this->addChild(replayBtn, 1);

  return true;
}

void replayView::ReplayBtn_click(Ref* sender) {
  // init
  Global::turn = 0;
  Global::bullets.clear();
  Global::isGameover = false;
  SimpleAudioEngine::getInstance()->playEffect("music/click.wav", false);
  Director::getInstance()->replaceScene(
      TransitionCrossFade::create(0.8f, choosePlayerScene::createScene()));
}