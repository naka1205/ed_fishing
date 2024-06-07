#ifndef __SDL_PoolManager_H__
#define __SDL_PoolManager_H__

#include <stack>

#include "SG_Engine/SEAutoreleasePool.h"

NS_SDL_BEGIN
class PoolManager : public Object
{
private:
	static PoolManager *_pInstance;
	AutoreleasePool *_releasePool;

private:
	PoolManager();
	~PoolManager();

public:
	static PoolManager *getInstance();

	static void purge();
	AutoreleasePool *getCurReleasePool();
	void addObject(Object *pObject);
	void removeObject(Object *pObject);
};
NS_SDL_END
#endif
