#ifndef __MenuItemReward_H__
#define __MenuItemReward_H__

#include "SG_Engine.h"
#include "StaticData.h"

using namespace SDL;

class MenuItemReward:public MenuItem
{
private:
	Sprite*m_pNormalSprite;
	Sprite*m_pToggleSprite;
	Node*m_pUpperSprite;
	ChestReward m_chestReward;
public:
	MenuItemReward();
	~MenuItemReward();
	static MenuItemReward*create(Node*upperSprite,const ChestReward&reward);
	bool init(Node*upperSprite,const ChestReward&reward);

	float hideUpperSprite(float duration);
	float showUpperSprite(float duration);
	//ÇÐ»»¾«Áé
	void toggleSprite();

	ChestReward& getChestReward();
public://override
	virtual void selected();
	virtual void unselected();
};
#endif