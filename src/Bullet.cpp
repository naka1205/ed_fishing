#include "Bullet.h"

Bullet::Bullet()
	:m_bMakeFishingEntity(true)
	,m_speed(0.f),m_distance(0.f)
	,m_pCollidedFish(nullptr)
{
	this->setType(Entity::Type::Bullet);
}

bool Bullet::init()
{
	//Ä¬ÈÏÉèÖÃ
	this->setSpeed(400);
	this->setDistance(300);

	return true;
}

float Bullet::getSpeed()const
{
	return m_speed;
}

void Bullet::setSpeed(float speed)
{
	m_speed = speed;
}

float Bullet::getDistance()const
{
	return m_distance;
}

void Bullet::setDistance(float distance)
{
	m_distance = distance;
}

Fish*Bullet::getCollidedFish()const
{
	return m_pCollidedFish;
}

void Bullet::collided(Fish*fish)
{
	Entity::collided();

	m_pCollidedFish = fish;
}