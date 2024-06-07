#include "Fish.h"
#include "FishingJoyData.h"
#include "StaticData.h"
#include "Skill.h"

const int Fish::FISH_ANIMATION_TAG = 1;

Fish::Fish()
		: m_nID(0), m_state(State::None), m_speed(0.f), m_duration(0.f), m_triggingType(TriggingType::None), m_bInvulnerable(false), m_deadCallback(nullptr)
{
	this->setType(Entity::Type::Fish);
}
Fish::~Fish()
{
}
Fish *Fish::create(int id)
{
	auto fish = new Fish();

	if (fish && fish->init(id))
		fish->autorelease();
	else
		SDL_SAFE_DELETE(fish);

	return fish;
}
bool Fish::init()
{
	Fish::init(1);

	return true;
}
bool Fish::init(int id)
{
	this->setID(id);

	this->setFishType(FishType::Fish);

	auto spriteFormat = STATIC_DATA_STRING("fish_sprite_format");
	auto spriteName = StringUtils::format(spriteFormat.c_str(), id);

	this->bindSpriteWithSpriteFrameName(spriteName);

	string anim_format = STATIC_DATA_STRING("fish_run_anim_format");
	auto animationName = StringUtils::format(anim_format.c_str(), id);
	Animation *animation = AnimationCache::getInstance()->getAnimation(animationName);
	Animate *animate = Animate::create(animation);
	animate->setTag(FISH_ANIMATION_TAG);

	getSprite()->runAction(animate);

	return true;
}

void Fish::setSkills(const map<TriggingType, Skill *> &skills)
{
	for (auto iter = skills.begin(); iter != skills.end(); iter++)
	{
		m_skills.insert(make_pair(iter->first, iter->second));
	}
}

bool Fish::isInvulnerable() const
{
	if ((m_state == State::Skill && m_triggingType == TriggingType::NetGunBullet) || m_bInvulnerable)
		return true;

	return false;
}

void Fish::setInvulnerable(bool invulnerable)
{
	m_bInvulnerable = invulnerable;
}

void Fish::setDeadCallback(const function<void(Fish *)> &callback)
{
	m_deadCallback = callback;
}

void Fish::update(float dt)
{
	if (m_state == State::Skill)
	{
		m_duration -= dt;

		if (m_duration <= 0.f)
		{
			auto skill = m_skills.at(m_triggingType);
			skill->skillExit(this);

			m_triggingType = TriggingType::None;
			m_state = State::Running;
		}
	}
}

void Fish::reset()
{
}

void Fish::clear()
{
	this->setSpeed(1.f);

	this->stopAllActions();
	this->getSprite()->stopAllActions();

	this->setState(State::None);
	this->setActive(false);
	this->setDead(true);
	this->setInvulnerable(false);

	m_skills.clear();
}

void Fish::caught()
{

	string anim_format = STATIC_DATA_STRING("fish_dead_anim_format");
	auto animationName = StringUtils::format(anim_format.c_str(), m_nID);

	Animation *animation = AnimationCache::getInstance()->getAnimation(animationName);
	Animate *animate = Animate::create(animation);
	animate->setTag(FISH_ANIMATION_TAG);

	this->getSprite()->stopActionByTag(FISH_ANIMATION_TAG);

	this->getSprite()->runAction(animate);

	this->setState(Fish::State::Caught);

	// this->setActive(false);

	DelayTime *delayTime = DelayTime::create(animate->getDuration());
	CallFunc *end = CallFunc::create(SDL_CALLBACK_0(Fish::dead, this));

	this->stopAllActions();

	auto seq = Sequence::create(delayTime, end, NULL);

	this->runAction(seq);
}
void Fish::collided(TriggingType type)
{
	auto afterHP = m_nHP - 1;

	if (afterHP <= 0)
	{
		afterHP = 0;
	}

	this->setHP(afterHP);

	if (m_triggingType != TriggingType::None || m_state == State::Caught)
		return;

	auto iter = m_skills.find(type);

	if (iter == m_skills.end())
		return;

	Skill *skill = iter->second;

	float success = RANDOM_0_1();

	if (success < skill->getRate())
	{
		m_state = State::Skill;
		m_triggingType = type;

		skill->skillEnter(this);
	}
}

void Fish::dead()
{
	this->setDead(true);

	if (m_deadCallback != nullptr)
	{
		m_deadCallback(this);
	}
}