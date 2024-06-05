#include "SG_Engine/SELabelTTF.h"
#include "SG_Engine/SESprite.h"
#include "SG_Engine/SEFont.h"
#include "SG_Engine/SEDirector.h"
#include "SG_Engine/SERenderer.h"
#include "SG_Engine/SEStringUtils.h"
#include "SG_Engine/SEFontCache.h"

NS_SDL_BEGIN

LabelTTF::LabelTTF()
	: _size(0), _wrapLine(0), _sprite(nullptr)
{
	_cascadeOpacity = true;
}

LabelTTF::~LabelTTF()
{
}

LabelTTF *LabelTTF::create(const std::string &text, int size, const Color4B &color, const std::string &ttfPath)
{
	auto label = new LabelTTF();

	if (label && label->init(text, size, color, ttfPath))
		label->autorelease();
	else
		SDL_SAFE_DELETE(label);

	return label;
}

LabelTTF *LabelTTF::createWithWrapLine(const std::string &text, Uint32 wrapLine, int size, const Color4B &color, const std::string &ttfPath)
{
	auto label = new LabelTTF();

	if (label && label->initWithWrapLine(text, wrapLine, size, color, ttfPath))
		label->autorelease();
	else
		SDL_SAFE_DELETE(label);

	return label;
}

bool LabelTTF::init(const std::string &text, int size, const Color4B &color, const std::string &ttfPath)
{
	_ttfPath = ttfPath;
	_size = size;
	_color = color;

	this->setString(text);
	this->visit();

	return true;
}

bool LabelTTF::initWithWrapLine(const std::string &text, Uint32 wrapLine, int size, const Color4B &color, const std::string &ttfPath)
{
	_text = text;
	_ttfPath = ttfPath;
	_size = size;
	_color = color;

	this->setWrapLine(wrapLine);
	this->visit();

	return true;
}

void LabelTTF::setWrapLine(Uint32 wrapLine)
{
	if (_wrapLine == wrapLine)
		return;

	_wrapLine = wrapLine;
	this->updateInnerSprite();
}

void LabelTTF::setColor(const Color4B &color)
{
	if (_color != color)
	{
		_color = color;
		this->updateInnerSprite();
	}
}

Color4B LabelTTF::getColor() const
{
	return _color;
}

void LabelTTF::setString(const std::string &text)
{
	if (text == _text)
		return;

	_text = text;
	this->updateInnerSprite();
}

std::string LabelTTF::getString() const
{
	return _text;
}

void LabelTTF::updateInnerSprite()
{
	if (strlen(_text.c_str()) == 0)
	{
		if (_sprite != nullptr)
			_sprite->setVisible(false);
		return;
	}
	else if (_sprite != nullptr)
	{
		_sprite->setVisible(true);
	}

	Font *font = FontCache::getInstance()->getFont(_ttfPath, _size);
	Surface *surface = nullptr;

	if (_wrapLine <= 0)
	{
		surface = font->ttfRenderUTF8_Solid(_text.c_str(), _color);
	}
	else
	{
		surface = font->ttfRenderUTF8_Blended_Wrapped(_text.c_str(), _color, _wrapLine);
	}
	if (_sprite == nullptr)
	{
		_sprite = Sprite::createWithSurface(surface);
		this->addChild(_sprite);
	}
	else
	{
		auto renderer = Director::getInstance()->getRenderer();
		auto texture = Texture::createWithSurface(renderer, surface);

		_sprite->setTexture(texture);
	}

	// surface->savePNG(StringUtils::format("%d.png", _uniqueID).c_str());

	Size size = _sprite->getContentSize();
	this->setContentSize(size);
	_sprite->setPosition(size.width / 2, size.height / 2);
}

NS_SDL_END
