#ifndef __SDL_EventDispatcher_H__
#define __SDL_EventDispatcher_H__
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

#include "SDL2/SDL.h"
#include "SEObject.h"
#include "SEEventCustom.h"

NS_SDL_BEGIN
class Node;
class Touch;
class EventListener;
class EventListenerCustom;

class EventDispatcher : public Object
{
private:
	std::map<std::string, std::vector<EventListener *> *> _listenerMap;
	std::unordered_map<Node *, std::vector<EventListener *> *> _nodeListenersMap;

	std::vector<EventListener *> _toAddedListeners;

	std::vector<EventListener *> _toRemovedListeners;

	bool _inDispatch;

	bool _reorderListenersDirty;

public:
	EventDispatcher();
	~EventDispatcher();

public:
	void addEventListener(EventListener *listener, Node *node);

	void forceAddEventListener(EventListener *listener, Node *node);

	void addEventCustomListener(const std::string &eventName, const std::function<void(EventCustom *)> &onEventCustom, Node *node);

	void removeEventListener(EventListener *listener);

	void removeEventListenerForTarget(Node *node, bool recursive = false);

	void resumeEventListenersForTarget(Node *node, bool recursive = false);

	void pauseEventListenersForTarget(Node *node, bool recursive = false);

	void dispatchKeyboardEvent(SDL_Event &event);

	void dispatchKeyboardStateEvent(SDL_Event &event);

	void dispatchMouseEvent(SDL_Event &event);

	void dispatchTouchEvent(const std::vector<Touch *> &touches, SDL_Event &event);

	void dispatchTextInputEvent(SDL_Event &event);

	void dispatchCustomEvent(const std::string &eventName, void *userData = nullptr);

	void dispatchCustomEvent(EventCustom *eventCustom);

	void updateListeners(const std::string &listenerID);

	bool isInDispatch() const { return _inDispatch; };
	void setInDispatch(bool var) { _inDispatch = var; }

private:
	void dispatchEventToTouchOneByOne(const std::vector<Touch *> &touches, SDL_Event *event);

	void dispatchEventToTouchAllAtOnce(const std::vector<Touch *> &touches, SDL_Event *event);

	std::vector<EventListener *> *getListeners(const std::string &listenerID);

	void sortEventListeners();

	static bool sorted(EventListener *e1, EventListener *e2);

	void cleanToRemovedListeners();

	void associateNodeAndEventListener(Node *node, EventListener *listener);

	void dissociateNodeAndEventListener(Node *node, EventListener *listener);

	friend class EventListener;
};
NS_SDL_END
#endif
