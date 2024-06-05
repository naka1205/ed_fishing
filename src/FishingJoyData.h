#ifndef __SDL_FishingJoy_H__
#define __SDL_FishingJoy_H__
#include<string>

#include "SG_Engine.h"

using namespace SDL;
using namespace std;

class Cannon;

class FishingJoyData:public Object
{
	//SDL_SYNTHESIZE(int,m_nGold,Gold);//金币
	SDL_SYNTHESIZE(float,m_energy,Energy);//当前能量槽的值
	SDL_SYNTHESIZE(int,m_nLevel,Level);//当前等级
	SDL_SYNTHESIZE(int,m_nExp,Exp);//当前经验值
	SDL_SYNTHESIZE(string,m_selectedBullet,SelectedBullet);//设置当前选中的子弹名称
	SDL_SYNTHESIZE(int,m_nSterengthenBullet,SterengthenBullet);//加强弹个数
	SDL_SYNTHESIZE(int,m_nFogBullet,FogBullet);//迷雾子弹
	SDL_SYNTHESIZE(int,m_nPearlBullet,PearlBullet);//珍珠弹
	SDL_SYNTHESIZE(int,m_nHalfEnergyNum,HalfEnergyNum);//half energy
	SDL_SYNTHESIZE(int,m_nTollgateIndex,TollgateIndex);//当前的关卡

	SDL_BOOL_SYNTHESIZE(m_isBeginner,Beginner);//是否是第一次玩
	SDL_BOOL_SYNTHESIZE(m_bChunk,PlayingChunk);//音效
	SDL_BOOL_SYNTHESIZE(m_bMusic,PlayingMusic);//音乐
private:
	static FishingJoyData*m_pInstance;
private:
	int m_nGold;
private:
	FishingJoyData();
	~FishingJoyData();
public:
	static FishingJoyData*getInstance();
	static void purge();

	bool init();
	//立即保存到xml文件
	void flush();
	//重置
	void reset();

	//获取当前等级的总经验值
	int getExpOfCurLevel()const;
	//根据当前渔枪获取对应所需的金币数
	int getGoldByCannon(Cannon*pCannon);
	//获取当前选中的子弹的名称
	unsigned int getBulletNumByName(const string&bulletName);
	//获取当前所选的子弹的数目
	unsigned int getSelectedBulletNum();
	//减少选中的子弹的数目
	void alterSelectedBulletNum(int num);

	int getGold()const;
	void setGold(int gold);
};
#endif