#include "B2Entity.h"
#include "GB2ShapeCacheX.h"
#include "GameScene.h"

B2Entity::B2Entity()
	:m_bActive(true),m_pBody(nullptr)
	,m_bCollided(false)
{ 
}
B2Entity::~B2Entity()
{
	if(m_pBody)
	{
		auto pWorld = GameScene::getWorld();
		if (pWorld)
			pWorld->DestroyBody(m_pBody);
	}
	m_pBody = nullptr;
}
void B2Entity::initBody()
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	//创建刚体
	b2World*pWorld = GameScene::getWorld();
	m_pBody = pWorld->CreateBody(&bodyDef);//PhysicalEngine::getInstance()->createBody(&bodyDef);
	//设置刚体用户数据
	m_pBody->SetUserData(this);
}
void B2Entity::addFixturesToBody(const std::string &shape)
{
	GB2ShapeCache::getInstance()->addFixturesToBody(m_pBody,shape);
}

void B2Entity::clearFixtures()
{
	//先清除原先的fixture
	for(auto fixture = m_pBody->GetFixtureList();fixture;)
	{
		auto temp = fixture;
		
		fixture = fixture->GetNext();

		m_pBody->DestroyFixture(temp);
	}
}

b2Fixture*B2Entity::getFixtureList()const
{
	if(m_pBody)
		return m_pBody->GetFixtureList();
	return nullptr;
}

void B2Entity::setActive(bool active)
{
	if(m_pBody == nullptr)
		return;

	m_pBody->SetActive(active);
	m_bActive = active;
}

bool B2Entity::isActive()const
{
	return m_bActive;
}