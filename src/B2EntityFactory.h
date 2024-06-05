#ifndef __B2EntityFactory_H__
#define __B2EntityFactory_H__

#include <list>
#include "SG_Engine.h"

using namespace std;
using namespace SDL;

class b2Entity;
class Fish;
class NetGunBullet;
class FishingNet;
class Laser;
class SkillManager;
class StarFish;
class Bullet;
class Pearl;
class Fog;

class B2EntityFactory
{
private:
	static B2EntityFactory *s_pInstance;
	list<Fish *> m_fishesPool; // 鱼缓冲池
	SkillManager *m_pSkillManager;

private:
	B2EntityFactory();
	~B2EntityFactory();

public:
	static B2EntityFactory *getInstance();
	void purge();
	bool init();
	// 根据id生成对应的鱼
	Fish *createFishForID(int id);
	// 生成海星
	StarFish *createStarFish(float startTime);
	// 根据等级生成对应的网枪弹
	NetGunBullet *createNetGunBulletForLv(int level);
	// 生成迷雾弹
	Bullet *createFogBullet();
	// 生成渔网
	FishingNet *createFishingNet(int level);
	// 生成强化弹
	Bullet *createStrengthenBullet();
	// 生成激光
	Laser *createLaser();
	// 创建珍珠
	Pearl *createPearl();
	// 生成迷雾
	Fog *createFog(Fish *fish);
	// 把鱼对象放入缓冲池
	void pushFishToPool(Fish *fish);
};
#endif