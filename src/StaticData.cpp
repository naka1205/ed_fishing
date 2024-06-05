#include "StaticData.h"
#include "RotateAndSpeed.h"

StaticData*StaticData::m_pInstance = nullptr;

StaticData::StaticData()
{
	m_staticDataPath = STATIC_DATA_PATH;
}
StaticData::~StaticData()
{
	for(auto iter = m_fishConfigs.begin();iter != m_fishConfigs.end();)
	{
		auto temp = *iter;
		delete temp;

		iter = m_fishConfigs.erase(iter);
	}

	m_fishPathes.clear();
}
StaticData*StaticData::getInstance()
{
	if(m_pInstance == nullptr)
	{
		m_pInstance = new StaticData();
		m_pInstance->init();
	}
	return m_pInstance;
}
void StaticData::purge()
{
	if(m_pInstance)
		SDL_SAFE_RELEASE_NULL(m_pInstance);
}
bool StaticData::init()
{
	bool bRet = true;
	//读取文件
	m_plistMap = FileUtils::getInstance()->getValueMapFromFile(m_staticDataPath);
	bRet |= !m_plistMap.empty();
	
	//加载路径信息
	auto path = this->getValueForKey("fish_path_path").asString();
	m_pathes = FileUtils::getInstance()->getValueMapFromFile(path);
	//加载鱼潮信息
	this->parseFishTide();
	//加载鱼的配置信息
	this->parseFishConfig();
	//加载鱼和路径的对应信息
	this->parseFishPath();
	//加载宝箱奖励信息
	this->parseChestReward();
	//加载鱼所对应的奖励
	this->parseFishReward();

	return bRet;
}

Value &StaticData::getValueForKey(const string&key)
{
	auto iter = m_plistMap.find(key);
	if(iter != m_plistMap.end())
		return iter->second;
	
	return Value("");
}
Point StaticData::getPointForKey(const string&key)
{
	Point ret;

	auto value = this->getValueForKey(key);
	if(value.getType() != Value::Type::STRING)
		return ret;
	ret = PointFromString(value.asString());
	return ret;
}

FishTideConfig& StaticData::getFishTideByID(int fishTideID)
{
	auto iter = m_fishTideConfigs.find(fishTideID);

	SDL_assert(iter != m_fishTideConfigs.end());

	return iter->second;
}

RotateAndSpeed*StaticData::getActionByPathID(int pathID,const Size&size,float offsetX,float randomY,bool reverse)
{
	auto iter = m_pathes.find(to_string(pathID));

	if(iter == m_pathes.end())
		return nullptr;

	Value value = iter->second;
	FiniteTimeAction*action = nullptr;

	if(value.getType() == Value::Type::MAP)
	{
		ValueMap& valueMap = value.asValueMap();

		action = this->getAction(valueMap,size,offsetX,randomY,reverse);
	}
/*	else if(value.getType() == Value::Type::VECTOR)
	{
		vector<FiniteTimeAction*> actions;
		auto values = value.asValueVector();

		for(auto iter = values.begin();iter != values.end();iter++)
		{
			ValueMap&valueMap = iter->asValueMap();

			auto action = this->getAction(valueMap,reverse);

			actions.push_back(action);
		}

		action = Sequence::create(actions);
	}*/

	return RotateAndSpeed::create(action);
}

Point StaticData::getStartPosByPathID(int pathID,bool reverse)
{
	//获取路径dict
	auto iter = m_pathes.find(to_string(pathID));

	SDL_assert(iter != m_pathes.end());

	ValueMap path = iter->second.asValueMap();

	string str;

	if(reverse)
		str = path.at("reverse-start-pos").asString();
	else
		str = path.at("startPos").asString();

	SDL_assert(!str.empty());

	Point pos = PointFromString(str);
	
	return pos;
}

const vector<int>* StaticData::getTotalPathes(int fishID)const
{
	auto iter = m_fishPathes.find(fishID);

	if(iter != m_fishPathes.end())
	{
		return &iter->second;
	}
	
	return nullptr;
}

FishConfig*StaticData::getFishConfigForID(int id)const
{
	FishConfig*config = nullptr;

	auto iter =find_if(m_fishConfigs.cbegin(),m_fishConfigs.cend(),[id](FishConfig*config)
	{
		return config->id == id;
	});

	if(iter != m_fishConfigs.cend())
	{
		config = *iter;
	}

	return config;
}

float StaticData::getExpByFishID(int id)
{
	FishConfig*config = this->getFishConfigForID(id);
	//todo 金币即经验值
	if(config)
	{
		return config->gold;
	}

	return 0;
}

float StaticData::getEnergyByFishID(int id)
{
	float energy = 0.f;
	FishConfig*config = this->getFishConfigForID(id);
	//能量值=金币/10
	if(config)
	{
		energy = config->gold/50.f;
	}

	return energy;
}

int StaticData::getGoldByFishID(int id)
{
	FishConfig*config = this->getFishConfigForID(id);

	if(config)
	{
		return config->gold;
	}
	return 0;
}

int StaticData::getMaxByFishID(int id)
{
	FishConfig*config = this->getFishConfigForID(id);

	if(config)
	{
		return config->max;
	}
	return 0;
}

int StaticData::getMaxPerTimeByFishID(int id)
{
	FishConfig*config = this->getFishConfigForID(id);

	if(config)
	{
		return config->max_per_time;
	}

	return 0;
}

int StaticData::getWeightByFishID(int id)
{
	FishConfig*config = this->getFishConfigForID(id);

	if(config)
	{
		return config->weight;
	}
	return 0;
}

vector<ChestReward>&StaticData::getChestRewards()
{
	return m_chestRewards;
}

