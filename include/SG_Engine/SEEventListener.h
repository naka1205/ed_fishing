#ifndef __SDL_EventListener_H__
#define __SDL_EventListener_H__
#include <string>

#include "SEObject.h"
#include "SENode.h"
#include "SEEventDispatcher.h"
#include "SEPlatformMarcos.h"
#include "SEDirector.h"
NS_SDL_BEGIN
class EventListener : public Object
{
protected:
	bool _bRegistered;

	bool _paused;

	int _priority;

	Node *_associatedNode;

	bool _swallow;
	std::string _listenerID;

protected:
	EventListener()
			: _bRegistered(false), _paused(true), _priority(0), _associatedNode(nullptr), _swallow(false)
	{
	}

public:
	bool init(const std::string &id)
	{
		_listenerID = id;
		return true;
	}
	virtual ~EventListener() {}
	virtual bool checkAvailable() const = 0;

	virtual EventListener *clone() = 0;

public:
	void setRegistered(bool registered)
	{
		_bRegistered = registered;
	}
	bool isRegistered() const
	{
		return _bRegistered;
	}
	int getPriority()
	{
		return _priority;
	}
	void setPriority(int priority)
	{
		if (_priority != priority)
		{
			_priority = priority;
			Director::getInstance()->getEventDispatcher()->_reorderListenersDirty = true;
		}
	}
	void setAssociatedNode(Node *node)
	{
		_associatedNode = node;
	}
	Node *getAssociatedNode() const
	{
		return _associatedNode;
	}
	bool isPaused() const
	{
		return _paused;
	}
	void setPaused(bool pause)
	{
		_paused = pause;
	}
	void setSwallowTouches(bool swallow)
	{
		_swallow = swallow;
	}
	bool isSwallowTouches()
	{
		return _swallow;
	}
	std::string getListenerID() const
	{
		return _listenerID;
	}
};
NS_SDL_END

#endif
