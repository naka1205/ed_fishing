#include "RotateSpeedSprite.h"
#include "Fish.h"

RotateSpeedSprite::RotateSpeedSprite()
	:m_pInnerAction(nullptr),m_speed(0.f)
{
}
RotateSpeedSprite::~RotateSpeedSprite()
{
	SDL_SAFE_RELEASE(m_pInnerAction);
}
RotateSpeedSprite*RotateSpeedSprite::create(FiniteTimeAction*pAction)
{
	auto action = new RotateSpeedSprite();

	if(action && action->init(pAction))
		action->autorelease();
	else
		SDL_SAFE_DELETE(action);
	return action;
}
bool RotateSpeedSprite::init(FiniteTimeAction*action)
{
	if(ActionInterval::initWithDuration(action->getDuration()))
	{
		action->retain();
		m_pInnerAction = action;
		return true;
	}
	return false;
}
void RotateSpeedSprite::startWithTarget(Node*target)
{
	m_pInnerAction->startWithTarget(target);
	ActionInterval::startWithTarget(target);
}
bool RotateSpeedSprite::isDone()const
{
	return m_pInnerAction->isDone();
}

void RotateSpeedSprite::step(float dt)
{
	//如果播放开启第一次进行时间流逝处理，将时间累和值设置为0
	if(_bFirstTick)
	{
		_elapsed = 0;
		_bFirstTick = false;
	}
	else
	{
		if(-FLT_MIN < m_speed && m_speed < FLT_MIN)
			_elapsed += dt;
		else
			_elapsed += dt*m_speed;
	}
	//调用update更新动画
	this->update(MAX(0,
		MIN(1, _elapsed /MAX(_duration, FLT_EPSILON))));
}

void RotateSpeedSprite::update(float time)
{
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

	//static_cast<Entity*>(_target)->getSprite()->setRotation(-angle);
	_target->setRotation(angle);
}
void RotateSpeedSprite::stop()
{
	m_pInnerAction->stop();
	ActionInterval::stop();
}

void RotateSpeedSprite::setSpeed(float speed)
{
	m_speed = speed;
}

float RotateSpeedSprite::getSpeed()const
{
	return m_speed;
}