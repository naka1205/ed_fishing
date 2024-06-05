#ifndef __EffectLayer_H__
#define __EffectLayer_H__
#include<string>
#include "SG_Engine.h"

#define EFFECT_ANIMATE_TAG 1
using namespace std;
using namespace SDL;

class EffectLayer:public Layer
{
private:
	Vector<Sprite*> m_sprites;
public:
	EffectLayer();
	~EffectLayer();
	CREATE_FUNC(EffectLayer);
	bool init();
public:
	void addGoldAnimation(const Point&pos);
	void addFadeScoreAction(int score,const Point&pos);
	//鱼潮来临的海浪动画
	void showWaveAction();
	//鱼潮来临 移动动画
	void showGroupComeMoving();
	//添加选中特效
	void addSelectedMark(const string&name,const Point&pos);
	void changeSelectedMark(const string&name,const Point&pos);
	void removeSelectedMark(const string&name);
	//显示升级动画
	void showLevelUpAction();
	//添加宝箱打开特效
	void addChestLightAnim(const string&name,const Point&pos);
	void removeChestLightAnim(const string&name);
	//电池获取的移动特效
	float showBatteryMoving(const Point&bornPos);
	//特殊子弹获取的移动特效
	float showPearlBulletMoving(const Point&bornPos);
	float showFogBulletMoving(const Point&bornPos);
	float showSterengthenBulletMoving(const Point&bornPos);
private:
	float itemMove(Sprite*sprite,const Point&bornPos,const Point&endPos);
};
#endif