#ifndef __SDL_Touch_H__
#define __SDL_Touch_H__

#include "SDL2/SDL.h"

#include "SG_Engine/SEObject.h"
#include "SG_Engine/SEPoint.h"

NS_SDL_BEGIN
typedef enum class TouchStatus
{
	TOUCH_NONE,
	TOUCH_DOWN,
	TOUCH_MOTION,
	TOUCH_UP
} TouchStatus;
class Touch : public Object
{
private:
	SDL_FingerID _id;

	TouchStatus _status;

	bool _startPointCaptured;
	Point _startPoint;
	Point _point;
	Point _prevPoint;

public:
	Touch();
	virtual ~Touch();
	CREATE_FUNC(Touch);
	bool init();

	Point getLocation() const;

	Point getPreviousLocation() const;

	Point getStartLocation() const;

	Point getDelta() const;

	bool isAvailable() const;

	SDL_FingerID getID() const
	{
		return _id;
	}
	TouchStatus getStatus() const
	{
		return _status;
	}

	void setTouchInfo(SDL_FingerID id, float x, float y, TouchStatus status)
	{
		_id = id;
		_prevPoint = _point;
		_point.x = x;
		_point.y = y;
		_status = status;

		if (!_startPointCaptured)
		{
			_startPointCaptured = true;
			_startPoint = _point;
			_prevPoint = _point;
		}

		if (status == TouchStatus::TOUCH_NONE)
		{
			_startPointCaptured = false;
			_startPoint = Point::ZERO;
			_prevPoint = Point::ZERO;
			_point = Point::ZERO;
		}
	}
};
NS_SDL_END
#endif
