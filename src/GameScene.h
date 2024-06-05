#ifndef __GameScene_H__
#define __GameScene_H__
#include<vector>
#include "SG_Engine.h"
#include "box2dx.h"

#include "FishLayer.h"
#include "PanelLayer.h"
#include "TouchLayer.h"
#include "Cannon.h"
#include "BulletLayer.h"
#include "FishingEntityLayer.h"
#include "RewardLayer.h"
#include "PauseLayer.h"
#include "GoldTimer.h"

using namespace std;
using namespace SDL;

class LevelLayer;
class NetGunBullet;
class FishingNet;
class FishingEntity;
class EffectLayer;
class Bullet;
class ItemButton;

enum class GameState
{
	Resume,//恢复
	Pause,//暂停
};

class GameScene:public Scene
	,public PanelLayerDelegate,public TouchLayerDelegate,public CannonDelegate
	,public BultLayerDelegate,public FishingEntityLayerDelegate
	,public FishLayerDelegate,public RewardLayerDelegate
	,public PauseLayerDelegate,public GoldTimerDelegate
	,public b2ContactListener
{
private:
	LevelLayer*m_pLevelLayer;
	TouchLayer*m_pTouchLayer;
	PanelLayer*m_pPanelLayer;
	Cannon*m_pCannon;
	GoldTimer*m_pGoldTimer;
	BulletLayer*m_pBulletLayer;
	FishingEntityLayer*m_pFishingEntityLayer;
	FishLayer*m_pFishLayer;
	RewardLayer*m_pRewardLayer;
	PauseLayer*m_pPauseLayer;

	EffectLayer*m_pEffectLayer;
	//所有的GameScene共享
	static b2World*s_pWorld;
public:
	GameScene();
	~GameScene();
	CREATE_FUNC(GameScene);
	bool init();

	virtual void update(float dt);

	void preloadResources();
	//游戏结束，释放一些单例类
	static void purge();
	//游戏暂停
	void gamePause();
	//游戏恢复
	void gameResume();
	//播放背景音乐
	void playBackgroundMusic();
	static b2World*getWorld();
private:
	void collisionOfBullet(Bullet*bullet,Entity*entity);
	void collisionOfFishingEntity(FishingEntity*fishingNet,Entity*entity);
	//递归恢复或暂停本节点下的所有节点
	void operateAllSchedulerAndActions(Node*node,GameState state);
private:

	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);

private:
	virtual void addCannonLevel();
	virtual void subCannonLevel();
	virtual void pauseBtnCallback();
	virtual void continueGame();
	virtual void turnStartLayer();
	virtual void chooseTollgate();
	virtual void operateMusic();
	virtual void operateChunk();
	//改变子弹
	virtual void changeBullet(ItemButton*button,bool oldVisible,bool visible);

	virtual bool onTouchBegan(Touch*touch,SDL_Event*event);
	virtual void onTouchMoved(Touch*touch,SDL_Event*event);
	virtual void onTouchEnded(Touch*touch,SDL_Event*event);

	virtual void fireCallback(const Point&pos);
	//射击结束
	virtual void fireEnd();
	//发射子弹
	void shootAt(Bullet*bullet,Fish*fish);
	//在一段金币动画后，增加金币数量，同时播放声音
	void addGold(int number,const Point&pos);
	//显示海浪动作，同时播放声音
	void showWaveAction();
	//显示宝箱打开特效
	void addChestLightAnim(const string&name,const Point&pos);
	void removeChestLightAnim(const string&name);
	//减少金币
	bool subGold(int number);
	//增加能量,只是简单地增加了能量
	void addEnergy(float delta);
	//增加经验值，内部会判断是否升级
	void addExp(float exp);
	//增加能量电池
	void addBattery(int count,const Point&bornPos);
	void addBattery(int count);
	//增加珍珠弹
	void addPearlBullet(int count,const Point&bornPos);
	void addPearlBullet(int count);
	//增加迷雾弹
	void addFogBullet(int count,const Point&bornPos);
	void addFogBullet(int count);
	//增加强化弹
	void addStrengthenBullet(int count,const Point&bornPos);
	void addStrengthenBullet(int count);
	//添加道具
	void addItem(const string&itemName,const Point&bornPos,int number);
	//获取关卡鱼潮的id
	int getRandomFishTideID()const;
	//升级
	void levelUp();
	//随机渔网
	void randomMakeFishingNets(float dt,int lv = 1);
	//鱼被捕捉回调函数
	void fishCaughtCallback(Fish*fish);
	//宝箱回调函数
	void showChestCallback(Object*sender);
	//添加渔网函数 简单地封装
	void addFishingNet(int lv,const Point&pos,float rotation);

	virtual vector<int>& getShownFishIdVec();
	virtual vector<float>& getStartFishStartTimeVec();
	virtual void showGroupComeMoving();
};
#endif