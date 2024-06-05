#include "SG_Engine/SELayer.h"
#include "SG_Engine/SEDirector.h"

NS_SDL_BEGIN

Layer::Layer()
{
}

Layer::~Layer()
{
}

bool Layer::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	setContentSize(visibleSize);

	this->setAnchorPoint(Point(0.f, 0.f));

	return true;
}

bool Layer::onTouchBegan(Touch *, SDL_Event *event)
{
	return false;
}

void Layer::onTouchMoved(Touch *touch, SDL_Event *event)
{
}

void Layer::onTouchEnded(Touch *touch, SDL_Event *event)
{
}

void Layer::onTouchCancelled(Touch *touch, SDL_Event *event)
{
}

void Layer::onTouchesBegan(std::vector<Touch *> touches, SDL_Event *event)
{
}

void Layer::onTouchesMoved(std::vector<Touch *> touches, SDL_Event *event)
{
}

void Layer::onTouchesEnded(std::vector<Touch *> touches, SDL_Event *event)
{
}

void Layer::onTouchesCancelled(std::vector<Touch *> touches, SDL_Event *event)
{
}

void Layer::onKeyPressed(SDL_Keycode, SDL_Event *event)
{
}

void Layer::onKeyReleased(SDL_Keycode, SDL_Event *event)
{
}
NS_SDL_END
