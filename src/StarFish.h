#ifndef __StarFish_H__
#define __StarFish_H__

#include<vector>
#include "Fish.h"
using namespace std;
class StarFishDelegate
{
public:
	virtual ~StarFishDelegate(){}
	virtual void starFishDeadCallback()=0;
};

class StarFish:public Fish
{
private:
	float m_elpased;
	float m_deadTime;//死亡时间
	vector<float> m_durations;
	int m_index;
	bool m_dirty;

	StarFishDelegate*m_pDelegate;
public:
	StarFish();
	~StarFish();
	/*startTime 0~1*/
	static StarFish*create(float startTime);
	bool init(float startTime);
	void dead();//在死亡时调用
	void setDelegate(StarFishDelegate*pDelegate);
public:
	virtual void update(float dt);
	virtual void reset();
	virtual void clear();

	virtual void caught();//被捕捉回调函数
	virtual void collided(TriggingType type);//碰撞回调函数
};
#endif