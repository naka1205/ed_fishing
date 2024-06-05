#ifndef __RewardLayer_H__
#define __RewardLayer_H__
#include "SG_Engine.h"
#include "StaticData.h"

using namespace SDL;

class RewardLayerDelegate
{
public:
	virtual ~RewardLayerDelegate(){}
	virtual void gameResume()=0;

	virtual void addItem(const string&itemName,const Point&bornPos,int number)=0;
	virtual void addChestLightAnim(const string&name,const Point&pos)=0;
	virtual void removeChestLightAnim(const string&name)=0;
};

class RewardLayer:public Layer
{
private:
	Menu*m_pChestMenu;
	int m_nChestChgNum;
	RewardLayerDelegate*m_pDelegate;
	//是否已经打开了宝箱，为了保证宝箱只能打开一个
	bool m_bOpenedChest;
public:
	RewardLayer();
	~RewardLayer();
	CREATE_FUNC(RewardLayer);
	bool init();

	void showChest();
	//当前是否可以点击宝箱，为了避免重复点击不用的宝箱
	bool isClickChest();
	void setDelegate(RewardLayerDelegate*pDelegate);
private:
	void chestRewardBtnCallback(Object*sender);
	void goBtnCallback(Object*sender);
	void moveChestSchedule(float);

	ChestReward&getRandomChestReward();
	Node*getNodeByChestReward(const ChestReward&reward);
};
#endif