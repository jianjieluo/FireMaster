#include "introductionPage.h"
#include "ChoosePlayerScene.h"
#include "FireMaster.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

using namespace ui;

Scene* introductionPage::createScene() {
  // 'scene' is an autorelease object
  auto scene = Scene::create();

  // 'layer' is an autorelease object
  auto layer = introductionPage::create();

  // add layer as a child to scene
  scene->addChild(layer);

  // return the scene
  return scene;
}

bool introductionPage::init() {
  // super init first
  if (!Layer::init()) {
    return false;
  }

  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  // add background
  auto bgSprite = Sprite::create("imges/introductionBg.png");
  bgSprite->setPosition(visibleSize / 2);
  bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width,
                     visibleSize.height / bgSprite->getContentSize().height);
  this->addChild(bgSprite, 0);

  // add button
  auto startBtn =
      Button::create("imges/startButton5.png", "imges/startButton4.png");
  startBtn->setScale(1.5);
  startBtn->setPosition(
      Vec2(visibleSize.width / 2, visibleSize.height * 1 / 10));
  startBtn->addClickEventListener(
      CC_CALLBACK_1(introductionPage::StartBtn_click, this));
  this->addChild(startBtn, 1);

  return true;
}

void introductionPage::StartBtn_click(Ref* sender) {
  SimpleAudioEngine::getInstance()->playEffect("music/click.wav", false);
  Director::getInstance()->replaceScene(
      TransitionCrossFade::create(0.8f, choosePlayerScene::createScene()));
}
