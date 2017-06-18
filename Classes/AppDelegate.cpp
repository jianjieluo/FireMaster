#include "AppDelegate.h"
#include "initScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect("BangBangGame", Rect(0, 0, 960, 640));
        director->setOpenGLView(glview);
    }

    director->getOpenGLView()->setDesignResolutionSize(960, 640, ResolutionPolicy::SHOW_ALL);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    FileUtils::getInstance()->addSearchPath("res");

	//load game resource
	loadGameResource();

    // create a scene. it's an autorelease object
    auto scene = initScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

void AppDelegate::loadGameResource() {
	//动画
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("imges/FireMaster.plist");
	char frameName[30];
	//坦克动画
	Animation* yellowTankAttackAnimation = Animation::create();
	Animation* blueTankAttackAnimation = Animation::create();
	for (int i = 1; i <= 4; i++) {
		sprintf(frameName, "yellowAttack%d.png", i);
		yellowTankAttackAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
		sprintf(frameName, "blueAttack%d.png", i);
		blueTankAttackAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
	}
	yellowTankAttackAnimation->setDelayPerUnit(0.02);
	blueTankAttackAnimation->setDelayPerUnit(0.02);
	AnimationCache::getInstance()->addAnimation(yellowTankAttackAnimation, "yellowTankAttackAnimation");
	AnimationCache::getInstance()->addAnimation(blueTankAttackAnimation, "blueTankAttackAnimation");

	//爆炸动画
	Animation* explosionAnimation = Animation::create();
	for (int i = 1; i <= 4; i++) {
		sprintf(frameName, "explosion%d.png", i);
		explosionAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
	}
	explosionAnimation->setDelayPerUnit(0.02);
	AnimationCache::getInstance()->addAnimation(explosionAnimation, "explosionAnimation");

    //发射的动画
    Animation* fireAnimation = Animation::create();
    for (int i = 1; i <= 4; i++) {
        sprintf(frameName, "fire%d.png", i);
        explosionAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
    }
    explosionAnimation->setDelayPerUnit(0.02);
    AnimationCache::getInstance()->addAnimation(explosionAnimation, "fireAnimation");

	
	//音乐
	SimpleAudioEngine::getInstance()->preloadEffect("music/attack.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("music/click.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("music/explosion.wav");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bgm.mp3");
	
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
