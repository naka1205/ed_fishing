#ifndef __Entity_H__
#define __Entity_H__
#include<string>
#include "SG_Engine.h"
using namespace SDL;
using namespace std;

class Entity:public Node
{
public:
	enum class Type
	{
		None,
		Fishing,//捕鱼功能
		Fish,//鱼
		Bullet,//网枪弹
	};
	SDL_SYNTHESIZE(int,m_nHP,HP);//当前血量
	SDL_SYNTHESIZE(Type,m_type,Type);//获取当前实体类型
	SDL_BOOL_SYNTHESIZE(m_bDead,Dead);//是否死亡，如果是则从屏幕上消失
protected:
	Sprite*m_pSprite;
public:
	Entity();
	~Entity();
	Sprite*getSprite()const;
	void bindSprite(Sprite*sprite);
	bool bindSpriteWithSpriteFrameName(const string&spriteName);
	//碰撞处理函数，只要发生碰撞，都会回调这个函数
	virtual void collided();
	//创建动画
	static Animate*createAnimate(std::string format,int begin,int end,float delayPerUnit,unsigned int loops=-1);
};
#endif