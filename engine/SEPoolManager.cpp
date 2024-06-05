#include "SG_Engine/SEPoolManager.h"
NS_SDL_BEGIN
PoolManager *PoolManager::_pInstance = NULL;

PoolManager::PoolManager()
	: _releasePool(NULL)
{
	_releasePool = new AutoreleasePool();
}

PoolManager::~PoolManager()
{

	_releasePool->clear();

	SDL_SAFE_RELEASE(_releasePool);
}

PoolManager *PoolManager::getInstance()
{
	if (_pInstance == NULL)
	{
		_pInstance = new PoolManager();
	}
	return _pInstance;
}

void PoolManager::addObject(Object *pObject)
{
	_releasePool->addObject(pObject);
}

void PoolManager::removeObject(Object *pObject)
{
	SDLASSERT(_releasePool, "Object();");

	_releasePool->removeObject(pObject);
}
AutoreleasePool *PoolManager::getCurReleasePool()
{
	return _releasePool;
}
void PoolManager::purge()
{

	SDL_SAFE_DELETE(_pInstance);
}
NS_SDL_END
