#include "PauseLayer.h"
#include "StartScene.h"
#include "FishingJoyData.h"

PauseLayer::PauseLayer()
	:m_pDelegate(nullptr),m_pContinueBtn(nullptr)
	,m_pMainBtn(nullptr),m_pTollgateBtn(nullptr)
	,m_pMusicBtn(nullptr),m_pChunkBtn(nullptr)
{
}
PauseLayer::~PauseLayer()
{
}
bool PauseLayer::init()
{
	auto node = ui::UIWidgetManager::getInstance()->createWidgetsWithXml("scene/PauseLayer.xml");
	this->addChild(node);
	//获取各种有用的控件
	m_pContinueBtn = node->getChildByName<ui::Button*>("continue_btn");
	m_pContinueBtn->addClickEventListener(SDL_CALLBACK_1(PauseLayer::continueBtnCallback,this));

	m_pMainBtn = node->getChildByName<ui::Button*>("main_btn");
	m_pMainBtn->addClickEventListener(SDL_CALLBACK_1(PauseLayer::mainBtnCallback,this));

	m_pTollgateBtn = node->getChildByName<ui::Button*>("choose_tollgate_btn");
	m_pTollgateBtn->addClickEventListener(SDL_CALLBACK_1(PauseLayer::tollgateBtnCallback,this));

	m_pMusicBtn = node->getChildByName<ui::CheckBox*>("music_btn");
	m_pMusicBtn->addClickEventListener(SDL_CALLBACK_1(PauseLayer::musicBtnCallback,this));

	m_pChunkBtn = node->getChildByName<ui::CheckBox*>("chunk_btn");
	m_pChunkBtn->addClickEventListener(SDL_CALLBACK_1(PauseLayer::chunkBtnCallback,this));

	return true;
}

void PauseLayer::updateUI()
{
	bool bPlayingMusic = FishingJoyData::getInstance()->isPlayingMusic();
	bool bPlayingChunk = FishingJoyData::getInstance()->isPlayingChunk();
	//这个是因为使用的Checkbox，所以得取反
	m_pMusicBtn->setSelected(!bPlayingMusic);
	m_pChunkBtn->setSelected(!bPlayingChunk);
}

void PauseLayer::setDelegate(PauseLayerDelegate*pDelegate)
{
	m_pDelegate = pDelegate;
}

void PauseLayer::continueBtnCallback(Object*obj)
{
	m_pDelegate->continueGame();
}

void PauseLayer::mainBtnCallback(Object*obj)
{
	m_pDelegate->turnStartLayer();
}

void PauseLayer::tollgateBtnCallback(Object*sender)
{
	m_pDelegate->chooseTollgate();
}

void PauseLayer::musicBtnCallback(Object*sender)
{
	m_pDelegate->operateMusic();
}

void PauseLayer::chunkBtnCallback(Object*sender)
{
	m_pDelegate->operateChunk();
}