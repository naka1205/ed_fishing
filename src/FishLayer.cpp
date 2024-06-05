#include "FishLayer.h"
#include "StaticData.h"
#include "FishingJoyData.h"
#include "B2EntityFactory.h"
#include "RotateAndSpeed.h"

FishLayer::FishLayer()
	:m_pDelegate(nullptr),m_state(State::Normal)
	,m_makeFishElapsed(0.f),m_groupComeElapsed(0.f)
	,m_fishTideCount(0),m_fishTidePathID(0)
	,m_starFishElapsed(0.f),m_starFishNum(0)
{
}

FishLayer::~FishLayer()
{
}

bool FishLayer::init()
{
	m_starFishNum = 5;

	return true;
}

void FishLayer::update(float dt)
{
	for(auto iter = m_fishes.begin();iter != m_fishes.end();)
	{
		auto fish = *iter;
		auto curState = fish->getState();
		//当前已经被捕捉，则不再进行碰撞
		if(curState == Fish::State::Caught && fish->isActive())
		{
			fish->setActive(false);
		}

		if(fish->isDead())
		{
			//发送死亡消息
/*			auto uniqueID = to_string(fish->getUniqueID());
			_eventDispatcher->dispatchCustomEvent(uniqueID);*/

			if(curState == Fish::State::Caught)
			{
				m_pDelegate->fishCaughtCallback(fish);
			}
			fish->setActive(false);
			//移除
			iter = m_fishes.erase(iter);
			//把鱼对象放入缓冲池，便于以后使用
			if(fish->getFishType() == FishType::Fish)
			{
				B2EntityFactory::getInstance()->pushFishToPool(fish);
			}
			else//移除,不参与对象重用
			{
				fish->removeFromParent();
			}
		}
		else
		{
			fish->update(dt);
			iter++;
		}
	}
	this->updateOfState(dt);
	this->updateStarFishes(dt);
}
std::vector<Fish*> FishLayer::getFishes()const
{
	return m_fishes;
}

void FishLayer::setDelegate(FishLayerDelegate*pDelegate)
{
	m_pDelegate = pDelegate;
}

void FishLayer::reset()
{
	m_weightOfFishes.clear();

	auto vec = m_pDelegate->getShownFishIdVec();
	for(auto id:vec)
	{
		auto weight = STATIC_FISH_WEIGHT(id);

		m_weightOfFishes.insert(make_pair(id,weight));
	}
}

FishLayer::State FishLayer::getCurState()const
{
	return m_state;
}

void FishLayer::updateOfState(float dt)
{
	switch(m_state)
	{
	case State::GroupCome:this->groupCome();break;
	case State::Group:this->duringGroupState();break;
	case State::Normal:
		{
			this->duringNormalState(dt);

			m_groupComeElapsed+= dt;

			auto time = STATIC_DATA_INT("group_come_time");
			//切换状态
			if(m_groupComeElapsed >= time )
			{
				m_state = State::GroupCome;
				m_groupComeElapsed = 0.f;
				//播放鱼潮来临 动作
				m_pDelegate->showGroupComeMoving();
			}
		}break;
	}

}

void FishLayer::groupCome()
{
	//如果鱼都离开，则转换状态
	if (m_fishes.empty())
	{
		m_state = State::Group;
		//随机获取鱼潮 todo
		int fishTideID = m_pDelegate->getRandomFishTideID();
		auto&fishTideConfig = StaticData::getInstance()->getFishTideByID(fishTideID);
		//获取鱼潮次数
		m_fishTideCount = fishTideConfig.count;
		//获取鱼潮的路径id
		m_fishTidePathID = fishTideConfig.path_id;
		//获取鱼潮的鱼的信息
		m_fishTideConfig = fishTideConfig.fishes;
		//出现鱼潮 特效
		m_pDelegate->showWaveAction();

		return;
	}

	for(auto iter = m_fishes.begin();iter != m_fishes.end();iter++)
	{
		auto fish = *iter;

		fish->collided(TriggingType::GroupCome);
	}
}

void FishLayer::duringGroupState()
{
	//鱼潮的一波是否结束
	if(m_fishTideConfig.empty() && m_fishes.empty())
	{
		m_fishTideCount--;
		//重置新的鱼潮
		if(m_fishTideCount > 0)
		{
			//todo
			auto&fishTideConfig = StaticData::getInstance()->getFishTideByID(1);
			m_fishTideConfig = fishTideConfig.fishes;

			m_fishTidePos = Point::ZERO;
		}
		else//鱼潮结束
		{
			m_state = State::Normal;
			return;
		}
	}
	m_fishTidePos.x += 1.f;
	//是否反向
	bool reverse = m_fishTideCount%2 == 0;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//查看是否有鱼登场
	for(auto iter = m_fishTideConfig.begin();iter != m_fishTideConfig.end();)
	{
		auto config = *iter;
		//生成鱼
		if(m_fishTidePos.x >= config.pos.x)
		{
			//从列表中删除
			iter = m_fishTideConfig.erase(iter);
			//生成鱼
			Fish*fish = B2EntityFactory::getInstance()->createFishForID(config.fishID);
			//确定出生位置
			Point pos = config.pos;
			Size size = fish->getContentSize();

			if(reverse)
				pos.x = visibleSize.width + size.width/2;
			else
				pos.x = -size.width/2;

			//确定动作
			auto action = StaticData::getInstance()->getActionByPathID(m_fishTidePathID,size,0,0,reverse);
			
			fish->setPosition(pos);
			fish->runAction(action);

			this->addChild(fish);
			m_fishes.push_back(fish);
		}
		else
			iter++;
	}
}

