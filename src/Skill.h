#ifndef __Skill_H__
#define __Skill_H__

#include "SG_Engine.h"
using namespace SDL;

class Fish;

class Skill
{
	SDL_SYNTHESIZE(float,m_rate,Rate);//成功发动的几率
	SDL_SYNTHESIZE(float,m_duration,Duration);//技能持续时间
public:
	Skill()
		:m_rate(0.f),m_duration(0.f)
	{}

	virtual void skillEnter(Fish*fish)=0;
	virtual void skillExit(Fish*fish)=0;
};
//-----------------------SkillNothing--------------------------
class SkillNothing:public Skill
{
public:
	SkillNothing(float rate,float duration);

	virtual void skillEnter(Fish*fish);
	virtual void skillExit(Fish*fish);
};
//-------------------------SkillSpeedUp---------------------------------
class SkillSpeedUp:public Skill
{
public:
	SkillSpeedUp(float rate,float duration);
	virtual void skillEnter(Fish*fish);
	virtual void skillExit(Fish*fish);
};
//------------------------SkillScared-----------------------------------
class SkillScared:public Skill
{
public:
	SkillScared(float rate);
	virtual void skillEnter(Fish*fish);
	virtual void skillExit(Fish*fish);
};
//------------------------SkillInvulnerable---------------------------------
class SkillInvulnerable:public Skill
{
public:
	SkillInvulnerable(float rate,float duration);

	virtual void skillEnter(Fish*fish);
	virtual void skillExit(Fish*fish);
};
#endif