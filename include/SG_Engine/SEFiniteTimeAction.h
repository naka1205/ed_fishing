#ifndef __SDL_FiniteTimeAction_H__
#define __SDL_FiniteTimeAction_H__
#include "SG_Engine/SEAction.h"
NS_SDL_BEGIN
class FiniteTimeAction : public Action
{
protected:
	float _duration;

public:
	FiniteTimeAction() : _duration(0.f) {}
	virtual ~FiniteTimeAction() {}

	inline float getDuration() const { return _duration; }
	inline void setDuration(float duration) { _duration = duration; }

	virtual FiniteTimeAction *reverse() const = 0;

	virtual FiniteTimeAction *clone() const = 0;
};
NS_SDL_END
#endif
