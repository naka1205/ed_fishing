#ifndef __Cannon_H__
#define __Cannon_H__
#include<cmath>
#include<string>
#include<vector>
#include<algorithm>
#include "Entity.h"

using namespace std;

class CannonDelegate
{
public:
	virtual ~CannonDelegate(){}
	virtual void fireCallback(const Point&pos)=0;
};
class Cannon:public Node
{
public:
	enum class Type : int
	{
		None,
		Lv1,
		Lv2,
		Lv3,
		Lv4,
		Lv5,
		Lv6,
		Lv7,
		Laser = 11//激光炮塔
	};
	SDL_BOOL_SYNTHESIZE(m_bCanFire,CanFire);//是否可发射，主要是当前的大炮状态是否可发射，和金币等无关
private:
	Type m_type;//炮塔类型
	Sprite*m_pSprite;//炮塔精灵
	vector<Type> m_netBulletTypes;//渔网子弹类型
	Type m_lastType;//保存上一个类型

	CannonDelegate*m_pDelegate;
public:
	Cannon();
	~Cannon();
	CREATE_FUNC(Cannon);
	bool init();
public:
	//炮台朝向
	void aimAt(const Point &target);
	//开火
	void fire(const Point&pos);
	void fireEnd();//发射结束

	void addLevel();
	void subLevel();

	float getRotation()const;

	void setType(Type type);
	Type getType()const;

	void setDelegate(CannonDelegate*pDelegate);
	bool isBelongNetBulletType(Type type);//是否属于渔网子弹类型
	//获取网枪弹等级
	int getNetGunBulletLv()const;
	//获取上一个类型
	Type getLastType()const;
	//设置网枪弹的类型
	void setNetGunBullets(const vector<int>&vec);
private:
	string getSpriteName(Type type);//根据类型获取大炮精灵名称
};
#endif