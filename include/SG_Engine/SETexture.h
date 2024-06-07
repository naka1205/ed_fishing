#ifndef __SDL_Texture_H__
#define __SDL_Texture_H__

#include "SDL2/SDL.h"
#include "SG_Engine/SEObject.h"
NS_SDL_BEGIN

class Renderer;
class Surface;

class Texture : public Object
{
private:
	SDL_Texture *_texture;

public:
	Texture();
	~Texture();

	static Texture *create(SDL_Texture *tex);

	static Texture *create(Renderer *renderer, const std::string &filename);

	static Texture *create(Renderer *renderer, Uint32 format, int access, int w, int h);

	static Texture *createWithSurface(Renderer *renderer, Surface *surface);

	bool init(SDL_Texture *tex);
	bool init(Renderer *renderer, const std::string &filename);
	bool init(Renderer *renderer, Uint32 format, int access, int w, int h);
	bool initWithSurface(Renderer *renderer, Surface *surface);

	void query(Uint32 *format, int *access, int *w, int *h);

	int setColorMod(Uint8 r, Uint8 g, Uint8 b) const;
	int getColorMod(Uint8 *r, Uint8 *g, Uint8 *b) const;

	int setAlphaMod(Uint8 alpha) const;
	int getAlphaMod(Uint8 *alpha) const;

	int setBlendMode(SDL_BlendMode blendMode) const;
	int getBlendMode(SDL_BlendMode *blendMode) const;

	SDL_Texture *getSDL_Texture() const;

	friend class Renderer;
};
NS_SDL_END
#endif
