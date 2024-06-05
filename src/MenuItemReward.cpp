#include "MenuItemReward.h"

MenuItemReward::MenuItemReward()
	:m_pNormalSprite(nullptr)
	,m_pToggleSprite(nullptr)
	,m_pUpperSprite(nullptr)
{
}

MenuItemReward::~MenuItemReward()
{
}

MenuItemReward*MenuItemReward::create(Node*upperSprite,const ChestReward&reward)
{
	MenuItemReward*item = new MenuItemReward();

	if(item && item->init(upperSprite,reward))
		item->autorelease();
	else
		SDL_SAFE_DELETE(item);

	return item;
}

bool MenuItemReward::init(Node*upperSprite,const ChestReward&reward)
{
	m_pNormalSprite = Sprite::createWithSpriteFrameName("chest.png");
	m_pToggleSprite = Sprite::createWithSpriteFrameName("chest_opened.png");
	m_pUpperSprite = upperSprite;

	this->addChild(m_pNormalSprite);
	this->addChild(m_pToggleSprite);
	this->addChild(m_pUpperSprite);

	Size size = m_pNormalSprite->getContentSize();
	this->setContentSize(size);

	m_pNormalSprite->setPosition(size.width/2,size.height/2);
	m_pToggleSprite->setPosition(size.width/2,size.height/2);
	m_pUpperSprite->setPosition(size.width/2,0);

	m_pToggleSprite->setVisible(false);

	m_chestReward = reward;

	return true;
}

float MenuItemReward::hideUpperSprite(float duration)
{
	Size size = this->getContentSize();

	MoveBy*move = MoveBy::create(duration,Point(0,size.height/2));
	Hide*hide = Hide::create();

	auto seq = Sequence::create(move,hide,nullptr);

	m_pUpperSprite->runAction(seq);

	return duration;
}

float MenuItemReward::showUpperSprite(float duration)
{
	Size size = this->getContentSize();

	MoveBy*move = MoveBy::create(duration,Point(0,-size.height/2));
	Show*show = Show::create();

	auto seq = Sequence::create(show,move,nullptr);

	m_pUpperSprite->runAction(seq);

	return duration;
}

void MenuItemReward::toggleSprite()
{
	m_pNormalSprite->setVisible(false);
	m_pToggleSprite->setVisible(true);
}

ChestReward& MenuItemReward::getChestReward()
{
	return m_chestReward;
}

void MenuItemReward::selected()
{
	MenuItem::selected();
}

void MenuItemReward::unselected()
{
	MenuItem::unselected();
}