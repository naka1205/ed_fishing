#include "StarFish.h"
#include "StaticData.h"

StarFish::StarFish()
	:m_elpased(0.f),m_index(0)
	,m_dirty(true),m_deadTime(0.f)
	,m_pDelegate(nullptr)
{
	m_durations.push_back(1.f);
	m_durations.push_back(0.4f);
	m_durations.push_back(1.f);
}

StarFish::~StarFish()
{
}

StarFish*StarFish::create(float startTime)
{
	auto fish = new StarFish();

	if(fish && fish->init(startTime))
		fish->autorelease();
	else
		SDL_SAFE_DELETE(fish);

	return fish;
}

bool StarFish::init(float startTime)
{
	//设置类型
	this->setFishType(FishType::StarFish);
	//设置id
	this->setID(0);

	m_elpased = startTime;
	//绑定精灵
	auto spriteFrameName = STATIC_DATA_STRING("star_fish_sprite_name");

	this->bindSpriteWithSpriteFrameName(spriteFrameName);
	//根据当前startTime确定不透明度
	float time = startTime/m_durations.at(0);
	Uint8 opacity = Uint8(time * 255);

	this->getSprite()->setOpacity(opacity);
	//设置存活时间
	m_deadTime = STATIC_DATA_INT("star_fish_live_time");

	return true;
}

void StarFish::dead()
{
	this->setDead(true);
	//回调，证明自己死亡
	m_pDelegate->starFishDeadCallback();
}

void StarFish::setDelegate(StarFishDelegate*pDelegate)
{
	m_pDelegate = pDelegate;
}

void StarFish::update(float dt)
{
	if(this->isDead() || this->getHP() <= 0)
		return ;

	m_deadTime -= dt;
	if(m_deadTime <= 0.f)
	{
		this->dead();
	}

	m_elpased += dt;

	if(m_elpased >= m_durations.at(m_index))
	{
		m_elpased -= m_durations.at(m_index);
		m_dirty = true;
		m_index = ++m_index%m_durations.size();
	}
	if(!m_dirty)
		return;
	//进行更新
	if(m_index == 0 || m_index == 2)
	{
		float time = m_elpased/m_durations.at(0);
		if(m_index == 2)
			time = 1.f - time;

		Uint8 opacity = Uint8(time * 255);

		this->getSprite()->setOpacity(opacity);
		m_dirty = true;
			
		this->setActive(false);
	}
	else if(m_index == 1)
	{
		m_dirty = false;

		this->setActive(true);
	}
}

void StarFish::reset()
{
}
void StarFish::clear()
{
}

void StarFish::caught()
{
	//获取捕捉动画
	auto deadAnimName = STATIC_DATA_STRING("star_fish_dead_anim_name");
	auto animation = AnimationCache::getInstance()->getAnimation(deadAnimName);

	SDL_assert(animation != nullptr);

	auto anim = Animate::create(animation);
	anim->setTag(Fish::FISH_ANIMATION_TAG);

	//设置类型为被捕捉
	this->setState(State::Caught);
	//动画结束后死亡
	DelayTime*delayTime = DelayTime::create(anim->getDuration());
	CallFunc*end = CallFunc::create([this]()
	{
		this->dead();
	});
	auto seq = Sequence::create(delayTime,end,nullptr);
	//运行动画
	this->getSprite()->runAction(anim);
	this->runAction(seq);
}
void StarFish::collided(TriggingType type)
{
	//海星不受鱼潮的影响
	if (type == TriggingType::GroupCome)
		return;

	auto afterHP = m_nHP - 1;
	
	if(afterHP <= 0)
	{
		afterHP = 0;
	}
	this->setHP(afterHP);
}