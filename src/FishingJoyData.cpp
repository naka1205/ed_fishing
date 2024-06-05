#include "FishingJoyData.h"
#include "StaticData.h"

#include "Cannon.h"

FishingJoyData*FishingJoyData::m_pInstance = nullptr;

FishingJoyData::FishingJoyData()
	:m_nGold(0),m_bChunk(false)
	,m_bMusic(false),m_isBeginner(true)
	,m_energy(0.f),m_nLevel(0),m_nExp(0)
	,m_nSterengthenBullet(0),m_nFogBullet(0)
	,m_nPearlBullet(0),m_nHalfEnergyNum(0)
	,m_nTollgateIndex(0)
{
}
FishingJoyData::~FishingJoyData()
{
	this->flush();
}
FishingJoyData*FishingJoyData::getInstance()
{
	if(m_pInstance == nullptr)
	{
		m_pInstance = new FishingJoyData();
		m_pInstance->init();
	}
	return m_pInstance;
}
void FishingJoyData::purge()
{
	if(m_pInstance)
		SDL_SAFE_RELEASE_NULL(m_pInstance);
}
bool FishingJoyData::init()
{
	auto userDefault = UserDefault::getInstance();
	//是否是第一次玩游戏
	m_isBeginner = userDefault->getBoolForKey("beginner",true);

	if(m_isBeginner)
	{
		this->reset();
		this->setBeginner(false);
		this->flush();
	}
	else
	{
		//读取金币数量
		m_nGold = userDefault->getIntegerForKey("gold",200);
		m_energy = userDefault->getFloatForKey("energy",0.f);
		m_nLevel = userDefault->getIntegerForKey("level",1);
		m_nExp = userDefault->getFloatForKey("exp",0.f);

		m_bChunk = userDefault->getBoolForKey("Chunk",true);
		m_bMusic = userDefault->getBoolForKey("Music",true);

		m_selectedBullet = userDefault->getStringForKey("SelectedBullet","NetGunBullet");
		m_nSterengthenBullet = userDefault->getIntegerForKey("SterengthenBullet",0);
		m_nFogBullet = userDefault->getIntegerForKey("FogBullet",0);
		m_nPearlBullet = userDefault->getIntegerForKey("PearlBullet",0);
		m_nHalfEnergyNum = userDefault->getIntegerForKey("HalfEnergy",0);
		m_nTollgateIndex = userDefault->getIntegerForKey("TollgateIndex",0);
		//清除UserDefault目前已经不需要了
		userDefault->purge();
	}

	return true;
}
void FishingJoyData::flush()
{
	auto userDefault = UserDefault::getInstance();
	//保存当前金币数目
	userDefault->setIntegerForKey("gold",this->getGold());
	userDefault->setFloatForKey("energy",this->getEnergy());
	userDefault->setIntegerForKey("level",this->getLevel());
	userDefault->setFloatForKey("exp",this->getExp());

	userDefault->setBoolForKey("Chunk",this->isPlayingChunk());
	userDefault->setBoolForKey("Music",this->isPlayingMusic());
	userDefault->setBoolForKey("beginner",this->isBeginner());

	userDefault->setStringForKey("SelectedBullet",this->getSelectedBullet());
	userDefault->setIntegerForKey("SterengthenBullet",this->getSterengthenBullet());
	userDefault->setIntegerForKey("FogBullet",this->getFogBullet());
	userDefault->setIntegerForKey("PearlBullet",this->getPearlBullet());
	userDefault->setIntegerForKey("HalfEnergy",this->getHalfEnergyNum());
	userDefault->setIntegerForKey("TollgateIndex",this->getTollgateIndex());

	userDefault->purge();
}
void FishingJoyData::reset()
{
	int gold = STATIC_DATA_INT("default_gold");

	this->setGold(gold);

	this->setPlayingChunk(true);
	this->setPlayingMusic(true);
	this->setLevel(1);

	this->setSelectedBullet("NetGunBullet");
	this->setSterengthenBullet(5);
	this->setFogBullet(5);
	this->setPearlBullet(5);
	this->setHalfEnergyNum(5);
	this->setTollgateIndex(0);
}


int FishingJoyData::getExpOfCurLevel()const
{
	//目前仅仅是一个简单的函数关系
	int exp = 1000 + (m_nLevel - 1)*100;

	return exp;
}

int FishingJoyData::getGoldByCannon(Cannon*pCannon)
{
	auto lv = pCannon->getNetGunBulletLv();

	return lv;
}

unsigned int FishingJoyData::getBulletNumByName(const string&bulletName)
{
	//todo
	if(bulletName == "NetGunBullet")
		return -1;
	else if(bulletName == "SterengthenBullet")
		return this->getSterengthenBullet();
	else if(bulletName == "FogBullet")
		return this->getFogBullet();
	else if(bulletName == "PearlBullet")
		return this->getPearlBullet();

	return 0;
}

unsigned int FishingJoyData::getSelectedBulletNum()
{
	return this->getBulletNumByName(m_selectedBullet);
}

void FishingJoyData::alterSelectedBulletNum(int num)
{
	if(m_selectedBullet == "NetGunBullet")
		return ;
	else if(m_selectedBullet == "SterengthenBullet")
	{
		this->setSterengthenBullet(this->getSterengthenBullet() + num);
	}
	else if(m_selectedBullet == "FogBullet")
	{
		this->setFogBullet(this->getFogBullet() + num);
	}
	else if(m_selectedBullet == "PearlBullet")
	{
		this->setPearlBullet(this->getPearlBullet() + num);
	}
}

int FishingJoyData::getGold()const
{
	return m_nGold;
}

void FishingJoyData::setGold(int gold)
{
	if (gold > 9999999)
		gold = 9999999;

	m_nGold = gold;
}