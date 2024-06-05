#ifndef __PanelLayer_H__
#define __PanelLayer_H__
#include<string>
#include<vector>

#include "SG_Engine.h"

using namespace SDL;
using namespace std;

class ItemButton;
class GoldCounter;

class PanelLayerDelegate
{
public:
	virtual ~PanelLayerDelegate(){}
	virtual void addCannonLevel()=0;
	virtual void subCannonLevel()=0;
	virtual void pauseBtnCallback()=0;
	virtual void changeBullet(ItemButton*button,bool oldVisible,bool visible)=0;
	virtual void addEnergy(float delta)=0;
};

class PanelLayer:public Layer
{
private:
	Sprite*m_pDownUI;//底层ui
	Sprite*m_pEnergyUI;//能量槽，负责动画
	GoldCounter*m_pGoldCounter;
	LabelAtlas*m_pLvUI;

	ui::Button*m_pSubLevelBtn;
	ui::Button*m_pAddLevelBtn;
	ui::Button*m_pPauseBtn;
	//能量槽唤醒条
	ProgressTimer*m_pEnergyProgress;
	ProgressTimer*m_pExpProgress;

	bool m_bBulletsVisible;//子弹按钮集合是否已经显示
	ItemButton*m_pSelectedBullet;//当前选中的子弹
	ItemButton*m_pHalfEnergyBtn;//半份能量添加

	Menu*m_pBulletMenu;//子弹按钮集合
	Menu*m_pBtnMenu;//按钮集合

	PanelLayerDelegate*m_pDelegate;
public:
	PanelLayer();
	~PanelLayer();
	CREATE_FUNC(PanelLayer);
	bool init();

	//获取炮塔出生的位置
	Point getConnonBornPos()const;

	void setDelegate(PanelLayerDelegate*pDelegate);
	//运动能量动画
	void runEnergyAnimation();
	//停止能量动画
	void stopEnergyAnimation();
	//显示add sub按钮
	void showAddAndSubBtn();
	//隐藏add sub按钮
	void hideAddAndSubBtn();
	//更新能量槽
	void updateEnergy();
	//更新金币数量
	void updateGold();
	//更新等级
	void updateLevel();
	//更新经验值
	void updateExp();
	//更新全部
	void updateSelf();
	void updateBulletBtns();//更新子弹按钮
	void updateHalfEnergyBtn();
	//设置选中当前子弹
	void setSelectedBulletBtn(const string&name);
	void setSelectedBulletBtn(ItemButton*button);
	//设置按钮是否可点击
	void setBtnEnable(bool enable);
private:
	void initBullets();

	void addBtnCallback(Object*sender);
	void subBtnCallback(Object*sender);
	void pauseBtnCallback(Object*sender);

	void handleSuperWeapon(ItemButton*button);
	void handleHalfEnergy(Object*sender);
	void handleBullet(Object*sender);
};
#endif