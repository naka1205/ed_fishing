#ifndef __NetGunBullet_H__
#define __NetGunBullet_H__
#include "Cannon.h"
#include "B2Entity.h"
#include "Bullet.h"

class NetGunBullet:public Bullet
{
	SDL_SYNTHESIZE(int,m_nLv,Level);//炮弹的等级
public:
	NetGunBullet();
	~NetGunBullet();
	CREATE_FUNC(NetGunBullet);
	static NetGunBullet*create(int lv);

	bool init();
	bool initWithLv(int lv);
	virtual float getSpeed()const;
	//获取最大距离 
	virtual float getDistance()const;
};
#endif