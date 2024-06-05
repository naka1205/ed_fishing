#include "EffectLayer.h"
#include "StaticData.h"
#include "ScoreSprite.h"

EffectLayer::EffectLayer()
{
}

EffectLayer::~EffectLayer()
{
}

bool EffectLayer::init()
{
	return true;
}

void EffectLayer::addGoldAnimation(const Point&pos)
{
	//获取可用的精灵
	Sprite*sprite = Sprite::create();
	this->addChild(sprite);

	//设置精灵帧
	auto spriteFrameName = STATIC_DATA_STRING("gold_sprite_frame_name");
	sprite->setSpriteFrame(spriteFrameName);
	//设置动画
	auto animationName = STATIC_DATA_STRING("gold_anim");
	auto animation = AnimationCache::getInstance()->getAnimation(animationName);
	auto animate = Animate::create(animation);

	sprite->setVisible(true);
	sprite->setPosition(pos);

	//获取目的位置
	auto targetPos = STATIC_DATA_POINT("gold_to_pos");
	auto delta = pos - targetPos;
	float length = delta.length();

	MoveTo*move = MoveTo::create(length/300.f,targetPos);

	CallFunc*end = CallFunc::create([sprite]()
	{
		sprite->removeFromParent();
	});
	auto seq = Sequence::create(move,end,NULL);
	
	//运行动画
	sprite->runAction(animate);
	sprite->runAction(seq);
}

void EffectLayer::addFadeScoreAction(int score,const Point&pos)
{
	ScoreSprite*sprite = ScoreSprite::create(score);
	sprite->setPosition(pos);

	this->addChild(sprite);
	//运行淡出动画
	float duration = sprite->runFadeOutAction();

	MoveBy*move = MoveBy::create(duration,Point(0,-50));
	RemoveSelf*remove = RemoveSelf::create();

	auto seq = Sequence::create(move,remove,nullptr);

	sprite->runAction(seq);
}

void EffectLayer::showWaveAction()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto spriteName = STATIC_DATA_STRING("wave_sprite_name");

	Sprite*s1 = Sprite::createWithSpriteFrameName(spriteName);
	Sprite*s2 = s1->clone();
	this->addChild(s1);
	this->addChild(s2);

	Size size = s1->getContentSize();

	s1->setAnchorPoint(Point::ZERO);
	s1->setPositionX(visibleSize.width);

	s2->setAnchorPoint(Point::ZERO);
	s2->setPosition(visibleSize.width,size.height);
	//添加动画
	MoveBy*move1 = MoveBy::create(4.f,Point(-visibleSize.width - size.width,0));
	RemoveSelf*remove = RemoveSelf::create();

	auto seq1 = Sequence::createWithTwoActions(move1,remove);

	auto seq2 = Sequence::createWithTwoActions(move1->clone(),remove->clone());
	//运行动画
	s1->runAction(seq1);
	s2->runAction(seq2);
}

void EffectLayer::showGroupComeMoving()
{
	auto spriteName = STATIC_DATA_STRING("group_coming_sprite_name");
	Size visibleSize = Director::getInstance()->getVisibleSize();

	Sprite*sprite = Sprite::createWithSpriteFrameName(spriteName);
	this->addChild(sprite);

	Size size = sprite->getContentSize();

	Point bornPos = Point(-size.width/2,visibleSize.height/2);
	MoveBy*move = MoveBy::create(3.f,Point(visibleSize.width + size.width,0));
	RemoveSelf*remove = RemoveSelf::create();
	auto seq = Sequence::createWithTwoActions(move,remove);

	sprite->setPosition(bornPos);
	sprite->runAction(seq);
}

void EffectLayer::addSelectedMark(const string&name,const Point&pos)
{
	auto spriteFrameName = STATIC_DATA_STRING("btn_selected_mark_sprite_name");
	Sprite*sprite = Sprite::createWithSpriteFrameName(spriteFrameName);

	sprite->setName(name);
	sprite->setPosition(pos);
	this->addChild(sprite);

	//运行动画
	auto animationName = STATIC_DATA_STRING("btn_selected_mark_ani_name");
	Animation*animation = AnimationCache::getInstance()->getAnimation(animationName);
	sprite->runAction(Animate::create(animation));
}

