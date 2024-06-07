#ifndef __Sprite_H__
#define __Sprite_H__
#include <string>

#include "SG_Engine/SEProtocols.h"
#include "SG_Engine/SENode.h"
#include "SG_Engine/SERenderCommand.h"
NS_SDL_BEGIN

class Surface;
class SpriteFrame;

class Sprite : public Node, public TextureProtocol
{
protected:
	Texture *_texture;

	Rect _rect;

	Rect _spriteFrameRect;

	bool _rectRotated;

	Point _offsetPosition;

	Size _originalSize;

	bool _insideBounds;

	TextureCommand _textureCommand;

public:
	Sprite();
	virtual ~Sprite();
	CREATE_FUNC(Sprite);

	static Sprite *create(const std::string &filename);
	static Sprite *create(const std::string &fileName, const Rect &rect);

	static Sprite *createWithTexture(Texture *texture);
	static Sprite *createWithTexture(Texture *texture, const Rect &rect);
	static Sprite *createWithTexture(Texture *texture, const Rect &rect, bool rotated, const Point &offset, const Size &originalSize);

	static Sprite *createWithSpriteFrame(SpriteFrame *spriteFrame);
	static Sprite *createWithSpriteFrameName(const std::string &name);

	static Sprite *createWithSurface(Surface *surface);
	static Sprite *createWithTexture(Uint32 format, int access, int w, int h);

	bool init();

	bool initWithFile(const std::string &filename);
	bool initWithFile(const std::string &filename, const Rect &rect);

	bool initWithTexture(Texture *texture);
	bool initWithTexture(Texture *texture, const Rect &rect);
	bool initWithTexture(Texture *texture, const Rect &rect, bool rotated, const Point &offset, const Size &originalSize);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame);
	bool initWithSpriteFrameName(const std::string &name);

	bool initWithSurface(Surface *surface);
	bool initWithTexture(Uint32 format, int access, int w, int h);

	virtual Texture *getTexture() const;
	virtual void setTexture(Texture *texture);
	void setTexture(const std::string &filename);

	virtual void draw();

	SpriteFrame *getSpriteFrame() const;
	void setSpriteFrame(const std::string &spriteFrameName);
	void setSpriteFrame(SpriteFrame *spriteFrame);

	Rect getSpriteFrameRect() const;

	Rect getTextureRect() const;
	void setTextureRect(const Rect &rect);
	void setTextureRect(const Rect &rect, bool rotated, Size untrimmedSize);

	Sprite *clone() const;
};
NS_SDL_END
#endif
