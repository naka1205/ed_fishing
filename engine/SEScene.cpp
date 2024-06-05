#include "SG_Engine/SEScene.h"
#include "SG_Engine/SEDirector.h"
NS_SDL_BEGIN
Scene::Scene()
{
}

Scene::~Scene()
{
}

bool Scene::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	setContentSize(visibleSize);

	this->setAnchorPoint(Point(0.f, 0.f));

	return true;
}

NS_SDL_END