void FishLayer::duringNormalState(float dt)
{
	m_makeFishElapsed += dt;

	if(m_makeFishElapsed < 1.f)
		return ;
	m_makeFishElapsed -= 1.f;

	auto fishMaxNum = STATIC_DATA_INT("fish_max_num");
	//如果鱼的数量达到上限
	if(m_fishes.size() >= fishMaxNum)
		return;

	map<int,int> weightOfFishes;

	//除去场上某类已经达到上限的鱼的id
	for(auto iter = m_weightOfFishes.begin();iter != m_weightOfFishes.end();iter++)
	{
		auto id = iter->first;
		int shownNum = this->getCountOfShownByID(id);
		//获取出场的最大数量
		int max = STATIC_FISH_MAX(id);

		if(max > shownNum)
		{
			weightOfFishes.insert(make_pair(iter->first,iter->second));
		}
	}

	//没有可上场的鱼
	if(weightOfFishes.empty())
		return ;
	//获取总加权数
	int sumWeight = 0;
	for(auto iter = weightOfFishes.begin();iter != weightOfFishes.end();iter++)
	{
		sumWeight += iter->second;
	}

	//当前能出场的最大数量
	int shownMax = 0;
	//获取当前权数
	int weight = rand()%sumWeight;

	int id = this->getIDForWeight(weight,weightOfFishes);
	//获取场上的该id的鱼的数目
	int shownNum = this->getCountOfShownByID(id);

	//获取出场的最大数量
	int max = STATIC_FISH_MAX(id);

	shownMax = max - shownNum;
	//获取每次能出现的最大值
	int maxPerTime = STATIC_FISH_MAX_PER_TIME(id);
	//根据当前的最大值和每次能出现的最大值获取最小的那个
	shownMax = std::min(shownMax,maxPerTime);

	//根据鱼的id随机获取一个路径id
	int pathID = this->getRandomPathID(id);
	//是否反向
	bool reverse = rand()%2 == 1;

	//获取到鱼的数目
	int count = rand()%shownMax + 1;

	Point bornPos = StaticData::getInstance()->getStartPosByPathID(pathID,reverse);

	for(int i = 0;i < count;i++)
	{
		Point offset;
		float randomY = rand()%200 - 100;

		Fish*fish = B2EntityFactory::getInstance()->createFishForID(id);
		Size size = fish->getContentSize();
		//设置开始位置
		if(i != 0)
			offset = this->getRandonOffsetPos();

		if(reverse)
			fish->setPosition(bornPos + offset + Point(size.width/2,size.height/2));
		else
			fish->setPosition(bornPos -offset - Point(size.width/2,size.height/2));

		//根据路径id获取动作
		RotateAndSpeed* action = StaticData::getInstance()->getActionByPathID(pathID,size,offset.x,randomY,reverse);

		fish->runAction(action);

		this->addChild(fish);
		m_fishes.push_back(fish);
	}
}

void FishLayer::updateStarFishes(float dt)
{
	//只有在海星全部死亡后才进行倒计时
	if(m_starFishNum != 5)
		return;
	auto time = STATIC_DATA_INT("star_fish_interval");
	m_starFishElapsed += dt;
	//在流逝n秒后产生海星
	if(m_starFishElapsed >= time)
	{
		m_starFishNum = 0;
		m_starFishElapsed -= time;

		auto posFormat = STATIC_DATA_STRING("star_fish_pos_format");
		auto &startTimes = m_pDelegate->getStartFishStartTimeVec();

		for(int i=0;i < 5;i++)
		{
			auto text = StringUtils::format(posFormat.c_str(),i+1);
			//获取出生位置
			Point pos = STATIC_DATA_POINT(text);
			//获取开始时间
			float startTime = startTimes.at(i);
			//生成
			auto fish = B2EntityFactory::getInstance()->createStarFish(startTime);
			fish->setPosition(pos);
			fish->setDelegate(this);

			this->addChild(fish);
			m_fishes.push_back(fish);
		}
	}
}

int FishLayer::getIDForWeight(int weight,const map<int,int>&weights)
{
	int curWeight = 0;

	for(auto iter = weights.begin();iter != weights.end();iter++)
	{
		if(curWeight <= weight && weight <= curWeight + iter->second)
			return iter->first;

		curWeight += iter->second;
	}

	return 0;
}

int FishLayer::getCountOfShownByID(int id)const
{
	int count = 0;

	for(auto iter = m_fishes.begin();iter != m_fishes.end();iter++)
	{
		auto fish = *iter;
		if(id == fish->getID())
			count++;
	}

	return count;
}

Point FishLayer::getRandonOffsetPos()
{
	//todo
	Size size = Size(100,100);

	int x = RANDOM_0_1() * size.width;
	int y = RANDOM_0_1() * size.height;

	return Point(x,y);
}

int FishLayer::getRandomPathID(int fishID)const
{
	const vector<int>* path = StaticData::getInstance()->getTotalPathes(fishID);

	SDL_assert(!path->empty());

	int size = path->size();

	int pathID = rand()%size;

	return path->at(pathID);
}

void FishLayer::starFishDeadCallback()
{
	m_starFishNum++;
	printf("star fish dead\n");
}