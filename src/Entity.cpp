#include "Entity.h"
Entity::Entity()
	:m_pSprite(nullptr),m_type(Type::None)
	,m_nHP(0),m_bDead(false)
{
}
Entity::~Entity()
{
}
void Entity::bindSprite(Sprite*sprite)
{
	if(m_pSprite)
		m_pSprite->removeFromParent();

	m_pSprite = sprite;
	Size size = m_pSprite->getContentSize();

	this->setContentSize(size);
	m_pSprite->setPosition(size.width/2,size.height/2);
	this->addChild(m_pSprite);
}
bool Entity::bindSpriteWithSpriteFrameName(const string&spriteName)
{
	//获取精灵帧
	auto spriteFrame = Director::getInstance()->getSpriteFrameCache()->getSpriteFrameByName(spriteName);
	
	if(spriteFrame)
	{
		Sprite*sprite = Sprite::createWithSpriteFrame(spriteFrame);
		Entity::bindSprite(sprite);

		return true;
	}
	return false;
}
Sprite*Entity::getSprite()const
{
	return m_pSprite;
}
void Entity::collided()
{
	auto afterHP = m_nHP - 1;
	
	if(afterHP <= 0)
	{
		afterHP = 0;
		m_bDead = true;
	}

	this->setHP(afterHP);
}
Animate*Entity::createAnimate(std::string format,int begin,int end,float delayPerUnit,unsigned int loops)
{
	std::vector<SpriteFrame*> frames;
	auto spriteFrameCache = Director::getInstance()->getSpriteFrameCache();
	//添加资源
	//spriteFrameCache->addSpriteFramesWithFile(xml,png);
	for(int i=begin;i<=end;i++)
	{
		auto frame = spriteFrameCache->getSpriteFrameByName(StringUtils::format(format.c_str(),i));
		frames.push_back(frame);
	}
	Animation*animation = Animation::createWithSpriteFrames(frames,delayPerUnit,loops);
	return Animate::create(animation);
}