void EffectLayer::changeSelectedMark(const string&name,const Point&pos)
{
	auto node = this->getChildByName(name);

	if(node)
	{
		node->setPosition(pos);
	}
}

void EffectLayer::removeSelectedMark(const string&name)
{
	Node*node = this->getChildByName(name);

	if(node)
	{
		node->stopAllActions();
		node->removeFromParent();
	}
}

void EffectLayer::showLevelUpAction()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto spriteName = STATIC_DATA_STRING("level_up_sprite_name");

	Sprite*sprite = Sprite::createWithSpriteFrameName(spriteName);
	
	sprite->setPosition(visibleSize.width/2,visibleSize.height/2);
	this->addChild(sprite);
	//设置动画
	ScaleBy*scale = ScaleBy::create(1.f,1.1f,1.1f);
	FadeOut*fadeOut = FadeOut::create(2.f);
	RemoveSelf*remove = RemoveSelf::create();

	auto seq = Sequence::create(scale,fadeOut,remove,nullptr);

	sprite->runAction(seq);
}

void EffectLayer::addChestLightAnim(const string&name,const Point&pos)
{
	Sprite*sprite = Sprite::createWithSpriteFrameName("chest_light00.png");
	sprite->setPosition(pos);
	sprite->setName(name);
	sprite->setAnchorPoint(Point(0.5f,1.f));

	this->addChild(sprite);
	//设置动画
	auto animation = AnimationCache::getInstance()->getAnimation("chest_light");

	Animate*animate = Animate::create(animation);

	sprite->runAction(animate);
}

void EffectLayer::removeChestLightAnim(const string&name)
{
	auto node = this->getChildByName(name);

	if(node)
	{
		node->stopAllActions();
		node->removeFromParent();
	}
}

float EffectLayer::showBatteryMoving(const Point&bornPos)
{
	Sprite*sprite = Sprite::createWithSpriteFrameName("battery.png");
	sprite->setPosition(bornPos);
	this->addChild(sprite);
	//获取目的位置
	Point targetPos = STATIC_DATA_POINT("battery_to_pos");
	float duration = this->itemMove(sprite,bornPos,targetPos);

	return duration;
}

float EffectLayer::showPearlBulletMoving(const Point&bornPos)
{
	Sprite*sprite = Sprite::createWithSpriteFrameName("bullet_pearl.png");
	sprite->setPosition(bornPos);
	this->addChild(sprite);
	//获取目的位置
	Point targetPos = STATIC_DATA_POINT("bullet_to_pos");
	float duration = this->itemMove(sprite,bornPos,targetPos);

	return duration;
}

float EffectLayer::showFogBulletMoving(const Point&bornPos)
{
	Sprite*sprite = Sprite::createWithSpriteFrameName("bullet_fog.png");
	sprite->setPosition(bornPos);
	this->addChild(sprite);
	//获取目的位置
	Point targetPos = STATIC_DATA_POINT("bullet_to_pos");
	float duration = this->itemMove(sprite,bornPos,targetPos);

	return duration;
}

float EffectLayer::showSterengthenBulletMoving(const Point&bornPos)
{
	Sprite*sprite = Sprite::createWithSpriteFrameName("bullet_strengthen.png");
	
	sprite->setPosition(bornPos);
	this->addChild(sprite);
	//获取目的位置
	Point targetPos = STATIC_DATA_POINT("bullet_to_pos");
	float duration = this->itemMove(sprite,bornPos,targetPos);

	return duration;
}

float EffectLayer::itemMove(Sprite*sprite,const Point&bornPos,const Point&endPos)
{
	auto delta = endPos - bornPos;
	float duration = delta.length()/300.f;

	MoveTo*move = MoveTo::create(duration,endPos);
	RemoveSelf*remove = RemoveSelf::create();
	auto seq = Sequence::createWithTwoActions(move,remove);

	sprite->runAction(seq);

	return duration;
}