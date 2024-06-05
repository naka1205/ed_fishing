#include "B2EntityFactory.h"
#include "Fish.h"
#include "NetGunBullet.h"
#include "FishingNet.h"
#include "StaticData.h"
#include "FishingJoyData.h"
#include "SkillManager.h"
#include "StarFish.h"
#include "Bullet.h"
#include "Pearl.h"
#include "Fog.h"

B2EntityFactory*B2EntityFactory::s_pInstance = nullptr;

B2EntityFactory::B2EntityFactory()
	:m_pSkillManager(nullptr)
{
}

B2EntityFactory::~B2EntityFactory()
{
	for(auto iter = m_fishesPool.begin();iter != m_fishesPool.end();)
	{
		auto fish = *iter;
		fish->release();

		iter = m_fishesPool.erase(iter);
	}

	delete m_pSkillManager;
}

B2EntityFactory*B2EntityFactory::getInstance()
{
	if(s_pInstance == nullptr)
	{
		s_pInstance = new B2EntityFactory();
		s_pInstance->init();
	}
	return s_pInstance;
}
void B2EntityFactory::purge()
{
	if(s_pInstance)
	{
		delete s_pInstance;
	}
	s_pInstance = nullptr;
}
bool B2EntityFactory::init()
{
	m_pSkillManager = new SkillManager();
	m_pSkillManager->init();

	return true;
}

Fish*B2EntityFactory::createFishForID(int id)
{
	Fish*fish = nullptr;

	if(m_fishesPool.empty())
	{
		fish = Fish::create(id);
		//初始化刚体
		fish->initBody();
	}
	else
	{
		fish = m_fishesPool.front();

		fish->init(id);
		fish->setActive(true);
		fish->setDead(false);
		//加入内存管理池
		fish->autorelease();

		m_fishesPool.pop_front();
	}
	fish->setState(Fish::State::Running);
	fish->setType(Entity::Type::Fish);

	fish->setHP(1);
	//这个跟图片有关
	fish->getSprite()->setFlipX(true);
	//设置技能
	FishConfig*config = StaticData::getInstance()->getFishConfigForID(id);

	map<TriggingType,Skill*> skills;
	if(!config->speedUpName.empty())
	{
		auto skill = m_pSkillManager->getDefaultSkill(config->speedUpName);
		skills.insert(make_pair(TriggingType::Fishing,skill));
	}
	if(!config->scaredName.empty())
	{
		auto skill = m_pSkillManager->getDefaultSkill(config->scaredName);
		skills.insert(make_pair(TriggingType::GroupCome,skill));
	}
	if(!config->invulnerableName.empty())
	{
		auto skill = m_pSkillManager->getDefaultSkill(config->invulnerableName);
		skills.insert(make_pair(TriggingType::NetGunBullet,skill));
	}
	//设置技能
	fish->setSkills(skills);
	//设置刚体
	string shape_format = STATIC_DATA_STRING("fish_shape_format");
	string shapeName = StringUtils::format(shape_format.c_str(),id);
	fish->addFixturesToBody(shapeName);

	return fish;
}

StarFish*B2EntityFactory::createStarFish(float startTime)
{
	StarFish*starFish = StarFish::create(startTime);

	starFish->setHP(1);
	starFish->setID(0);
	starFish->setType(Entity::Type::Fish);
	starFish->setFishType(FishType::StarFish);

	//设置刚体
	starFish->initBody();
	string shapeName = STATIC_DATA_STRING("star_fish_shape_name");
	starFish->addFixturesToBody(shapeName);

	return starFish;
}

NetGunBullet*B2EntityFactory::createNetGunBulletForLv(int level)
{
	NetGunBullet*bullet = NetGunBullet::create(level);
	bullet->setType(Entity::Type::Bullet);
	bullet->setHP(1);
	bullet->setBulletType(BulletType::NetGunBullet);

	bullet->initBody();

	//设置刚体
	string shape_format = STATIC_DATA_STRING("net_gun_bullet_shape_format");
	string shapeName = StringUtils::format(shape_format.c_str(),level);

	bullet->addFixturesToBody(shapeName);

	return bullet;
}

