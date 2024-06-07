#ifndef __Renderer_H__
#define __Renderer_H__
#include <vector>
#include <algorithm>

#include "SDL2/SDL.h"
#include "SDL_image.h"

#include "SG_Engine/SEObject.h"
#include "SG_Engine/SEColor.h"
#include "SG_Engine/SERect.h"
#include "SG_Engine/SEPoint.h"

NS_SDL_BEGIN

class Window;
class Surface;
class Texture;
class Rect;
class RenderCommand;

class Renderer : public Object
{
private:
	SDL_Renderer *_renderer;
	Window *_window;

	std::vector<RenderCommand *> _renderQueue;

public:
	Renderer();
	~Renderer();

	static Renderer *create(Window *window, int index, Uint32 flags);

	bool init(Window *win, int index, Uint32 flags);

	int clear();

	void addCommand(RenderCommand *command);

	unsigned int render();

	void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void setDrawColor(const Color4B &color);

	Color4B getDrawColor() const;

	void present();

	int copy(Texture *texture, const SDL_Rect *srcRect, SDL_Rect *destRect);
	int copy(Texture *texture, const Rect &srcRect, const Rect &destRect);

	int copyEx(Texture *texture, const SDL_Rect *srcRect, SDL_Rect *destRect, const double angle, const SDL_Point *center, SDL_RendererFlip flip);
	int copyEx(Texture *texture, const Rect &srcRect, const Rect &destRect, const double angle, const Point &center, SDL_RendererFlip flip);

	int drawLine(const Point &startPos, const Point &endPos);
	int drawRect(const Rect &rect);

	int fillRect(const Rect &rect);

	int setScale(float scaleX, float scaleY);
	Size getScale() const;

	int setLogicalSize(int w, int h);
	void getLogicalSize(int &w, int &h);

	int setDrawBlendMode(SDL_BlendMode mode);
	int getDrawBlendMode(SDL_BlendMode *mode);

	int setViewport(const SDL_Rect *rect);

	Rect getViewport();

	int setTarget(Texture *texture);

	int setClipRect(const SDL_Rect *rect);
	Rect getClipRect();

	bool checkVisibility(const Rect &boundingBox) const;

	::SDL_Renderer *getSDL_Renderer() const;

public:
	static bool commandComparisonLess(RenderCommand *n1, RenderCommand *n2);
};
NS_SDL_END
#endif
