#include "SG_Engine/SELayerColor.h"
#include "SG_Engine/SEDirector.h"
#include "SG_Engine/SESprite.h"
#include "SG_Engine/SETexture.h"
NS_SDL_BEGIN
LayerColor::LayerColor()
	: _sprite(nullptr), _dirty(true)
{
}
LayerColor::~LayerColor()
{
}
LayerColor *LayerColor::create(const Color4B &color)
{
	auto layer = new LayerColor();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	if (layer && layer->init(color, 0, 0, visibleSize.width, visibleSize.height))
		layer->autorelease();
	else
		SDL_SAFE_DELETE(layer);
	return layer;
}
LayerColor *LayerColor::create(const Color4B &color, float width, float height)
{
	auto layer = new LayerColor();
	if (layer && layer->init(color, 0, 0, width, height))
		layer->autorelease();
	else
		SDL_SAFE_DELETE(layer);
	return layer;
}
LayerColor *LayerColor::create(const Color4B &color, const Rect &rect)
{
	auto layer = new LayerColor();
	if (layer && layer->init(color, rect.origin.x, rect.origin.y, rect.size.width, rect.size.height))
		layer->autorelease();
	else
		SDL_SAFE_DELETE(layer);
	return layer;
}
LayerColor *LayerColor::create(const Color4B &color, float x, float y, float w, float h)
{
	auto layer = new LayerColor();
	if (layer && layer->init(color, x, y, w, h))
		layer->autorelease();
	else
		SDL_SAFE_DELETE(layer);
	return layer;
}
bool LayerColor::init(const Color4B &color, float x, float y, float w, float h)
{
	_color = color;
	_rect = Rect(x, y, w, h);

	this->setImage(color, x, y, w, h);
	return true;
}
void LayerColor::setColor(const Color4B &color)
{
	if (color.equals(_color))
		return;
	_color = color;

	this->setImage(color, _rect.origin.x, _rect.origin.y, _rect.size.width, _rect.size.height);
}
Color4B LayerColor::getColor() const
{
	return _color;
}

Sprite *LayerColor::getInnerSprite() const
{
	return _sprite;
}

void LayerColor::setImage(const Color4B &color, float x, float y, float width, float height)
{
	Texture *texture = nullptr;

	if (_dirty)
	{
		_dirty = false;

		if (_sprite)
			_sprite->removeFromParent();

		_sprite = Sprite::createWithTexture(SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
		this->addChild(_sprite);

		texture = _sprite->getTexture();
		texture->setBlendMode(SDL_BLENDMODE_BLEND);

		_sprite->setAnchorPoint(Point::ZERO);
	}
	else
		texture = _sprite->getTexture();

	_sprite->setPosition(x, y);

	auto renderer = Director::getInstance()->getRenderer();

	renderer->setTarget(texture);

	SDL_BlendMode oldBlendMode;

	renderer->getDrawBlendMode(&oldBlendMode);

	renderer->setDrawBlendMode(SDL_BLENDMODE_NONE);

	renderer->clear();

	auto oldColor = renderer->getDrawColor();
	renderer->setDrawColor(color);
	renderer->fillRect(Rect(0, 0, width, height));

	renderer->setDrawColor(oldColor);
	renderer->setDrawBlendMode(oldBlendMode);
	renderer->setTarget(nullptr);
}
NS_SDL_END
