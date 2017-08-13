#include "ChoosePlayerScene.h"
#include "FireMaster.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace CocosDenshion;
USING_NS_CC;

using namespace ui;
using namespace cocostudio::timeline;

Scene* choosePlayerScene::createScene() {
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
bool choosePlayerScene::init() {
  // super init first
  if (!Layer::init()) {
    return false;
  }

  //EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
  //listener->setSwallowTouches(true);
  //listener->onTouchBegan = CC_CALLBACK_2(choosePlayerScene::onTouchBegan, this);
  //Director::getInstance()
  //    ->getEventDispatcher()
  //    ->addEventListenerWithSceneGraphPriority(listener, this);

  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  // add background
  auto bgSprite = Sprite::create("imges/bg2.png");
  bgSprite->setPosition(visibleSize / 2);
  bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width,
                     visibleSize.height / bgSprite->getContentSize().height);
  this->addChild(bgSprite, 0);

  // add menuitems
  auto bluelabel = Label::createWithTTF("blue", "fonts/Marker Felt.ttf", 28);
  auto bluemenuitem = MenuItemLabel::create(
      bluelabel, CC_CALLBACK_1(choosePlayerScene::choose_click, this,
                               Global::TankType::blue));
  bluemenuitem->setPosition(visibleSize.width * 0.2, 128);

  auto yellowlabel =
      Label::createWithTTF("yellow", "fonts/Marker Felt.ttf", 28);
  auto yellowmenuitem = MenuItemLabel::create(
      yellowlabel, CC_CALLBACK_1(choosePlayerScene::choose_click, this,
                                 Global::TankType::yellow));
  yellowmenuitem->setPosition(visibleSize.width * 0.5, 128);

  auto oklabel = Label::createWithTTF("ok", "fonts/Marker Felt.ttf", 28);
  auto okmenuitem = MenuItemLabel::create(
      oklabel, CC_CALLBACK_1(choosePlayerScene::ok_click, this));
  okmenuitem->setPosition(visibleSize.width * 0.8, 128);

  auto menu = Menu::create(bluemenuitem, yellowmenuitem, okmenuitem, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  num_need_choose = 2;
  currTankType = Global::TankType::none;

  // add reminder
  tipsUI = Label::createWithTTF("Choose a tank for left position,\n then click ok to confirm", "fonts/arial.ttf", 28);
  tipsUI->setAnchorPoint(Point(0.5, 0.5));
  tipsUI->setPosition(visibleSize.width / 2, visibleSize.height - 180);
  this->addChild(tipsUI, 1);

  detailUI = Label::createWithTTF("You need to choose a specific tank and get its details", "fonts/arial.ttf", 28);
  detailUI->setAnchorPoint(Point(0.5, 0.5));
  detailUI->setPosition(visibleSize.width / 2, visibleSize.height - 250);
  this->addChild(detailUI, 1);

  this->schedule(schedule_selector(choosePlayerScene::update), 0.2);

  // add characters, 先用直接赋值的方式代替
  // leftTankType = Global::TankType::blue;
  // rightTankType = Global::TankType::yellow;

  return true;
}

void choosePlayerScene::update(float ft) {
  std::string tip = "Tips: ";
  std::string detail = "Tank Details: ";

  switch (num_need_choose) {
    case 2:
      tip += "Choose a tank for left position,\n then click ok to confirm";
      break;
    case 1:
      tip += "Choose a tank for right position,\n then click ok to confirm";
      break;
    case 0:
      tip += "Click ok to play the game";
      break;
  }

  switch (currTankType) {
    case Global::TankType::blue:
      detail += "Blue Tank! blablabla...";
      break;
    case Global::TankType::yellow:
      detail += "Yellow Tank! blablabla...";
      break;
    case Global::TankType::none:
      detail += "You need to choose a specific tank and get its details";
      break;
  }

  tipsUI->setString(tip);
  detailUI->setString(detail);
}

void choosePlayerScene::choose_click(Ref* sender, Global::TankType t_type) {
  currTankType = t_type;
}

void choosePlayerScene::ok_click(Ref* sender) {
  if (currTankType == Global::TankType::none) return;

  SimpleAudioEngine::getInstance()->playEffect("music/click.wav", false);

  switch (num_need_choose) {
    case 2:
      leftTankType = currTankType;
      --num_need_choose;
      break;
    case 1:
      rightTankType = currTankType;
      --num_need_choose;
      break;
    case 0:
      Director::getInstance()->replaceScene(TransitionCrossFade::create(
          0.8f, FireMaster::createScene(leftTankType, rightTankType)));
      break;
  }
}