vector<FishReward>& StaticData::getFishRewards()
{
	return m_fishRewardVec;
}

ActionInterval*StaticData::getAction(const ValueMap&valueMap,const Size&size,float offsetX,float randomY,bool reverse)
{
	float duration = 0.f;
	//设置偏移
	//float offsetY = rand()%200 - 100;

	Point offset = Point(size.width + offsetX,randomY);

	string type;
	ActionInterval*action = nullptr;
	//先获取必要的属性
	duration = valueMap.at("duration").asFloat();
	type = valueMap.at("type").asString();

	if(type == "MoveBy")
	{
		Point relativePos = PointFromString(valueMap.at("relativePos").asString());
		relativePos += offset; 

		action = MoveBy::create(duration,relativePos);
	}
	else if(type == "BezierBy")
	{
		ccBezierConfig config;

		config.controlPoint_1 = PointFromString(valueMap.at("controlPoint_1").asString());
		config.controlPoint_2 = PointFromString(valueMap.at("controlPoint_2").asString());
		config.endPosition = PointFromString(valueMap.at("endPos").asString());

		config.controlPoint_1 += offset;
		config.controlPoint_2 += offset;
		config.endPosition += offset;

		action = BezierBy::create(duration,config);
	}

	if(reverse)
		return action->reverse();

	return action;
}

void StaticData::parseFishConfig()
{
	//加载fish配置信息
	auto path = this->getValueForKey("fish_csv_path").asString();
	//获取字符串,并设置为字符流
	stringstream text(FileUtils::getInstance()->getDataFromFile(path));
	string line;
	int n = 0;

	while(getline(text,line))
	{
		//去掉前两行和为空的行
		if(n == 0 || n == 1 || line.empty())
		{
			n++;
			continue;
		}
		
		ValueVector value = StringUtils::split(line,',');
		//新建对象并赋值
		FishConfig*fishConfig = new FishConfig();
		
		fishConfig->id = value.at(0).asInt();
		fishConfig->name = value.at(1).asString();
		fishConfig->desc = value.at(2).asString();
		fishConfig->gold = value.at(3).asInt();
		fishConfig->weight = value.at(4).asInt();
		fishConfig->max_per_time = value.at(5).asInt();
		fishConfig->max = value.at(6).asInt();
		fishConfig->speedUpName = value.at(7).asString();
		fishConfig->scaredName = value.at(8).asString();

		auto str = value.at(9).asString();
		//todo 删除 \r
		int index = str.find('\r');

		str = str.substr(0,index);

		fishConfig->invulnerableName = str;
		//放入数组中
		m_fishConfigs.push_back(fishConfig);
	}
}

void StaticData::parseFishPath()
{
	stringstream text;
	string line;
	//加载fish path对应信息
	auto path = this->getValueForKey("fish_path_csv_path").asString();
	//重置信息
	text.str(FileUtils::getInstance()->getDataFromFile(path));
	int n = 0;

	while(getline(text,line))
	{
		//去掉前两行和为空的行
		if(n == 0 || n == 1 || line.empty())
		{
			n++;
			continue;
		}
		ValueVector value = StringUtils::split(line,',');
		
		int fishID = value.at(0).asInt();
		int pathID = value.at(1).asInt();
		//把相关信息放入map中
		auto iter = m_fishPathes.find(fishID);

		if(iter == m_fishPathes.end())
		{
			vector<int> path;
			path.push_back(pathID);
			m_fishPathes.insert(make_pair(fishID,path));
		}
		else
		{
			auto & path = iter->second;

			path.push_back(pathID);
		}
	}
}

void StaticData::parseFishTide()
{
	string path = this->getValueForKey("fish_tide_path").asString();
	auto valueMap = FileUtils::getInstance()->getValueMapFromFile(path);

	for(auto value : valueMap)
	{
		int id = stoi(value.first);
		auto fishTide = value.second.asValueMap();
		FishTideConfig config;

		config.count = fishTide.at("count").asInt();
		config.path_id = fishTide.at("path_id").asInt();
		auto fishBornVec = fishTide.at("fishes").asValueVector();

		for(auto bornMap:fishBornVec)
		{
			auto temp = bornMap.asValueMap();

			Point startPos = PointFromString(temp.at("pos").asString());
			int fishID = temp.at("fish_id").asInt();

			config.fishes.push_back(FishBornConfig(startPos,fishID));
		}
		m_fishTideConfigs.insert(make_pair(id,config));
	}
}

void StaticData::parseChestReward()
{
	//获取路径
	auto path = this->getValueForKey("chest_reward_path").asString();
	//获取value
	auto valueVector = FileUtils::getInstance()->getValueVectorFromFile(path);
	//parse
	for(auto value:valueVector)
	{
		auto valueMap = value.asValueMap();

		ChestReward reward;
		reward.type = valueMap.at("type").asString();
		reward.number = valueMap.at("number").asInt();

		m_chestRewards.push_back(reward);
	}
}

void StaticData::parseFishReward()
{
	stringstream text;
	string line;
	//加载fish path对应信息
	auto path = this->getValueForKey("fish_reward_csv_path").asString();
	//重置信息
	text.str(FileUtils::getInstance()->getDataFromFile(path));
	int n = 0;

	while(getline(text,line))
	{
		//去掉前两行和为空的行
		if(n == 0 || line.empty())
		{
			n++;
			continue;
		}
		ValueVector value = StringUtils::split(line,',');
		
		int fishID = value.at(0).asInt();
		string type = value.at(1).asString();
		int number = value.at(2).asInt();
		float success = value.at(3).asFloat();
		//把相关信息放入vector中
		m_fishRewardVec.push_back(FishReward(fishID,type,number,success));
	}
}