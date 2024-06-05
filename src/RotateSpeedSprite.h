#ifndef __RotateSpeedSprite_H__
#define __RotateSpeedSprite_H__
#include "SG_Engine.h"
using namespace SDL;
class Fish;

class RotateSpeedSprite:public ActionInterval
{
private:
	FiniteTimeAction*m_pInnerAction;
	float m_speed;
public:
	RotateSpeedSprite();
	~RotateSpeedSprite();
	static RotateSpeedSprite*create(FiniteTimeAction*action);
	bool init(FiniteTimeAction*action);

	virtual void startWithTarget(Node*target);
	virtual bool isDone()const;

	virtual void step(float dt);
	virtual void update(float time);
	virtual void stop();

	void setSpeed(float speed);
	float getSpeed()const;
};
#endif