#include "ScrollLabel.h"

ScrollLabel::ScrollLabel()
		: m_nCurIndex(0), m_bDown(true), m_pCurSprite(nullptr), m_pNextSprite(nullptr), m_nTargetIndex(0), m_bUpdated(false), m_speed(0.f)
{
}

ScrollLabel::~ScrollLabel()
{
	for (auto iter = m_sprites.begin(); iter != m_sprites.end();)
	{
		auto sprite = *iter;
		sprite->release();

		iter = m_sprites.erase(iter);
	}
}

ScrollLabel *ScrollLabel::create(const vector<Sprite *> &vec, const Size &size, int curIndex)
{
	auto label = new ScrollLabel();

	if (label && label->init(vec, size, curIndex))
		label->autorelease();
	else
		SDL_SAFE_DELETE(label);

	return label;
}

bool ScrollLabel::init(const vector<Sprite *> &vec, const Size &size, int curIndex)
{
	for (auto sprite : vec)
	{
		sprite->retain();
		m_sprites.push_back(sprite);
	}
	this->setContentSize(size);
	// 设置当前的精灵
	m_nCurIndex = m_nTargetIndex = curIndex;

	m_pCurSprite = Sprite::createWithSpriteFrame(m_sprites.at(m_nCurIndex)->getSpriteFrame());
	m_pCurSprite->setPosition(size.width / 2, size.height / 2);
	this->addChild(m_pCurSprite);

	int logicIndex = this->getNexLogicIndex(m_nCurIndex);

	m_pNextSprite = Sprite::createWithSpriteFrame(m_sprites.at(logicIndex)->getSpriteFrame());
	m_pNextSprite->setPosition(size.width / 2, size.height / 2);
	m_pNextSprite->setVisible(false);

	this->addChild(m_pNextSprite);

	return true;
}

void ScrollLabel::setIndex(int index, float duration, bool down)
{
	bool bChange = false;
	bool ret = false;

	if (m_nTargetIndex == m_nCurIndex)
		bChange = true;
	else if (down != m_bDown)
	{
		m_nCurIndex = this->getNexLogicIndex(m_nCurIndex);
		ret = true;
	}

	m_pNextSprite->setVisible(true);

	m_nTargetIndex = index;
	m_bDown = down;
	// 确定临近的精灵的贴图和位置
	int nextLogicInde = this->getNexLogicIndex(m_nCurIndex);
	Point nextPos = this->getNexLogicPos();

	m_bUpdated = true;
	// 确定距离和速度
	int number = 0;
	int temp = m_nCurIndex;
	do
	{
		temp = this->getNexLogicIndex(temp);
		number += 1;

	} while (temp != m_nTargetIndex);

	Size size = this->getContentSize();

	float offsetY = m_pCurSprite->getPositionY() - size.height / 2.f;

	float distance = number * size.height - offsetY;

	m_speed = distance / duration;

	if (bChange)
	{
		m_pNextSprite->setPosition(nextPos + Point(0, offsetY));
		m_pNextSprite->setSpriteFrame(m_sprites.at(nextLogicInde)->getSpriteFrame());
	}
	if (ret)
	{
		auto temp = m_pCurSprite;
		m_pCurSprite = m_pNextSprite;
		m_pNextSprite = temp;
	}
}

void ScrollLabel::visit()
{
	Size size = this->getContentSize();
	auto pos = this->convertToWorldSpace(Point::ZERO);

	SDL_Rect rect = {(int)pos.x, (int)pos.y, (int)size.width, (int)size.height};

	Renderer *renderer = Director::getInstance()->getRenderer();

	renderer->setClipRect(&rect);

	Node::visit();

	renderer->setClipRect(nullptr);
}

void ScrollLabel::update(float dt)
{
	if (!m_bUpdated)
		return;
	auto size = this->getContentSize();
	auto pos = m_pCurSprite->getPosition();

	float speed = m_speed * dt;

	if (m_bDown)
	{
		m_pCurSprite->setPosition(pos + Point(0, speed));
		m_pNextSprite->setPosition(m_pNextSprite->getPosition() + Point(0, speed));

		// 到达下一索引
		if (pos.y >= size.height / 2 * 3)
		{
			m_nCurIndex = this->getNexLogicIndex(m_nCurIndex);

			m_pCurSprite->setSpriteFrame(m_pNextSprite->getSpriteFrame());
			m_pCurSprite->setPosition(size.width / 2, size.height / 2);
			// 确定临近的精灵的贴图和位置
			int nextLogicIndex = this->getNexLogicIndex(m_nCurIndex);

			m_pNextSprite->setSpriteFrame(m_sprites.at(nextLogicIndex)->getSpriteFrame());
			m_pNextSprite->setPosition(this->getNexLogicPos());

			if (m_nCurIndex == m_nTargetIndex)
				m_bUpdated = false;
		}
	}
	else
	{
		m_pCurSprite->setPosition(pos - Point(0, speed));
		m_pNextSprite->setPosition(m_pNextSprite->getPosition() - Point(0, speed));

		// 到达下一索引
		if (pos.y <= -size.height / 2)
		{
			m_nCurIndex = this->getNexLogicIndex(m_nCurIndex);

			m_pCurSprite->setSpriteFrame(m_pNextSprite->getSpriteFrame());
			m_pCurSprite->setPosition(size.width / 2, size.height / 2);
			// 确定临近的精灵的贴图和位置
			int nextLogicIndex = this->getNexLogicIndex(m_nCurIndex);

			m_pNextSprite->setSpriteFrame(m_sprites.at(nextLogicIndex)->getSpriteFrame());
			m_pNextSprite->setPosition(this->getNexLogicPos());

			if (m_nCurIndex == m_nTargetIndex)
				m_bUpdated = false;
		}
	}
}

int ScrollLabel::getNexLogicIndex(int index)
{
	int size = m_sprites.size();

	if (m_bDown)
	{
		if (index < size - 1)
			return index + 1;
		else
			return 0;
	}
	else if (!m_bDown)
	{
		if (index <= 0)
			return size - 1;
		else
			return index - 1;
	}
	return 0;
}

Point ScrollLabel::getNexLogicPos()
{
	Size size = this->getContentSize();

	if (!m_bDown)
		return Point(size.width / 2, size.height / 2 * 3);

	return Point(size.width / 2, -size.height / 2);
}