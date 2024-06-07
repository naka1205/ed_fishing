#ifndef __Window_H__
#define __Window_H__

#include "SDL2/SDL.h"

#include "SG_Engine/SEObject.h"

NS_SDL_BEGIN
class Surface;
class Renderer;
class Size;

class Window : public Object
{
private:
	SDL_Window *_window;

	SDL_Point _position;
	int _width;
	int _height;

public:
	Window();
	~Window();

	static Window *create(const char *title, int x, int y, int width, int height, Uint32 flags);
	bool init(const char *title, int x, int y, int width, int height, Uint32 flags);

	int updateWindowSurface();

	void destroyWindow();

	Surface *getWindowSurface();

	void showWindow();

	void hideWindow();

	Size getWindowSize() const;

	Size getRealWindowSize() const;

	SDL_Point getWindowPosition();

	friend class Renderer;
};
NS_SDL_END
#endif
