#ifndef __Object_H__
#define __Object_H__

#include "SG_Engine/SEPlatformMarcos.h"

NS_SDL_BEGIN
class AutoreleasePool;

class Clonable
{
public:
	virtual Clonable *clone() const = 0;
	virtual ~Clonable() {}
};

class Object
{
protected:
	unsigned int _referenceCount;

	bool _managed;
	unsigned int _uniqueID;

public:
	CREATE_FUNC(Object);
	bool init();
	Object();
	virtual ~Object();

	void retain();

	void release();

	Object *autorelease();

	unsigned int getReferenceCount() const { return _referenceCount; }

	unsigned int getUniqueID() const { return _uniqueID; }

	bool isManaged() const { return _managed; }

	friend class AutoreleasePool;
};

typedef void (Object::*SEL_CallFunc)(void);
typedef void (Object::*SEL_SCHEDULE)(float);
typedef void (Object::*SEL_CallFuncO)(Object *);

#define callfunc_selector(_SELECTOR) static_cast<SDL::SEL_CallFunc>(&_SELECTOR)
#define schedule_selector(_SELECTOR) static_cast<SDL::SEL_SCHEDULE>(&_SELECTOR)
#define scheduleO_selector(_SELECTOR) static_cast<SDL::SEL_CallFuncO>(&_SELECTOR)
NS_SDL_END
#endif
