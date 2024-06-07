#ifndef __RewardLayer_H__
#define __RewardLayer_H__
#include "SG_Engine.h"
#include "StaticData.h"

using namespace SDL;

class RewardLayerDelegate
{
public:
	virtual ~RewardLayerDelegate() {}
	virtual void gameResume() = 0;

	virtual void addItem(const std::string &itemName, const Point &bornPos, int number) = 0;
	virtual void addChestLightAnim(const std::string &name, const Point &pos) = 0;
	virtual void removeChestLightAnim(const std::string &name) = 0;
};

class RewardLayer : public Layer
{
private:
	Menu *m_pChestMenu;
	int m_nChestChgNum;
	RewardLayerDelegate *m_pDelegate;

	bool m_bOpenedChest;

public:
	RewardLayer();
	~RewardLayer();
	CREATE_FUNC(RewardLayer);
	bool init();

	void showChest();

	bool isClickChest();
	void setDelegate(RewardLayerDelegate *pDelegate);

private:
	void chestRewardBtnCallback(Object *sender);
	void goBtnCallback(Object *sender);
	void moveChestSchedule(float);

	ChestReward &getRandomChestReward();
	Node *getNodeByChestReward(const ChestReward &reward);
};
#endif