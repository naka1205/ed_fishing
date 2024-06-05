#include "BulletLayer.h"
#include "B2EntityFactory.h"
#include "NetGunBullet.h"

BulletLayer::BulletLayer()
	:m_pDelegate(nullptr)
{
}
BulletLayer::~BulletLayer()
{
}
bool BulletLayer::init()
{
	return true;
}

void BulletLayer::update(float dt)
{
	for(auto iter = m_bullets.begin();iter != m_bullets.end();)
	{
		auto bullet = *iter;

		if(bullet->isDead())
		{
			//如果能生成渔网，则添加渔网
			if(bullet->isMakeFishingEntity())
			{
				auto fish = bullet->getCollidedFish();
				m_pDelegate->shootAt(bullet,fish);
			}
			//移除
			iter = m_bullets.erase(iter);
			bullet->removeFromParent();
		}
		else
			iter++;
	}
}

void BulletLayer::addNetGunBullet(int level,const Point&bornPos,const Point&loc,float rotation)
{
	Point target = loc - bornPos;
	//创建网枪弹
	auto bullet = B2EntityFactory::getInstance()->createNetGunBulletForLv(level);

	bullet->setPosition(bornPos);
	bullet->getSprite()->setRotation(rotation);

	this->addChild(bullet);
	//添加到容器中
	m_bullets.push_back(bullet);

	//获取子弹最大距离
	auto distance = bullet->getDistance();

	//获取子弹落点 并设置方向
	auto degree = SDL_ANGLE_TO_DEGREE(rotation);
	target = Point(distance * SDL_sinf(degree),distance * SDL_cosf(degree) * -1.f);
	//获取子弹速度
	auto speed = bullet->getSpeed();
	float time = target.length()/speed;
	//运行动作
	MoveBy*move = MoveBy::create(time,target);

	CallFunc*end = CallFunc::create([bullet,this]()
	{
		bullet->collided(nullptr);//或者bullet->setDead(true);
	});

	auto seq = Sequence::create(move,end,nullptr);

	bullet->runAction(seq);
}

void BulletLayer::addStrengthenBullet(const Point&bornPos,const Point&loc,float rotation)
{
	Point target = loc - bornPos;
	//创建加强弹
	auto bullet = B2EntityFactory::getInstance()->createStrengthenBullet();

	bullet->setPosition(bornPos);
	bullet->getSprite()->setRotation(rotation);

	this->addChild(bullet);
	//添加到容器中
	m_bullets.push_back(bullet);

	//获取子弹最大距离
	auto distance = bullet->getDistance();

	//获取子弹落点 并设置方向
	auto degree = SDL_ANGLE_TO_DEGREE(rotation);
	target = Point(distance * SDL_sinf(degree),distance * SDL_cosf(degree) * -1.f);
	//获取子弹速度
	auto speed = bullet->getSpeed();
	float time = target.length()/speed;
	//运行动作
	MoveBy*move = MoveBy::create(time,target);

	CallFunc*end = CallFunc::create([bullet]()
	{
		bullet->collided(nullptr);
	});

	auto seq = Sequence::create(move,end,nullptr);

	bullet->runAction(seq);
}

void BulletLayer::addFogBullet(const Point&bornPos,const Point&loc,float rotation)
{
	Point target = loc - bornPos;
	//创建迷雾弹
	auto bullet = B2EntityFactory::getInstance()->createFogBullet();

	bullet->setPosition(bornPos);
	bullet->getSprite()->setRotation(rotation);

	this->addChild(bullet);
	//添加到容器中
	m_bullets.push_back(bullet);

	//获取子弹最大距离
	auto distance = bullet->getDistance();

	//获取子弹落点 并设置方向
	auto degree = SDL_ANGLE_TO_DEGREE(rotation);
	target = Point(distance * SDL_sinf(degree),distance * SDL_cosf(degree) * -1.f);
	//获取子弹速度
	auto speed = bullet->getSpeed();
	float time = target.length()/speed;
	//运行动作
	MoveBy*move = MoveBy::create(time,target);

	CallFunc*end = CallFunc::create([bullet]()
	{
		bullet->collided(nullptr);
	});

	auto seq = Sequence::create(move,end,nullptr);

	bullet->runAction(seq);
}

void BulletLayer::setDelegate(BultLayerDelegate*pDelegate)
{
	m_pDelegate = pDelegate;
}