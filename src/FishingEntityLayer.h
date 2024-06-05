#ifndef __FishingEntityLayer_H__
#define __FishingEntityLayer_H__

#include<list>
#include "SG_Engine.h"

using namespace SDL;
using namespace std;

class Fish;
class FishingNet;
class FishingEntity;

class FishingEntityLayerDelegate
{
public:
	virtual ~FishingEntityLayerDelegate(){}
	virtual void fireEnd()=0;
};

class FishingEntityLayer:public Layer
{
private:
	list<FishingEntity*> m_fishingEntitys;
	FishingEntityLayerDelegate*m_pDelegate;
public:
	FishingEntityLayer();
	~FishingEntityLayer();
	CREATE_FUNC(FishingEntityLayer);
	bool init();

	void update(float dt);

	void setDelegate(FishingEntityLayerDelegate*pDelegate);
	//添加FishingNet
	void addFishingNet(int lv,const Point&pos,float rotation);
	//添加激光
	void addLaser(const Point&pos,float rotation);
	//添加珍珠弹
	void addPearl(const Point&pos,float rotation);
	//添加迷雾
	void addFog(Fish*fish,const Point&pos,float rotation);
};
#endif