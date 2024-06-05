#include "SG_Engine/SENotificationCenter.h"
NS_SDL_BEGIN
NotificationCenter *NotificationCenter::_pInstance = NULL;
NotificationCenter::NotificationCenter()
{
}
NotificationCenter::~NotificationCenter()
{
	_observers.clear();
}
void NotificationCenter::addObserver(Object *target, SEL_CallFuncO selector, const std::string &msgName)
{

	for (auto iter = _observers.begin(); iter != _observers.end(); iter++)
	{
		auto observer = *iter;
		if (observer->name == msgName && observer->target == target && observer->selector == selector)
			return;
	}

	auto observer = new NotificationObserver(target, selector, msgName);
	observer->autorelease();
	_observers.push_back(observer);
}
void NotificationCenter::removeObserver(Object *target, const std::string &msgName)
{

	auto iter = std::find_if(_observers.begin(), _observers.end(), [&](NotificationObserver *o1)
							 { return o1->name == msgName && o1->target == target; });

	_observers.erase(iter);
}
void NotificationCenter::postNotification(const std::string &msgName, Object *data)
{
	for (unsigned int i = 0; i < _observers.size(); i++)
	{
		auto observer = _observers.at(i);
		auto target = observer->target;
		auto selector = observer->selector;
		if (observer->name == msgName)
			(target->*selector)(data);
	}
}
void NotificationCenter::purge()
{
	SDL_SAFE_DELETE(_pInstance);
}
NS_SDL_END
