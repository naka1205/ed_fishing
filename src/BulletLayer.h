#ifndef __NetGunBulletLayer_H__
#define __NetGunBulletLayer_H__

#include<list>
#include "SG_Engine.h"

using namespace std;
using namespace SDL;

class NetGunBullet;
class Bullet;
class Fish;

class BultLayerDelegate
{
public:
	virtual ~BultLayerDelegate(){}
	virtual void shootAt(Bullet*bullet,Fish*fish)=0;
};

class BulletLayer:public Layer
{
private:
	BultLayerDelegate*m_pDelegate;
	list<Bullet*> m_bullets;
public:
	BulletLayer();
	~BulletLayer();
	CREATE_FUNC(BulletLayer);
	bool init();

	void update(float dt);
	//创建网枪弹
	void addNetGunBullet(int level,const Point&bornPos,const Point&loc,float rotation=0.f);
	//创建加强弹
	void addStrengthenBullet(const Point&bornPos,const Point&loc,float rotation = 0.f);
	//创建迷雾弹
	void addFogBullet(const Point&bornPos,const Point&loc,float rotation = 0.f);
	void setDelegate(BultLayerDelegate*pDelegate);
};
#endif