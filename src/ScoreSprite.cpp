#include "ScoreSprite.h"
#include "StaticData.h"

ScoreSprite::ScoreSprite()
		: m_pPrizeSign(nullptr), m_pPrizeNum(nullptr)
{
}

ScoreSprite::~ScoreSprite()
{
	m_pPrizeSign = nullptr;
	m_pPrizeNum = nullptr;
}

ScoreSprite *ScoreSprite::create(int score)
{
	auto sprite = new ScoreSprite();

	if (sprite && sprite->init(score))
		sprite->autorelease();
	else
		SDL_SAFE_DELETE(sprite);

	return sprite;
}

bool ScoreSprite::init(int score)
{
	auto prizeSignName = STATIC_DATA_STRING("prizesign_sprite_name");
	m_pPrizeSign = Sprite::createWithSpriteFrameName(prizeSignName);
	this->addChild(m_pPrizeSign);

	auto prizeNumName = STATIC_DATA_STRING("prizenum_sprite_name");
	m_pPrizeNum = LabelAtlas::create("", prizeNumName, 48, 48, '0');
	this->addChild(m_pPrizeNum);
	// 设置数
	this->setScore(score);

	return true;
}

float ScoreSprite::runFadeOutAction()
{
	FadeOut *fade = FadeOut::create(1.f);

	m_pPrizeSign->runAction(fade);
	m_pPrizeNum->getInnerSprite()->runAction(fade->clone());

	return fade->getDuration();
}

void ScoreSprite::setScore(int score)
{
	auto text = to_string(score);

	m_pPrizeNum->setString(text);

	Size signSize = m_pPrizeSign->getContentSize();
	Size numSize = m_pPrizeNum->getContentSize();
	// 调整对应位置
	m_pPrizeSign->setPositionX(-numSize.width / 2);
	m_pPrizeNum->setPositionX(signSize.width / 2);
}