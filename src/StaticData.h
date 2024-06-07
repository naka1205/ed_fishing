#ifndef __StaticData_H__
#define __StaticData_H__
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "SG_Engine.h"

using namespace SDL;

class RotateAndSpeed;

#define STATIC_DATA_PATH "data/static_data.plist"

#define STATIC_DATA_STRING(key) (StaticData::getInstance()->getValueForKey(key)->asString())
#define STATIC_DATA_INT(key) StaticData::getInstance()->getValueForKey(key)->asInt()
#define STATIC_DATA_FLOAT(key) StaticData::getInstance()->getValueForKey(key)->asFloat()
#define STATIC_DATA_BOOLEAN(key) StaticData::getInstance()->getValueForKey(key)->asBool()
#define STATIC_DATA_POINT(key) (StaticData::getInstance()->getPointForKey(key))

#define STATIC_FISH_EXP(id) (StaticData::getInstance()->getExpByFishID(id))
#define STATIC_FISH_ENERGY(id) (StaticData::getInstance()->getEnergyByFishID(id))
#define STATIC_FISH_GOLD(id) (StaticData::getInstance()->getGoldByFishID(id))
#define STATIC_FISH_MAX(id) (StaticData::getInstance()->getMaxByFishID(id))
#define STATIC_FISH_MAX_PER_TIME(id) (StaticData::getInstance()->getMaxPerTimeByFishID(id))
#define STATIC_FISH_WEIGHT(id) (StaticData::getInstance()->getWeightByFishID(id))

typedef struct FishConfig
{
	int id;
	std::string name;
	std::string desc;
	int gold;
	int weight;
	int max_per_time;
	int max;

	std::string speedUpName;
	std::string scaredName;
	std::string invulnerableName;

public:
	FishConfig()
			: id(0), gold(0), weight(0), max_per_time(0), max(0)
	{
	}
} FishConfig;

typedef struct FishBornConfig
{
	Point pos;
	int fishID;

public:
	FishBornConfig(const Point &pos, int id)
			: pos(pos), fishID(id)
	{
	}
} FishBornConfig;

typedef struct FishTideConfig
{
	int count;
	int path_id;
	std::vector<FishBornConfig> fishes;
} FishTideConfig;

typedef struct ChestReward
{
	std::string type;
	int number;
} ChestReward;

typedef struct FishReward
{
	int fishID;
	std::string type;
	int number;
	float scope;

public:
	FishReward(int fishID, const std::string &type, int number, float scope)
			: fishID(fishID), type(type), number(number), scope(scope)
	{
	}
} FishReward;

class StaticData : public Object
{
	SDL_SYNTHESIZE_READONLY(std::string, m_staticDataPath, StaticDataPath);

private:
	StaticData();
	~StaticData();

private:
	static StaticData *m_pInstance;

	ValueMap m_plistMap;

	std::vector<FishConfig *> m_fishConfigs;

	ValueMap m_pathes;

	std::map<int, std::vector<int>> m_fishPathes;

	std::map<int, FishTideConfig> m_fishTideConfigs;

	std::vector<ChestReward> m_chestRewards;

	std::vector<FishReward> m_fishRewardVec;

public:
	static StaticData *getInstance();
	static void purge();
	bool init();

	Value *getValueForKey(const std::string &key);

	Point getPointForKey(const std::string &key);

	FishTideConfig &getFishTideByID(int fishTideID);

	RotateAndSpeed *getActionByPathID(int pathID, const Size &size, float offsetX, float randomY, bool reverse);

	Point getStartPosByPathID(int pathID, bool reverse = false);

	FishConfig *getFishConfigForID(int id) const;

	const std::vector<int> *getTotalPathes(int fishID) const;

	float getExpByFishID(int id);

	float getEnergyByFishID(int id);

	int getGoldByFishID(int id);

	int getMaxByFishID(int id);

	int getMaxPerTimeByFishID(int id);

	int getWeightByFishID(int id);

	std::vector<ChestReward> &getChestRewards();
	std::vector<FishReward> &getFishRewards();

private:
	ActionInterval *getAction(const ValueMap &valueMap, const Size &size, float offsetX, float randomY, bool reverse);

	void parseFishConfig();
	void parseFishPath();
	void parseFishTide();
	void parseChestReward();
	void parseFishReward();
};
#endif