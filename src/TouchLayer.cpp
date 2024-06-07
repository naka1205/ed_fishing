#include "TouchLayer.h"
TouchLayer::TouchLayer()
	:m_pDelegate(nullptr),m_pListener(nullptr)
{
}
TouchLayer::~TouchLayer()
{
	//注销监听器
	_eventDispatcher->removeEventListenerForTarget(this);
}
bool TouchLayer::init()
{
	m_pListener = EventListenerTouchOneByOne::create();

	m_pListener->onTouchBegan = SDL_CALLBACK_2(TouchLayer::onTouchBegan,this);
	m_pListener->onTouchMoved = SDL_CALLBACK_2(TouchLayer::onTouchMoved,this);
	m_pListener->onTouchEnded = SDL_CALLBACK_2(TouchLayer::onTouchEnded,this);
	m_pListener->setPriority(10);

	_eventDispatcher->addEventListener(m_pListener,this);
	return true;
}
void TouchLayer::setDelegate(TouchLayerDelegate*pDelegate)
{
	m_pDelegate = pDelegate;
}

void TouchLayer::setTouchEnable(bool enable)
{
	m_pListener->setPaused(!enable);
}

bool TouchLayer::onTouchBegan(Touch*touch,SDL_Event*event)
{
	// Point target = touch->getLocation();
	// Rect rect=Rect(0,288,600,72);
	// //如果点击海面，传到CannonLayer
	// if(!rect.containsPoint(target))
	// 	m_pDelegate->cannonAimAt(target);
	// //如果点击的不是海面，传到BulletChooseLayer
	// else
	// 	m_pDelegate->chooseBullet(target);
	// return false;
	m_pDelegate->onTouchBegan(touch,event);
	return true;
}
void TouchLayer::onTouchMoved(Touch*touch,SDL_Event*event)
{
	m_pDelegate->onTouchMoved(touch,event);
}
void TouchLayer::onTouchEnded(Touch*touch,SDL_Event*event)
{
	m_pDelegate->onTouchEnded(touch,event);
}