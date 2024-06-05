#include "Pearl.h"
#include "StaticData.h"

Pearl::Pearl()
	:m_speed(0.f)
{
}

Pearl::~Pearl()
{
}

bool Pearl::init()
{
	//绑定精灵
	auto spriteName = STATIC_DATA_STRING("pearl_sprite_name");
	this->bindSpriteWithSpriteFrameName(spriteName);

	m_speed = 6.f;

	return true;
}

void Pearl::setDelat(float rotation)
{
	auto degree = SDL_ANGLE_TO_DEGREE(rotation);

	m_delta.x = SDL_sinf(degree);
	m_delta.y = SDL_cosf(degree) * -1.f;
}

float Pearl::showAt()
{
	return 0.f;
}

void Pearl::collided()
{
	auto afterHP = m_nHP - 1;
	
	if(afterHP <= 0)
	{
		afterHP = 0;
	}

	this->setHP(afterHP);
	//将要死亡，播放死亡动画
	if(afterHP > 0)
	{
		return;
	}
	//播放淡出动画
	FadeOut*fade = FadeOut::create(1.f);

	DelayTime*delayTime = DelayTime::create(fade->getDuration());
	CallFunc*end = CallFunc::create([this]()
	{
		this->setDead(true);
	});
	//运行动画
	auto seq = Sequence::create(delayTime,end,nullptr);

	this->runAction(seq);
	this->getSprite()->runAction(fade);
}

void Pearl::update(float dt)
{
	//进行移动
	bool bChangeDir = false;

	//判断是否和边界发生碰撞
	Size visibleSize = Director::getInstance()->getVisibleSize();

	Rect r1(0,0,visibleSize.width,1);//上边界
	Rect r3(0,visibleSize.height,visibleSize.width,1);//下边界
	Rect r2(0,0,1,visibleSize.height);//左边界
	Rect r4(visibleSize.width,0,1,visibleSize.height);//右边界

	Rect r = this->getBoundingBox();
	//和上下边界碰撞
	if (r1.intersectRect(r) || r3.intersectRect(r))
	{
		bChangeDir = true;
		m_delta.y = -m_delta.y;
	}
	//和左右边界发生碰撞
	if (r2.intersectRect(r) || r4.intersectRect(r))
	{
		bChangeDir = true;
		m_delta.x = -m_delta.x;
	}
	//碰撞到边界，则能量减少1
	if(bChangeDir)
	{
		this->collided();
	}
	//进行移动
	Point nextPos = this->getPosition() + Point(m_speed * m_delta.x,m_speed * m_delta.y);

	this->setPosition(nextPos);
}