#include "StartScene.h"
#include "GameScene.h"
#include "LevelChooseLayer.h"
#include "LoadingLayer.h"
#include "FishingJoyData.h"
#include "Toast.h"

StartScene::StartScene()
	:m_pStartLayer(nullptr),m_pLevelChooseLayer(nullptr)
	,m_pHelpLayer(nullptr)
{
}
StartScene::~StartScene()
{
}
bool StartScene::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//加载图片资源
	Director::getInstance()->getSpriteFrameCache()->addSpriteFramesWithFile("start_ui.xml");
	//加入背景
	Sprite*bg = Sprite::create("start.png");
	bg->setAnchorPoint(Point::ZERO);
	this->addChild(bg);
	//开始界面
	m_pStartLayer = StartLayer::create();
	m_pStartLayer->setDelegate(this);
	this->addChild(m_pStartLayer);
	//关卡选择界面
	m_pLevelChooseLayer = LevelChooseLayer::create();
	m_pLevelChooseLayer->setPosition(0,-visibleSize.height);
	m_pLevelChooseLayer->setVisible(false);
	m_pLevelChooseLayer->setDelegate(this);
	this->addChild(m_pLevelChooseLayer);
	//帮助界面
	m_pHelpLayer = HelpLayer::create();
	m_pHelpLayer->setDelegate(this);
	m_pHelpLayer->setVisible(false);
	m_pHelpLayer->setPosition(0,-visibleSize.height);

	this->addChild(m_pHelpLayer);
	//更新ui
	m_pStartLayer->updateUI();
	m_pLevelChooseLayer->updateUI();
	//加载音乐
	SoundManager::getInstance()->addSoundAsync("sound/bgm_start_scene.ogg",Music::TYPE ,SDL_CALLBACK_2(StartScene::asyncLoadMusicCallback,this));

	return true;
}

void StartScene::enterTollgateLayer()
{
	this->startGame();
}

void StartScene::startGame()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_pStartLayer->setActive(false);

	m_pLevelChooseLayer->setVisible(true);
	m_pLevelChooseLayer->setPosition(0,-visibleSize.height);
	//动画显示选择界面
	MoveBy*move = MoveBy::create(0.5f,Point(0,visibleSize.height));
	EaseExponentialOut*action = EaseExponentialOut::create(move);

	m_pLevelChooseLayer->runAction(action);
}

void StartScene::helpGame()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_pStartLayer->setActive(false);

	m_pHelpLayer->setVisible(true);
	m_pHelpLayer->setPosition(0,-visibleSize.height);
	//动画显示选择界面
	MoveBy*move = MoveBy::create(0.5f,Point(0,visibleSize.height));
	EaseExponentialOut*action = EaseExponentialOut::create(move);

	m_pHelpLayer->runAction(action);
}

void StartScene::exitGame()
{
	Director::getInstance()->setRunning(false);
}

void StartScene::changeMusic()
{
	auto soundManager = SoundManager::getInstance();
	bool playingMusic = FishingJoyData::getInstance()->isPlayingMusic();
	playingMusic = !playingMusic;
	//将要播放音乐
	if(playingMusic)
		soundManager->playBackgroundMusic("sound/bgm_start_scene.ogg",-1);
	else
		soundManager->stopBackgroundMusic();
	//回写
	FishingJoyData::getInstance()->setPlayingMusic(playingMusic);
}

void StartScene::changeSound()
{
	bool bPlayingChunk = FishingJoyData::getInstance()->isPlayingChunk();
	bPlayingChunk = ! bPlayingChunk;
	//回写
	FishingJoyData::getInstance()->setPlayingChunk(bPlayingChunk);
}

void StartScene::asyncLoadMusicCallback(Object*sound,const std::string&type)
{
	//播放背景音乐
	if(type == Music::TYPE)
	{
		Music*music = static_cast<Music*>(sound);
		bool bPlayingMusic = FishingJoyData::getInstance()->isPlayingMusic();

		if (bPlayingMusic)
			music->playMusic(-1);
	}
}

void StartScene::closeLevelChooseLevel()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_pStartLayer->setActive(true);

	//动画显示选择界面
	MoveBy*move = MoveBy::create(0.5f,Point(0,-visibleSize.height));
	EaseExponentialOut*action = EaseExponentialOut::create(move);

	m_pLevelChooseLayer->runAction(action);
}

void StartScene::closeHelpLayer()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_pStartLayer->setActive(true);

	//动画显示选择界面
	MoveBy*move = MoveBy::create(0.5f,Point(0,-visibleSize.height));
	EaseExponentialOut*action = EaseExponentialOut::create(move);

	m_pHelpLayer->runAction(action);
}

void StartScene::enterLoadingLayer()
{
	//进行资源的卸载 todo
	//进行一些必要的判断
	int tollgateIndex = m_pLevelChooseLayer->getTollgateIndex();
	int needfulGold = 0;
	int allGold = FishingJoyData::getInstance()->getGold();
	//todo 获取进入此关卡所需要的金币
	if(tollgateIndex == 0)
		needfulGold = 10;
	else if(tollgateIndex == 1)
		needfulGold = 50;
	else if (tollgateIndex == 2)
		needfulGold = 100;
	//金币是否足够
	if(allGold >= needfulGold)
	{
		//减少金币并回写
		FishingJoyData::getInstance()->setGold(allGold - needfulGold);
		//更新界面
		m_pLevelChooseLayer->updateUI();
		//写入到文档中
		FishingJoyData::getInstance()->setTollgateIndex(tollgateIndex);

		auto scene = LoadingLayer::createScene();
		Director::getInstance()->replaceScene(scene);
		//成功进入
		Sprite*sprite = Sprite::createWithSpriteFrameName("UI_enterSuccess.png");

		Toast::makeSprite(scene,sprite,1.5f);
	}
	else//进入失败
	{
		Sprite*sprite = Sprite::createWithSpriteFrameName("UI_enterDefault.png");
		Toast::makeSprite(this,sprite,1.5f);
	}
}