#include "NumberSprite.h"

NumberSprite::NumberSprite()
	:m_nNumber(0),m_pSprite(nullptr),
	m_pLabel(nullptr)
{
}

NumberSprite::~NumberSprite()
{
}

NumberSprite*NumberSprite::create(unsigned int number)
{
	auto sprite = new NumberSprite();

	if(sprite && sprite->init(number))
		sprite->autorelease();
	else
		SDL_SAFE_DELETE(sprite);

	return sprite;
}

bool NumberSprite::init(unsigned int number)
{
	this->setNumber(number);

	return true;
}

void NumberSprite::setNumber(unsigned int number)
{
	m_nNumber = number;
	//"无穷" 精灵
	if(number == MAX_UINT)
	{
		this->handleSprite();
	}
	else
	{
		this->handleLabel();
	}
}

unsigned int NumberSprite::getNumber()const
{
	return m_nNumber;
}

void NumberSprite::handleSprite()
{
	if(m_pSprite == nullptr)
	{
		m_pSprite = Sprite::createWithSpriteFrameName("fonts_laser_num01.png");

		this->addChild(m_pSprite);
	}
	else
	{
		m_pSprite->setVisible(true);
	}

	Size size = m_pSprite->getContentSize();
	m_pSprite->setPosition(size.width/2.f,size.height/2.f);

	this->setContentSize(size);
	//处理label
	if(m_pLabel)
	{
		m_pLabel->setVisible(false);
	}
}

void NumberSprite::handleLabel()
{
	string text = to_string(m_nNumber);

	if(m_pLabel == nullptr)
	{
		m_pLabel = LabelAtlas::create(text,"fonts_laser_num00.png",22,32,'0');
		this->addChild(m_pLabel);
	}
	else
	{
		m_pLabel->setVisible(true);
		m_pLabel->setString(text);
	}
	Size size = m_pLabel->getContentSize();
	m_pLabel->setPosition(size.width/2.f,size.height/2.f);
	this->setContentSize(size);

	if(m_pSprite)
	{
		m_pSprite->setVisible(false);
	}
}