#ifndef __SDL_InputHandler_H__
#define __SDL_InputHandler_H__
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <functional>

#include "SDL2/SDL.h"

#include "SG_Engine/SEObject.h"
#include "SG_Engine/SEPlatformMarcos.h"

NS_SDL_BEGIN

class Node;
class Touch;
class EventListener;

class InputHandler : public Object
{
private:
	std::vector<Touch *> _touches;

public:
	InputHandler();
	~InputHandler();

	void update();

private:
	void mouseBtnDown(SDL_Event event);
	void mouseBtnUp(SDL_Event event);

	void mouseMotion(SDL_Event event);

	void touchBegan(SDL_Event event);
	void touchMoved(SDL_Event event);
	void touchEnded(SDL_Event event);

	Touch *getAvaiableTouch();

	Touch *getTouchByID(SDL_FingerID id);
};
NS_SDL_END
#endif
