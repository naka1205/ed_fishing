#include "RotateAndSpeed.h"
#include "Fish.h"

RotateAndSpeed::RotateAndSpeed()
	:m_pInnerAction(nullptr),m_pTarget(nullptr)
{
}
RotateAndSpeed::~RotateAndSpeed()
{
	SDL_SAFE_RELEASE(m_pInnerAction);
}
RotateAndSpeed*RotateAndSpeed::create(FiniteTimeAction*pAction)
{
	auto action = new RotateAndSpeed();

	if(action && action->init(pAction))
		action->autorelease();
	else
		SDL_SAFE_DELETE(action);

	return action;
}
bool RotateAndSpeed::init(FiniteTimeAction*action)
{
	if(ActionInterval::initWithDuration(action->getDuration()))
	{
		action->retain();
		m_pInnerAction = action;
		return true;
	}

	return false;
}
void RotateAndSpeed::startWithTarget(Node*target)
{
	m_pTarget = static_cast<Fish*>(target);

	m_pInnerAction->startWithTarget(target);

	ActionInterval::startWithTarget(target);
}
bool RotateAndSpeed::isDone()const
{
	return m_pInnerAction->isDone();
}

void RotateAndSpeed::step(float dt)
{
	float speed = m_pTarget->getSpeed();
	//如果播放开启第一次进行时间流逝处理，将时间累和值设置为0
	if(_bFirstTick)
	{
		_elapsed = 0;
		_bFirstTick = false;
	}
	else
	{
		if(-FLT_MIN < speed && speed < FLT_MIN)
			_elapsed += dt;
		else
			_elapsed += dt*speed;
	}
	//调用update更新动画
	this->update(MAX(0,
		MIN(1, _elapsed /MAX(_duration, FLT_EPSILON))));
}

void RotateAndSpeed::update(float time)
{
	//鱼死亡 todo
	if(time == 1.f)
	{
		m_pTarget->dead();

		return;
	}

	Point prePos = _target->getPosition();

	m_pInnerAction->update(time);

	Point curPos = _target->getPosition();
	Point delta = curPos - prePos;

	//如果坐标没发生改变 或者x轴改变为0.f,则直接返回
	if(curPos.x == prePos.x || (delta.x > -FLT_MIN && delta.x < FLT_MIN))
	{
		return;
	}

	float degree = SDL_atan(delta.y/delta.x);
	float angle = SDL_DEGREE_TO_ANGLE(degree);

	if(delta.x < 0)
		angle += 180.f;

	//_target->setRotation(angle);
	Sprite*sprite = m_pTarget->getSprite();
	sprite->setRotation(angle);
}
void RotateAndSpeed::stop()
{
	m_pInnerAction->stop();

	ActionInterval::stop();
}
