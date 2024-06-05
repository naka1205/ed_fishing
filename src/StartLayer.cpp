#include "StartLayer.h"
#include "FishingJoyData.h"

StartLayer::StartLayer()
	:m_pStartBtn(nullptr),m_pHelpBtn(nullptr)
	,m_pExitBtn(nullptr),m_pDelegate(nullptr)
	,m_pMusicBtn(nullptr),m_pEffectBtn(nullptr)
{
}

StartLayer::~StartLayer()
{
}

bool StartLayer::init()
{
	//添加外部文件
	string xml = "scene/StartLayer.xml";
	auto node = ui::UIWidgetManager::getInstance()->createWidgetsWithXml(xml);
	this->addChild(node);
	//获取各种有用的信息
	m_pStartBtn = node->getChildByName<ui::Button*>("start_btn");
	m_pHelpBtn = node->getChildByName<ui::Button*>("help_btn");
	m_pExitBtn = node->getChildByName<ui::Button*>("exit_btn");

	m_pStartBtn->addClickEventListener(SDL_CALLBACK_1(StartLayer::startGame,this));
	m_pHelpBtn->addClickEventListener(SDL_CALLBACK_1(StartLayer::helpGame,this));
	m_pExitBtn->addClickEventListener(SDL_CALLBACK_1(StartLayer::exitGame,this));
	//添加音效和音乐
	m_pMusicBtn = node->getChildByName<ui::CheckBox*>("music_btn");//ui::CheckBox::create("music00.png","music01.png");
	m_pMusicBtn->addEventListener(SDL_CALLBACK_2(StartLayer::changeMusic,this));

	m_pEffectBtn = node->getChildByName<ui::CheckBox*>("chunk_btn");//ui::CheckBox::create("sound00.png","sound01.png");
	m_pEffectBtn->addEventListener(SDL_CALLBACK_2(StartLayer::changeSound,this));

	return true;
}

void StartLayer::setActive(bool active)
{
	this->setVisible(active);

	m_pStartBtn->setTouchEnabled(active);
	m_pHelpBtn->setTouchEnabled(active);
	m_pExitBtn->setTouchEnabled(active);
	m_pMusicBtn->setTouchEnabled(active);
	m_pEffectBtn->setTouchEnabled(active);
}

void StartLayer::setDelegate(StartLayerDelegate*pDelegate)
{
	m_pDelegate = pDelegate;
}

void StartLayer::updateUI()
{
	bool bPlayingMusic = FishingJoyData::getInstance()->isPlayingMusic();
	bool bPlayingChunk = FishingJoyData::getInstance()->isPlayingChunk();
	//这个是因为使用的Checkbox，所以得取反
	m_pMusicBtn->setSelected(!bPlayingMusic);
	m_pEffectBtn->setSelected(!bPlayingChunk);
}

void StartLayer::startGame(Object*)
{
	m_pDelegate->startGame();
}

void StartLayer::helpGame(Object*sender)
{
	m_pDelegate->helpGame();
}

void StartLayer::exitGame(Object*sender)
{
	m_pDelegate->exitGame();
}

void StartLayer::changeMusic(Object*,ui::CheckBox::EventType type)
{
	m_pDelegate->changeMusic();
}

void StartLayer::changeSound(Object*,ui::CheckBox::EventType type)
{
	m_pDelegate->changeSound();
}