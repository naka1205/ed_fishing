#include "SG_Engine/SESpriteFrame.h"
#include "SG_Engine/SETexture.h"
#include "SG_Engine/SEDirector.h"
#include "SG_Engine/SETextureCache.h"

NS_SDL_BEGIN

SpriteFrame::SpriteFrame()
	: _offsetPosition(Point::ZERO), _originalSize(Size::ZERO), _rectRotated(false), _rect(Rect::ZERO), _texture(nullptr), _textureFilename("")
{
}
SpriteFrame::~SpriteFrame()
{
	SDL_SAFE_RELEASE(_texture);
}
SpriteFrame *SpriteFrame::create(const std::string &filename, const Rect &rect)
{
	return SpriteFrame::create(filename, rect, false, Point::ZERO, rect.size, Point(0.5f, 0.5f));
}
SpriteFrame *SpriteFrame::create(const std::string &filename, const Rect &rect, bool rotated, const Point &offset, const Size &originalSize, const Point &anchorPoint)
{
	auto spriteFrame = new SpriteFrame();

	spriteFrame->initWithTextureFilename(filename, rect, rotated, offset, originalSize, anchorPoint);
	spriteFrame->autorelease();

	return spriteFrame;
}
SpriteFrame *SpriteFrame::createWithTexture(Texture *texture, const Rect &rect)
{
	return SpriteFrame::createWithTexture(texture, rect, false, Point::ZERO, rect.size, Point(0.5f, 0.5f));
}
SpriteFrame *SpriteFrame::createWithTexture(Texture *texture, const Rect &rect, bool rotated, const Point &offset, const Size &originalSize, const Point &anchorPoint)
{
	auto spriteFrame = new SpriteFrame();
	if (spriteFrame && spriteFrame->initWithTexture(texture, rect, rotated, offset, originalSize, anchorPoint))
		spriteFrame->autorelease();
	else
		SDL_SAFE_DELETE(spriteFrame);
	return spriteFrame;
}
bool SpriteFrame::initWithTextureFilename(const std::string &filename, const Rect &rect, bool rotated, const Point &offset, const Size &originalSize, const Point &anchorPoint)
{
	_textureFilename = filename;
	_rect = rect;
	_rectRotated = rotated;
	_offsetPosition = offset;
	_originalSize = originalSize;

	_texture = nullptr;
	_anchorPoint = anchorPoint;
	// SpriteFrameCache*frameCache = TheDirector::getInstance()->getSpriteFrameCache();
	setTexture(Director::getInstance()->getTextureCache()->addImage(filename));
	return true;
}
bool SpriteFrame::initWithTexture(Texture *texture, const Rect &rect, bool rotated, const Point &offset, const Size &originalSize, const Point &anchorPoint)
{
	setTexture(texture);

	_rect = rect;
	_rectRotated = rotated;
	_offsetPosition = offset;
	_originalSize = originalSize;
	_anchorPoint = anchorPoint;

	return true;
}
Texture *SpriteFrame::getTexture() const
{
	if (_texture)
	{
		return _texture;
	}

	return nullptr;
}
void SpriteFrame::setTexture(Texture *texture)
{
	if (_texture != texture)
	{
		SDL_SAFE_RELEASE(_texture);
		_texture = texture;
		_texture->retain();
	}
}
SpriteFrame *SpriteFrame::clone() const
{

	return nullptr;
}
NS_SDL_END
