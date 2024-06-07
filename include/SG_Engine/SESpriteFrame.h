#ifndef __SDL_SpriteFrame_H__
#define __SDL_SpriteFrame_H__
#include <string>

#include "SG_Engine/SEObject.h"
#include "SG_Engine/SEPoint.h"
#include "SG_Engine/SESize.h"
#include "SG_Engine/SERect.h"

NS_SDL_BEGIN
class Texture;

class SpriteFrame : public Object, public Clonable
{
protected:
	Point _offsetPosition;

	Size _originalSize;

	bool _rectRotated;

	Rect _rect;

	Rect _texturePackerRect;

	Texture *_texture;

	std::string _textureFilename;

	Point _anchorPoint;

public:
	SpriteFrame();
	virtual ~SpriteFrame();

	static SpriteFrame *create(const std::string &filename, const Rect &rect);
	static SpriteFrame *create(const std::string &filename, const Rect &Rect, bool rotated, const Point &offset, const Size &originalSize, const Point &anchorPoint);

	static SpriteFrame *createWithTexture(Texture *texture, const Rect &rect);
	static SpriteFrame *createWithTexture(Texture *texture, const Rect &Rect, bool rotated, const Point &offset, const Size &originalSize, const Point &anchorPoint);

	bool initWithTextureFilename(const std::string &filename, const Rect &rect, bool rotated, const Point &offset, const Size &originalSize, const Point &anchorPoint);
	bool initWithTexture(Texture *texture, const Rect &Rect, bool rotated, const Point &offset, const Size &originalSize, const Point &anchorPoint);

	bool isRotated() const
	{
		return _rectRotated;
	}
	void setRotated(bool rotate)
	{
		_rectRotated = rotate;
	}
	Rect getRect() const
	{
		return _rect;
	}
	void setRect(const Rect &rect)
	{
		_rect = rect;
	}
	Point getOffsetPosition() const
	{
		return _offsetPosition;
	}
	void setOffsetPosition(const Point &offset)
	{
		_offsetPosition = offset;
	}
	Size getOriginalSize() const
	{
		return _originalSize;
	}
	void setOriginalSize(const Size &size)
	{
		_originalSize = size;
	}
	std::string getTextureFilename() const
	{
		return _textureFilename;
	}
	void setTextureFilename(const std::string &filename)
	{
		_textureFilename = filename;
	}
	void setTexturePackerRect(const Rect &rect)
	{
		_texturePackerRect = rect;
	}
	Rect getTexturePackerRect() const
	{
		return _texturePackerRect;
	}
	Point getAnchorPoint() const
	{
		return _anchorPoint;
	}

	void setAnchorPoint(const Point &anchorPoint)
	{
		_anchorPoint = anchorPoint;
	}

	Texture *getTexture() const;
	void setTexture(Texture *texture);

	SpriteFrame *clone() const;
};
NS_SDL_END
#endif
