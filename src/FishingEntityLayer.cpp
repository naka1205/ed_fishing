#include "FishingEntityLayer.h"
#include "FishingNet.h"
#include "B2EntityFactory.h"
#include "Pearl.h"
#include "Fog.h"

FishingEntityLayer::FishingEntityLayer()
		: m_pDelegate(nullptr)
{
}

FishingEntityLayer::~FishingEntityLayer()
{
}

bool FishingEntityLayer::init()
{
	return true;
}

void FishingEntityLayer::update(float dt)
{
	for (auto iter = m_fishingEntitys.begin(); iter != m_fishingEntitys.end();)
	{
		auto entity = *iter;

		if (entity->isDead())
		{
			// 移除
			iter = m_fishingEntitys.erase(iter);
			entity->removeFromParent();
		}
		else
		{
			if (entity->getHP() <= 0)
				entity->setActive(false);

			iter++;
			entity->update(dt);
		}
	}
}

void FishingEntityLayer::setDelegate(FishingEntityLayerDelegate *pDelegate)
{
	m_pDelegate = pDelegate;
}

void FishingEntityLayer::addFishingNet(int lv, const Point &pos, float rotation)
{
	auto fishingNet = B2EntityFactory::getInstance()->createFishingNet(lv);
	// 设置位置
	fishingNet->setPosition(pos);
	// 设置旋转
	fishingNet->setRotation(rotation);

	this->addChild(fishingNet);
	m_fishingEntitys.push_back(fishingNet);
	// 显示动画
	float duration = fishingNet->showAt();

	DelayTime *delayTime = DelayTime::create(duration);
	CallFunc *end = CallFunc::create([fishingNet]()
																	 { fishingNet->setDead(true); });
	auto seq = Sequence::create(delayTime, end, nullptr);

	fishingNet->runAction(seq);
}

void FishingEntityLayer::addLaser(const Point &pos, float rotation)
{
	Point offset;

	auto laser = B2EntityFactory::getInstance()->createLaser();

	laser->setVisible(false);
	// 先设置刚体不参与碰撞
	laser->setActive(false);

	this->addChild(laser);
	m_fishingEntitys.push_back(laser);

	// 修正位置 todo
	// Size size = laser->getContentSize() + Size(60, 60);
	Size size = laser->getContentSize();
	// size->setSize(size.width + 60.f, size.height + 60.f);

	float degree = SDL_ANGLE_TO_DEGREE(rotation);

	offset.x = size.height / 2 * SDL_sinf(degree);
	offset.y = -size.height / 2 * SDL_cosf(degree);

	laser->setPosition(pos + offset);

	laser->getSprite()->setRotation(rotation);

	// 先停1s
	DelayTime *delayTime = DelayTime::create(1.f);

	CallFunc *fire = CallFunc::create([laser, this]()
																		{
		laser->setVisible(true);
		laser->setActive(true);

		float duration = laser->showAt();

		DelayTime*delay = DelayTime::create(duration);

		CallFunc*end = CallFunc::create([this,laser]()
		{
			m_pDelegate->fireEnd();
			laser->setDead(true);
		});

		auto seq = Sequence::create(delay,end,nullptr);

		laser->runAction(seq); });
	auto delayFire = Sequence::create(delayTime, fire, NULL);

	this->runAction(delayFire);
}

void FishingEntityLayer::addPearl(const Point &pos, float rotation)
{
	Pearl *pearl = B2EntityFactory::getInstance()->createPearl();
	pearl->setPosition(pos);
	pearl->setDelat(rotation);

	this->addChild(pearl);
	m_fishingEntitys.push_back(pearl);
}

void FishingEntityLayer::addFog(Fish *fish, const Point &pos, float rotation)
{
	Fog *fog = B2EntityFactory::getInstance()->createFog(fish);

	fog->setPosition(pos);
	fog->getSprite()->setRotation(rotation);

	this->addChild(fog);
	m_fishingEntitys.push_back(fog);
}