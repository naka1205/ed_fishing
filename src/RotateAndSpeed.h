#ifndef __RotateWithAction_H__
#define __RotateWithAction_H__

#include "SG_Engine.h"

using namespace SDL;
class Fish;

class RotateAndSpeed:public ActionInterval
{
private:
	FiniteTimeAction*m_pInnerAction;
	Fish*m_pTarget;
public:
	RotateAndSpeed();
	~RotateAndSpeed();
	static RotateAndSpeed*create(FiniteTimeAction*action);
	bool init(FiniteTimeAction*action);

	virtual void startWithTarget(Node*target);
	virtual bool isDone()const;

	virtual void step(float dt);
	virtual void update(float time);
	virtual void stop();
};
#endif