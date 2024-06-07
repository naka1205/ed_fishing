#ifndef __Pearl_H__
#define __Pearl_H__
#include "FishingNet.h"

class Pearl:public FishingEntity
{
private:
	Point m_delta;
	float m_speed;
public:
	Pearl();
	~Pearl();

	CREATE_FUNC(Pearl);
	bool init();

	void setDelat(float rotation);

	virtual float showAt();
	virtual void collided();
	virtual void update(float dt);
};
#endif