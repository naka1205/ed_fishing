#ifndef __SDL_Surface_H__
#define __SDL_Surface_H__
#include <string>

#include "SDL2/SDL.h"
#include "SDL_image.h"

#include "SG_Engine/SEObject.h"
NS_SDL_BEGIN
class Surface : public Object
{
protected:
	SDL_Surface *_surface;
	SDL_Rect _rect;

public:
	Surface();
	virtual ~Surface();

	static Surface *create(SDL_Surface *surface);
	static Surface *create(const std::string &filepath);
	static Surface *createWithBMP(const std::string &filepath);

	static Surface *createWithRGBA(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);

	bool init(SDL_Surface *sur);
	bool init(const std::string &filepath);
	bool initWithBMP(const std::string &filepath);
	bool initWithRGBA(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);

	int blit(const SDL_Rect *srcRect, Surface *dest, SDL_Rect *destRect);

	Surface *convert(const SDL_PixelFormat *fmt, Uint32 flags);

	int fillRect(const SDL_Rect *rect, Uint32 color);

	Uint32 mapRGB(Uint8 r, Uint8 g, Uint8 b) const;

	Uint32 mapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const;

	int setColorKey(Uint32 colorkey) const;

	Uint32 getARGB(int x, int y) const;

	int setBlendMode(SDL_BlendMode blendMode);
	int getBlendMode(SDL_BlendMode *blendMode);

	int savePNG(const char *filename);

	SDL_Surface *getSDL_Surface() const;
	SDL_Rect getClipRect() const;

	friend class Renderer;
};
NS_SDL_END
#endif
