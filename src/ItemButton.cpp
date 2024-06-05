#include "ItemButton.h"
#include "NumberSprite.h"

ItemButton::ItemButton()
	:m_pNormalSprite(nullptr),m_pDisabelSprite(nullptr)
	,m_pNumberSprite(nullptr),m_buttonType(ButtonType::None)
{
}

ItemButton::~ItemButton()
{
}

ItemButton*ItemButton::create(Sprite*normalSprite,unsigned int number,ButtonType type)
{
	auto btn = new ItemButton();

	if(btn && btn->init(normalSprite,number,type))
		btn->autorelease();
	else
		SDL_SAFE_DELETE(btn);

	return btn;
}

bool ItemButton::init()
{
	return true;
}

bool ItemButton::init(Sprite*normalSprite,unsigned int number,ButtonType type)
{
	m_buttonType = type;

	m_pNormalSprite = normalSprite;
	m_pDisabelSprite = Sprite::createWithSpriteFrameName("button_disable.png");
	m_pNumberSprite = NumberSprite::create(number);

	Size size = m_pNormalSprite->getContentSize();
	this->setContentSize(size);
	//ÉèÖÃÎ»ÖÃ
	m_pNormalSprite->setPosition(size.width/2.f,size.height/2.f);
	m_pDisabelSprite->setPosition(size.width/2.f,size.height/2.f);
	m_pNumberSprite->setPosition(size.width,0.f);

	this->addChild(m_pNormalSprite,1);
	this->addChild(m_pDisabelSprite,2);
	this->addChild(m_pNumberSprite,5);

	this->updateImagesVisibility();

	return true;
}

void ItemButton::setNormalSprite(Sprite*normalSprite)
{
	if(m_pNormalSprite == normalSprite)
		return;
	if(m_pNormalSprite)
		m_pNormalSprite->removeFromParent();

	m_pNormalSprite = normalSprite;
	Size size = m_pNormalSprite->getContentSize();
	m_pNormalSprite->setPosition(size.width/2,size.height/2);

	this->setContentSize(size);
	this->addChild(m_pNormalSprite,1);
}

Sprite*ItemButton::getNormalSprite()const
{
	return m_pNormalSprite;
}

void ItemButton::setNumber(unsigned int number)
{
	if(m_pNumberSprite == nullptr)
	{
		Size size = this->getContentSize();

		m_pNumberSprite = NumberSprite::create(number);
		m_pNumberSprite->setPosition(size.width,0);

		this->addChild(m_pNumberSprite,5);
	}
	else
		m_pNumberSprite->setNumber(number);
}
unsigned int ItemButton::getNumber()const
{
	return m_pNumberSprite->getNumber();
}

void ItemButton::setButtonType(ButtonType type)
{
	m_buttonType = type;
}

ButtonType ItemButton::getButtonType()const
{
	return m_buttonType;
}

void ItemButton::setEnabled(bool enable)
{
	MenuItem::setEnabled(enable);
	updateImagesVisibility();
}

void ItemButton::selected()
{
	MenuItem::selected();
}

void ItemButton::unselected()
{
	MenuItem::unselected();
}

void ItemButton::updateImagesVisibility()
{
	if(_bEnabled && m_pDisabelSprite)
	{
		m_pDisabelSprite->setVisible(false);
	}
	else if(m_pDisabelSprite)
	{
		m_pDisabelSprite->setVisible(true);
	}
}