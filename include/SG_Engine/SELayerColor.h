#ifndef __SDL_LayerColor_H__
#define __SDL_LayerColor_H__

#include "SG_Engine/SESize.h"
#include "SG_Engine/SEColor.h"
#include "SG_Engine/SELayer.h"

NS_SDL_BEGIN
class Sprite;
class LayerColor : public Layer
{
private:
	Sprite *_sprite;
	Color4B _color;
	Rect _rect;
	bool _dirty;

public:
	LayerColor();
	~LayerColor();
	static LayerColor *create(const Color4B &color);
	static LayerColor *create(const Color4B &color, float width, float height);
	static LayerColor *create(const Color4B &color, const Rect &rect);
	static LayerColor *create(const Color4B &color, float x, float y, float w, float h);

	bool init(const Color4B &color, float x, float y, float width, float height);

	void setColor(const Color4B &color);
	Color4B getColor() const;
	Sprite *getInnerSprite() const;

private:
	void setImage(const Color4B &color, float x, float y, float width, float height);
};
NS_SDL_END
#endif
