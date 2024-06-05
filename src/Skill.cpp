#include "Skill.h"
#include "Fish.h"
#include "StaticData.h"

//-------------------------SkillNothing--------------------
SkillNothing::SkillNothing(float rate,float duration)
{
	this->setRate(rate);
	this->setDuration(duration);
}
void SkillNothing::skillEnter(Fish*fish)
{
}

void SkillNothing::skillExit(Fish*fish)
{
}
//------------------------SkillSpeedUp-----------------------
SkillSpeedUp::SkillSpeedUp(float rate,float duration)
{
	this->setRate(rate);
	this->setDuration(duration);
}

void SkillSpeedUp::skillEnter(Fish*fish)
{
	fish->setSpeed(1.3);
	fish->setDuration(this->getDuration());
}

void SkillSpeedUp::skillExit(Fish*fish)
{
	fish->setSpeed(1.f);
}
//------------------------SkillScared-----------------------------------
SkillScared::SkillScared(float rate)
{
	this->setRate(rate);

	//默认持续时间为20 todo
	float duration = 20.f;
	this->setDuration(duration);
}

void SkillScared::skillEnter(Fish*fish)
{
	fish->setDuration(this->getDuration());
	fish->setSpeed(2.5f);
}

void SkillScared::skillExit(Fish*fish)
{
	fish->setSpeed(1.f);
}
//------------------------SkillInvulnerable---------------------------------
SkillInvulnerable::SkillInvulnerable(float rate,float duration)
{
	this->setRate(rate);
	this->setDuration(duration);
}

void SkillInvulnerable::skillEnter(Fish*fish)
{
	fish->setDuration(this->getDuration());
	//尝试获取fish的无敌动画
	auto id = fish->getID();

	auto animationFormat = STATIC_DATA_STRING("fish_collided_enter_anim_format");
	auto animationName = StringUtils::format(animationFormat.c_str(),id);

	auto animation = AnimationCache::getInstance()->getAnimation(animationName);

	SDL_assert(animation != nullptr);

	//停止原先动画
	fish->getSprite()->stopActionByTag(Fish::FISH_ANIMATION_TAG);

	auto animate = Animate::create(animation);
	animate->setTag(Fish::FISH_ANIMATION_TAG);

	fish->getSprite()->runAction(animate);
}

void SkillInvulnerable::skillExit(Fish*fish)
{
	//尝试获取fish的无敌动画
	auto id = fish->getID();

	auto animationFormat = STATIC_DATA_STRING("fish_collided_exit_anim_format");
	auto animationName = StringUtils::format(animationFormat.c_str(),id);

	auto animation = AnimationCache::getInstance()->getAnimation(animationName);

	SDL_assert(animation != nullptr);

	//停止原先动画
	fish->getSprite()->stopActionByTag(Fish::FISH_ANIMATION_TAG);

	auto animate1 = Animate::create(animation);
	//正常动画
	animationFormat = STATIC_DATA_STRING("fish_run_anim_format");
	animationName = StringUtils::format(animationFormat.c_str(),id);

	animation = AnimationCache::getInstance()->getAnimation(animationName);
	Animate*animate2 = Animate::create(animation);

	auto seq = Sequence::create(animate1,animate2,nullptr);
	seq->setTag(Fish::FISH_ANIMATION_TAG);

	fish->getSprite()->runAction(seq);
}