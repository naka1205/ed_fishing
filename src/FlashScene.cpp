#include "FlashScene.h"
#include "StaticData.h"
#include "GB2ShapeCacheX.h"
#include "StartScene.h"
#include "B2EntityFactory.h"

FlashScene::FlashScene()
{
}

FlashScene::~FlashScene()
{
}

bool FlashScene::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// 加载背景
	Sprite *bg = Sprite::create("logo.png");
	bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);

	this->addChild(bg);
	// 下一帧加载一些静态资源
	this->scheduleOnce(SDL_CALLBACK_1(FlashScene::preload, this), "preload", 0.1f);

	return true;
}

void FlashScene::preload(float dt)
{
	// 加载一些无法放到工作线程的资源
	StaticData::getInstance();
	// 加载鱼的刚体
	GB2ShapeCache::getInstance()->addShapesWithFile(STATIC_DATA_STRING("box2d_filepath"));
	// 进入开始游戏场景
	StartScene *scene = StartScene::create();

	Director::getInstance()->replaceScene(scene);
}