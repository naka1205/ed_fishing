#ifndef __FishLayer_H__
#define __FishLayer_H__
#include<map>
#include<string>
#include<vector>
#include<numeric>
#include "SG_Engine.h"
#include "Fish.h"
#include "StaticData.h"
#include "StarFish.h"

using namespace std;
using namespace SDL;

class Fish;

class FishLayerDelegate
{
public:
	virtual ~FishLayerDelegate(){}
	virtual void showWaveAction()=0;
	virtual vector<int>& getShownFishIdVec()=0;
	virtual int getRandomFishTideID()const=0;
	virtual vector<float>& getStartFishStartTimeVec()=0;
	virtual void fishCaughtCallback(Fish*fish)=0;
	virtual void showGroupComeMoving() = 0;
};

class FishLayer:public Layer,public StarFishDelegate
{
public:
	enum class State
	{
		GroupCome,
		Group,
		Normal,
	};
private:
	std::vector<Fish*> m_fishes;
	map<int,int> m_weightOfFishes;//鱼的权数
	State m_state;
	//生成鱼的流逝时间
	float m_makeFishElapsed;
	//鱼潮还有多久来临
	float m_groupComeElapsed;
	//海星的出现时间
	float m_starFishElapsed;
	//海星的个数
	int m_starFishNum;
	//鱼潮剩余次数
	int m_fishTideCount;
	//鱼潮的路径类型
	int m_fishTidePathID;
	//鱼潮鱼的信息
	vector<FishBornConfig> m_fishTideConfig;
	//当前鱼潮出现位置
	Point m_fishTidePos;

	FishLayerDelegate*m_pDelegate;
public:
	FishLayer();
	~FishLayer();
	CREATE_FUNC(FishLayer);
	bool init();
public:
	virtual void update(float dt);

	std::vector<Fish*> getFishes()const;
	void setDelegate(FishLayerDelegate*pDelegate);

	void reset();
	State getCurState()const;
private:
	void updateOfState(float dt);

	void groupCome();
	void duringGroupState();
	void duringNormalState(float dt);
	//生成海星
	void updateStarFishes(float dt);
	//根据当前的权数和总权数 获取当前权数所对应的id
	int getIDForWeight(int weight,const map<int,int>&weights);
	//获取场上为此id的鱼的数量
	int getCountOfShownByID(int id)const;

	Point getRandonOffsetPos();
	//获取随机的路径id
	int getRandomPathID(int fishID)const;
private:
	virtual void starFishDeadCallback();
};
#endif