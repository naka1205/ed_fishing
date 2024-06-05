#include "LevelLayer.h"
#include "StaticData.h"

LevelLayer::LevelLayer()
	:m_pTiledMap(nullptr),m_nRatio(1)
	,m_fTax(0.f)
{
}
LevelLayer::~LevelLayer()
{
}
LevelLayer*LevelLayer::create(const string&levelName)
{
	auto layer = new LevelLayer();

	if(layer && layer->initWithLevelName(levelName))
		layer->autorelease();
	else
		SDL_SAFE_DELETE(layer);

	return layer;
}

bool LevelLayer::initWithLevelName(const string&levelName)
{
	m_pTiledMap = TMXTiledMap::create(levelName);
	this->addChild(m_pTiledMap);

	this->parseFishID();
	this->parseNetGun();
	this->parseFishTide();
	this->parseStarFishStartTime();
	//获取场景倍率
	auto rateName = STATIC_DATA_STRING("level_ratio");
	auto valueRate = m_pTiledMap->getPropertyForName(rateName);

	m_nRatio = valueRate.asInt();
	//获取当前场景的抽税值
	auto taxName = STATIC_DATA_STRING("level_tax");
	auto valueTax = m_pTiledMap->getPropertyForName(taxName);

	m_fTax = valueTax.asFloat();

	return true;
}

vector<int>& LevelLayer::getShownFishIdVec()
{
	return m_fishIDs;
}

vector<int>& LevelLayer::getNetGunBulletVec()
{
	return m_netGunLevelIDs;
}

vector<int>& LevelLayer::getFishTideVec()
{
	return m_fishTideIDs;
}

vector<float>& LevelLayer::getStartFishStartTimeVec()
{
	return m_starFishStartTimes;
}

string LevelLayer::getBGM()
{
	auto property = m_pTiledMap->getPropertyForName("bgm");

	return property.asString();
}

void LevelLayer::parseFishID()
{
	auto fishes = STATIC_DATA_STRING("level_fish");
	auto property = m_pTiledMap->getPropertyForName(fishes);

	string text = property.asString();

	this->parse(text,",",m_fishIDs);
}

void LevelLayer::parseNetGun()
{
	auto net_gun = STATIC_DATA_STRING("level_net_gun");
	auto property = m_pTiledMap->getPropertyForName(net_gun);

	string text = property.asString();

	this->parse<int>(text,",",m_netGunLevelIDs);
}

void LevelLayer::parseFishTide()
{
	auto fishes = STATIC_DATA_STRING("level_fish_tide");
	auto property = m_pTiledMap->getPropertyForName(fishes);

	string text = property.asString();

	this->parse<int>(text,",",m_fishTideIDs);
}

void LevelLayer::parseStarFishStartTime()
{
	auto fishes = STATIC_DATA_STRING("star_fish_start_time");
	auto property = m_pTiledMap->getPropertyForName(fishes);

	string text = property.asString();

	this->parse<float>(text,",",m_starFishStartTimes);
}

template<typename T>
void LevelLayer::parse(const string&src,const string&token,vector<T>&vect)
{
	unsigned int nend = 0;
	unsigned int nbegin = 0;
	unsigned int tokenSize = token.size();

	while(nend != std::string::npos)
	{
		nend = src.find(token,nbegin);
		if(nend == std::string::npos)
		{
			auto str = src.substr(nbegin, src.length()-nbegin);
			float id = stof(str);

			vect.push_back(id);
		}
		else
		{
			auto str = src.substr(nbegin, nend-nbegin);
			float id = stof(str);

			vect.push_back(id);
		}
		nbegin = nend + tokenSize;
	}
}