Bullet*B2EntityFactory::createFogBullet()
{
	Bullet*bullet = Bullet::create();

	bullet->setType(Entity::Type::Bullet);
	bullet->setBulletType(BulletType::FogBullet);
	bullet->setHP(1);
	//todo
	bullet->setDistance(800);

	bullet->initBody();
	//设置精灵
	auto spriteName = STATIC_DATA_STRING("fog_bullet_sprite_name");
	bullet->bindSpriteWithSpriteFrameName(spriteName);
	//设置刚体
	string shapeName = STATIC_DATA_STRING("fog_shape_name");
	bullet->addFixturesToBody(shapeName);

	return bullet;
}

FishingNet*B2EntityFactory::createFishingNet(int level)
{
	FishingNet*fishingNet = FishingNet::create(level);

	fishingNet->setType(Entity::Type::Fishing);
	fishingNet->setFishingType(FishingType::FishingNet);
	fishingNet->setHP(1);
	fishingNet->initBody();
	//设置刚体
	string shape_format = STATIC_DATA_STRING("fish_net_shape_format");
	string shapeName = StringUtils::format(shape_format.c_str(),level);

	fishingNet->addFixturesToBody(shapeName);

	return fishingNet;
}

Bullet*B2EntityFactory::createStrengthenBullet()
{
	Bullet*bullet = Bullet::create();

	bullet->setType(Entity::Type::Bullet);
	bullet->setBulletType(BulletType::StrengthenBullet);
	bullet->setHP(1);
	//todo
	bullet->setDistance(800);

	bullet->initBody();
	//设置精灵
	auto spriteName = STATIC_DATA_STRING("strengthen_bullet_sprite_name");
	bullet->bindSpriteWithSpriteFrameName(spriteName);
	//设置刚体
	string shapeName = STATIC_DATA_STRING("strengthen_bullet_shape_name");
	bullet->addFixturesToBody(shapeName);

	return bullet;
}

Laser*B2EntityFactory::createLaser()
{
	Laser*laser = Laser::create();

	laser->setType(Entity::Type::Fishing);
	laser->setFishingType(FishingType::Laser);
	laser->setHP(1);
	//加入刚体
	auto shapeName = STATIC_DATA_STRING("laser_bullet_shape");

	laser->initBody();
	laser->addFixturesToBody(shapeName);

	return laser;
}

Pearl*B2EntityFactory::createPearl()
{
	Pearl*pearl = Pearl::create();

	//todo
	pearl->setHP(10);
	pearl->setType(Entity::Type::Fishing);
	pearl->setFishingType(FishingType::Pearl);

	//创建刚体
	auto shapeName = STATIC_DATA_STRING("pearl_shape_name");

	pearl->initBody();
	pearl->addFixturesToBody(shapeName);

	return pearl;
}

Fog*B2EntityFactory::createFog(Fish*fish)
{
	//生成迷雾
	Fog*fog = Fog::create(fish);

	fog->setType(Entity::Type::Fishing);
	fog->setFishingType(FishingType::Fog);
	fog->setHP(1);
	//获取该鱼的id
	int fishID = fish->getID();
	//获取该鱼的刚体名称
	string shape_format = STATIC_DATA_STRING("fish_shape_format");
	string shapeName = StringUtils::format(shape_format.c_str(),fishID);
	
	fog->initBody();
	fog->addFixturesToBody(shapeName);
	//设置形状的mask和category
	//获取maskbit和category todo
	b2Filter filter;
	filter.maskBits = 0x04;
	filter.categoryBits = 0x01;

	for(auto fixture = fog->getFixtureList();fixture;fixture = fixture->GetNext())
	{
		fixture->SetFilterData(filter);
	}

	return fog;
}

void B2EntityFactory::pushFishToPool(Fish*fish)
{
	//清除动画,设置一些属性
	fish->clear();
	//清除形状(不清除刚体)
	fish->clearFixtures();

	fish->retain();
	m_fishesPool.push_back(fish);

	fish->removeFromParent();
}