#ifndef __Fog_H__
#define __Fog_H__
#include<string>
#include "FishingNet.h"
using namespace std;
class Fish;

class Fog:public FishingEntity
{
private:
	Fish*m_pFish;//ÃÔÎíËùÒÀ¸½µÄ¸ÕÌå
public:
	Fog();
	~Fog();
	static Fog*create(Fish*fish);
	bool init(Fish*fish);

	virtual void update(float dt);
	virtual float showAt();
	virtual void collided();
private:
	void fishDeadCallback(EventCustom*eventCustom);
};

#endif