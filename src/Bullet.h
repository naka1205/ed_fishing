#ifndef __Bullet_H__
#define __Bullet_H__

#include "B2Entity.h"

enum class BulletType
{
	NetGunBullet,//网枪弹
	StrengthenBullet,//加强弹
	FogBullet,//迷雾弹
};
class Fish;

class Bullet:public B2Entity
{
	SDL_BOOL_SYNTHESIZE(m_bMakeFishingEntity,MakeFishingEntity);//死亡时是否生成渔网
	SDL_SYNTHESIZE(BulletType,m_bulletType,BulletType);//子弹类型
private:
	float m_speed;
	float m_distance;
	Fish*m_pCollidedFish;
public:
	Bullet();
	CREATE_FUNC(Bullet);
	bool init();
public:
	//获取子弹速度
	virtual float getSpeed()const;
	void setSpeed(float speed);
	//获取最大距离
	virtual float getDistance()const;
	void setDistance(float distance);
	Fish*getCollidedFish()const;

	virtual void collided(Fish*fish);
};
#endif