#include "SG_Engine/SEObject.h"
#include "SG_Engine/SEPoolManager.h"

NS_SDL_BEGIN
Object::Object()
	: _uniqueID(0)
{

	static unsigned int uObjectCount = 0;
	uObjectCount++;

	_referenceCount = 1;
	_managed = false;

	_uniqueID = uObjectCount;
}

bool Object::init()
{
	return true;
}

Object::~Object()
{

	if (_managed)
	{
		PoolManager::getInstance()->removeObject(this);
	}
}

void Object::retain()
{
	SDLASSERT(_referenceCount > 0, "引用数不能小于0");
	++_referenceCount;
}

void Object::release()
{
	SDLASSERT(_referenceCount > 0, "引用数不能小于0");
	--_referenceCount;

	if (_referenceCount == 0)
		delete this;
}

Object *Object::autorelease()
{

	if (_managed == false)
	{

		PoolManager::getInstance()->addObject(this);
		_managed = true;
	}
	return this;
}

NS_SDL_END
