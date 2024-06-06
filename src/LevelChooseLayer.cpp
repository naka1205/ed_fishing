#include "LevelChooseLayer.h"
#include "FishingJoyData.h"

LevelChooseLayer::LevelChooseLayer()
	:m_pCloseBtn(nullptr),m_pDelegate(nullptr)
	,m_pPageView(nullptr),m_pLeftBtn(nullptr)
	,m_pRightBtn(nullptr),m_nCurIndex(0)
	,m_nSize(0),m_bClickBtn(true)
	,m_pConfirmBtn(nullptr),m_pGoldLabel(nullptr)
{
}

LevelChooseLayer::~LevelChooseLayer()
{
}

bool LevelChooseLayer::init()
{
	//添加外部文件
	auto node = ui::UIWidgetManager::getInstance()->createWidgetsWithXml("scene/LevelChooseLayer.xml");
	this->addChild(node);
	//获取各种有用的控件
	m_pCloseBtn = node->getChildByName<ui::Button*>("close_btn");
	//绑定按钮
	m_pCloseBtn->addClickEventListener(SDL_CALLBACK_1(LevelChooseLayer::closeThePage,this));
	//获取向左按钮
	m_pLeftBtn = node->getChildByName<ui::Button*>("left_btn");
	m_pLeftBtn->addClickEventListener(SDL_CALLBACK_1(LevelChooseLayer::leftBtnCallback,this));
	//获取向右按钮
	m_pRightBtn = node->getChildByName<ui::Button*>("right_btn");
	m_pRightBtn->addClickEventListener(SDL_CALLBACK_1(LevelChooseLayer::rightBtnCallback,this));
	//获取确认按钮
	m_pConfirmBtn = node->getChildByName<ui::Button*>("confirm_btn");
	m_pConfirmBtn->addClickEventListener(SDL_CALLBACK_1(LevelChooseLayer::confirmBtnCallback,this));
	//获取当前金币
	m_pGoldLabel = node->getChildByName<LabelAtlas*>("gold");
	//获取page view
	m_pPageView = node->getChildByName("level_page_view");
	//设置当前索引 todo
	//获取当前选中的关卡
	int tollgateIndex =  FishingJoyData::getInstance()->getTollgateIndex();
	m_nSize = m_pPageView->getChildren().size();

	this->movePage(tollgateIndex,0.f);

	return true;
}

void LevelChooseLayer::movePage(int nextPage,float duration)
{
	if(!m_bClickBtn)
		return;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	int temp = m_nCurIndex - nextPage;
	Point dstPos = Point(visibleSize.width * temp,0.f);

	if(duration <= FLT_EPSILON && duration >= -FLT_EPSILON)
	{
		Place*place = Place::create(dstPos);
		m_pPageView->runAction(place);
	}
	else
	{
		m_bClickBtn = false;

		MoveBy*move = MoveBy::create(duration,dstPos);
		CallFunc*end = CallFunc::create([this]()
		{
			this->m_bClickBtn = true;
		});
		auto seq = Sequence::createWithTwoActions(move,end);
		m_pPageView->runAction(seq);
	}
	m_nCurIndex = nextPage;
	//是否隐藏或者显示按钮
	if(m_nCurIndex == 0)
	{
		m_pLeftBtn->setVisible(false);
		m_pRightBtn->setVisible(true);
	}
	else if (m_nCurIndex == m_nSize - 1)
	{
		m_pLeftBtn->setVisible(true);
		m_pRightBtn->setVisible(false);
	}
	else
	{
		m_pLeftBtn->setVisible(true);
		m_pRightBtn->setVisible(true);
	}
}

void LevelChooseLayer::confirmBtnCallback(Object*sender)
{
	m_pDelegate->enterLoadingLayer();
}

void LevelChooseLayer::closeThePage(Object*sender)
{
	m_pDelegate->closeLevelChooseLevel();
}

void LevelChooseLayer::leftBtnCallback(Object*sender)
{
	int afterPage = m_nCurIndex - 1;

	if(afterPage >= 0)
	{
		this->movePage(afterPage,0.5f);
	}
}

void LevelChooseLayer::rightBtnCallback(Object*sender)
{
	int afterPage = m_nCurIndex + 1;

	if(afterPage < m_nSize)
	{
		this->movePage(afterPage,0.5f);
	}
}

int LevelChooseLayer::getTollgateIndex()const
{
	return m_nCurIndex;
}

void LevelChooseLayer::updateUI()
{
	auto gold = FishingJoyData::getInstance()->getGold();
	string text = to_string(gold);

	m_pGoldLabel->setString(text);
}

void LevelChooseLayer::setDelegate(LevelChooseLayerDelegate*pDelegate)
{
	m_pDelegate = pDelegate;
}