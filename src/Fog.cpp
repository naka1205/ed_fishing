#include "Fog.h"
#include "Fish.h"
#include "StaticData.h"

Fog::Fog()
	:m_pFish(nullptr)
{
}

Fog::~Fog()
{
	_eventDispatcher->removeEventListenerForTarget(this);
}

Fog*Fog::create(Fish*fish)
{
	auto fog = new Fog();

	if(fog && fog->init(fish))
		fog->autorelease();
	else
		SDL_SAFE_DELETE(fog);

	return fog;
}

bool Fog::init(Fish*fish)
{
	//绑定精灵
	auto spriteName = STATIC_DATA_STRING("fog_sprite_name");
	this->bindSpriteWithSpriteFrameName(spriteName);
	//运行正常动画
	auto animationName = STATIC_DATA_STRING("fog_run_anim_name");
	auto animation = AnimationCache::getInstance()->getAnimation(animationName);
	auto animate = Animate::create(animation);

	this->getSprite()->runAction(animate);

	m_pFish = fish;

	//绑定消息
	auto uniqueID = to_string(fish->getUniqueID());

	_eventDispatcher->addEventCustomListener(uniqueID,SDL_CALLBACK_1(Fog::fishDeadCallback,this),this);

	return true;
}

void Fog::update(float dt)
{
	auto pos = m_pFish->getPosition();
	//这个跟图片有关
	auto rotation = m_pFish->getSprite()->getRotation();

	this->setPosition(pos);
	this->getSprite()->setRotation(rotation);
}

float Fog::showAt()
{
	return 0.f;
}

void Fog::collided()
{
	//nothing to do
}

void Fog::fishDeadCallback(EventCustom*eventCutom)
{
	this->setActive(false);
	this->setDead(true);
}