#ifndef __SDL_Layer_H__
#define __SDL_Layer_H__
#include "SDL2/SDL.h"

#include "SG_Engine/SENode.h"
NS_SDL_BEGIN

class Touch;

class Layer : public Node
{
protected:
public:
	Layer();
	virtual ~Layer();

	CREATE_FUNC(Layer);
	bool init();

	virtual bool onTouchBegan(Touch *touch, SDL_Event *event);
	virtual void onTouchMoved(Touch *touch, SDL_Event *event);
	virtual void onTouchEnded(Touch *touch, SDL_Event *event);
	virtual void onTouchCancelled(Touch *touch, SDL_Event *event);

	virtual void onTouchesBegan(std::vector<Touch *> touches, SDL_Event *event);
	virtual void onTouchesMoved(std::vector<Touch *> touches, SDL_Event *event);
	virtual void onTouchesEnded(std::vector<Touch *> touches, SDL_Event *event);
	virtual void onTouchesCancelled(std::vector<Touch *> touches, SDL_Event *event);

	virtual void onKeyPressed(SDL_Keycode keyCode, SDL_Event *event);
	virtual void onKeyReleased(SDL_Keycode keyCode, SDL_Event *event);
};
NS_SDL_END
#endif
