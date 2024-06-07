#ifndef __SDL_EventListenerTouchOneByOne_H__
#define __SDL_EventListenerTouchOneByOne_H__
#include <functional>
#include "SEEventListener.h"
NS_SDL_BEGIN
class Touch;
class EventListenerTouchOneByOne : public EventListener
{
private:
	bool _bTouchBegan;

public:
	static const std::string LISTENER_ID;

public:
	EventListenerTouchOneByOne();
	virtual ~EventListenerTouchOneByOne();
	static EventListenerTouchOneByOne *create();
	bool init();

public:
	typedef std::function<bool(Touch *, SDL_Event *event)> ccTouchBeganCallback;
	typedef std::function<void(Touch *, SDL_Event *event)> ccTouchCallback;

	ccTouchBeganCallback onTouchBegan;
	ccTouchCallback onTouchMoved;
	ccTouchCallback onTouchEnded;
	ccTouchCallback onTouchCancelled;

public:
	bool isTouchBegan() const;
	void setTouchBegan(bool touchBegan);

	virtual bool checkAvailable() const;
	EventListener *clone();
};
NS_SDL_END
#endif
