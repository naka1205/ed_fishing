#ifndef __StaticData_H__
#define __StaticData_H__
#include<map>
#include<string>
#include<vector>
#include<sstream>
#include<algorithm>

#include "SG_Engine.h"
using namespace std;
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
	string name;
	string desc;
	int gold;
	int weight;
	int max_per_time;//每次能生成的最大数目
	int max;//场上最多出现的数目

	string speedUpName;//加速技能名字
	string scaredName;//鱼群来临技能
	string invulnerableName;//无敌技能
public:
	FishConfig()
		:id(0),gold(0),weight(0),max_per_time(0),max(0)
	{}
}FishConfig;

typedef struct FishBornConfig
{
	Point pos;
	int fishID;
public:
	FishBornConfig(const Point&pos,int id)
		:pos(pos),fishID(id)
	{
	}
}FishBornConfig;

typedef struct FishTideConfig
{
	int count;
	int path_id;
	vector<FishBornConfig> fishes;
}FishTideConfig;

typedef struct ChestReward
{
	string type;//类型
	int number;//数目
}ChestReward;

typedef struct FishReward
{
	int fishID;
	string type;
	int number;
	float scope;
public:
	FishReward(int fishID,const string&type,int number,float scope)
		:fishID(fishID),type(type),number(number),scope(scope)
	{
	}
}FishReward;

class StaticData:public Object
{
	SDL_SYNTHESIZE_READONLY(string,m_staticDataPath,StaticDataPath);//获取静态数据类型
private:
	StaticData();
	~StaticData();
private:
	static StaticData*m_pInstance;
	//保存数据字典
	ValueMap m_plistMap;
	//鱼的配置信息
	vector<FishConfig*> m_fishConfigs;
	//鱼的路径信息
	ValueMap m_pathes;
	//鱼和路径对应的信息
	map<int,vector<int> > m_fishPathes;
	//鱼潮的相关信息
	map<int,FishTideConfig> m_fishTideConfigs;
	//宝箱奖励相关信息
	vector<ChestReward> m_chestRewards;
	//鱼可能掉落的物品集合
	vector<FishReward> m_fishRewardVec;
public:
	static StaticData*getInstance();
	static void purge();
	bool init();

	Value *getValueForKey(const string&key);

	Point getPointForKey(const string&key);

	FishTideConfig& getFishTideByID(int fishTideID);

	RotateAndSpeed*getActionByPathID(int pathID,const Size&size,float offsetX,float randomY,bool reverse);

	Point getStartPosByPathID(int pathID,bool reverse = false);

	FishConfig*getFishConfigForID(int id)const;

	const vector<int>* getTotalPathes(int fishID)const;
	//获取鱼所对应的经验值 分数即经验
	float getExpByFishID(int id);
	//获取鱼所对应的能量值
	float getEnergyByFishID(int id);
	//根据id获取对应的金币
	int getGoldByFishID(int id);
	//根据id获取对应的最大出场值
	int getMaxByFishID(int id);
	//获取一次能产生的最大数目
	int getMaxPerTimeByFishID(int id);
	//获取权数
	int getWeightByFishID(int id);
	//获取全部的箱子奖励物品
	vector<ChestReward>&getChestRewards();
	vector<FishReward>&getFishRewards();
private:
	ActionInterval*getAction(const ValueMap&valueMap,const Size&size,float offsetX,float randomY,bool reverse);
	
	void parseFishConfig();
	void parseFishPath();
	void parseFishTide();
	void parseChestReward();
	void parseFishReward();
};
#endif