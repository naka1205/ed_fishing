#ifndef __SDL_Action_H__
#define __SDL_Action_H__

#include "SG_Engine/SEObject.h"

NS_SDL_BEGIN
class Node;
class ActionInterval;

class Action : public Object, public Clonable
{
protected:
	Node *_originalTarget;
	Node *_target;

	int _tag;

public:
	Action();
	virtual ~Action();

	inline Node *getTarget() { return _target; }

	inline void setTarget(Node *pTarget) { _target = pTarget; }

	inline Node *getOriginalTarget() { return _originalTarget; }
	inline void setOriginalTarget(Node *pTarget) { _originalTarget = pTarget; }

	int getTag() const;
	void setTag(int tag);

	virtual bool isDone() const;

	virtual void startWithTarget(Node *pTarget);

	virtual void stop();

	virtual void step(float dt);

	virtual void update(float time);

	virtual Action *clone() const = 0;
};

class Speed : public Action
{
private:
	float _speed;
	ActionInterval *_pInnerAction;

public:
	Speed();
	~Speed();
	static Speed *create(ActionInterval *action, float speed);
	bool initWithAction(ActionInterval *action, float speed);

	float getSpeed() const { return _speed; }
	void setSpeed(float speed) { _speed = speed; }

	virtual bool isDone() const;
	virtual void startWithTarget(Node *pTarget);
	virtual void stop();
	virtual void step(float dt);
	virtual Action *clone() const;
};
NS_SDL_END
#endif
