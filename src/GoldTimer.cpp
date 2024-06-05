#include "GoldTimer.h"
#include "FishingJoyData.h"
GoldTimer::GoldTimer()
	:m_nGold(5),m_nSecond(60)
	,m_elpased(0.f),m_pLabel(nullptr)
	,m_pDelegate(nullptr)
{
}
GoldTimer::~GoldTimer()
{
}
bool GoldTimer::init()
{
	m_pLabel = LabelAtlas::create("60","common-font.png",16,24,'0');
	this->addChild(m_pLabel);

	return true;
}

void GoldTimer::setDelegate(GoldTimerDelegate*pDelegate)
{
	m_pDelegate = pDelegate;
}

void GoldTimer::update(float dt)
{
	m_elpased += dt;
	if(m_elpased >= m_nSecond)
	{
		m_elpased -= m_nSecond;

		auto pos = this->getPosition();

		m_pDelegate->addGold(m_nGold,pos);
	}
	int second = m_nSecond - m_elpased;
	
	m_pLabel->setString(StringUtils::format("%d",second));
}