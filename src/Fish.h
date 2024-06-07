#ifndef __Fish_H__
#define __Fish_H__
#include <map>
#include <string>
#include <functional>
#include "B2Entity.h"

using namespace std;

enum class TriggingType
{
	None,
	NetGunBullet, // 和网枪弹发生碰撞
	Fishing,			// 和捕鱼类型的发生碰撞
	GroupCome,		// 鱼潮来临
};

enum class FishType
{
	Fish,
	StarFish
};

class Skill;

class Fish : public B2Entity
{
public:
	enum class State
	{
		None,
		Running, // 运动状态
		Skill,	 // 技能状态
		Caught,	 // 被捕捉状态
	};
	static const int FISH_ANIMATION_TAG;

	SDL_SYNTHESIZE(unsigned int, m_nID, ID);
	SDL_SYNTHESIZE(State, m_state, State);					// 当前鱼的状态
	SDL_SYNTHESIZE(float, m_speed, Speed);					// 速度
	SDL_SYNTHESIZE(float, m_duration, Duration);		// 技能持续时间
	SDL_SYNTHESIZE(FishType, m_fishType, FishType); // 鱼的类型
private:
	map<TriggingType, Skill *> m_skills;

	TriggingType m_triggingType;
	bool m_bInvulnerable;									 // 是否是无敌
	function<void(Fish *)> m_deadCallback; // 死亡回调函数
public:
	Fish();
	~Fish();
	CREATE_FUNC(Fish);
	static Fish *create(int id);
	bool init();
	bool init(int id);
	void setSkills(const map<TriggingType, Skill *> &skills);
	bool isInvulnerable() const;
	void setInvulnerable(bool invulnerable);
	void setDeadCallback(const function<void(Fish *)> &callback);

public:
	virtual void update(float dt);
	virtual void reset();
	virtual void clear();

	virtual void caught();										// 被捕捉回调函数
	virtual void collided(TriggingType type); // 碰撞回调函数

	// 死亡回调函数
	void dead();
};
#endif