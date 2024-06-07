#ifndef __SDL_EventListenerTextInput_H__
#define __SDL_EventListenerTextInput_H__
#include <string>
#include <functional>

#include "SDL2/SDL.h"
#include "SEEventListener.h"

NS_SDL_BEGIN
class EventListenerTextInput : public EventListener
{
public:
	static std::string LISTENER_ID;

public:
	std::function<void(char[32], SDL_Event *)> onTextInput;

	std::function<void(char *, Sint32, Sint32, SDL_Event *)> onTextEditing;

public:
	EventListenerTextInput();
	~EventListenerTextInput();
	CREATE_FUNC(EventListenerTextInput);
	bool init();

	virtual EventListenerTextInput *clone();
	virtual bool checkAvailable() const;
};
NS_SDL_END
#endif
