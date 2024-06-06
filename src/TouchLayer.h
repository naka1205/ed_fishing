#ifndef __TouchLayer_H__
#define __TouchLayer_H__
#include "SDL2/SDL.h"

#include "SG_Engine.h"
using namespace SDL;
class TouchLayerDelegate
{
public:
	virtual ~TouchLayerDelegate(){}
	virtual bool onTouchBegan(Touch*touch,SDL_Event *event)=0;
	virtual void onTouchMoved(Touch*touch,SDL_Event*event)=0;
	virtual void onTouchEnded(Touch*touch,SDL_Event*event)=0;
};
class TouchLayer:public Layer
{
private:
	TouchLayerDelegate*m_pDelegate;
	EventListenerTouchOneByOne*m_pListener;
public:
	TouchLayer();
	~TouchLayer();
	CREATE_FUNC(TouchLayer);
	bool init();
public:
	void setDelegate(TouchLayerDelegate*pDelegate);
	void setTouchEnable(bool enable);
public:
	virtual bool onTouchBegan(Touch*,SDL_Event*);
	virtual void onTouchMoved(Touch*touch,SDL_Event*event);
	virtual void onTouchEnded(Touch*touch,SDL_Event*event);
};
#endif