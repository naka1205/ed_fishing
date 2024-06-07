#ifndef __FishingNet_H__
#define __FishingNet_H__
#include "B2Entity.h"
#include "Cannon.h"

// 捕鱼的具体类型
enum class FishingType : int
{
	None,
	FishingNet,
	Pearl,
	Fog,
	Laser = 11 // 激光炮塔
};

class FishingEntity : public B2Entity
{
protected:
	FishingType m_fishingType;

public:
	FishingEntity();
	virtual float showAt() = 0;
	virtual void collided() = 0;

	FishingType getFishingType() const;
	void setFishingType(FishingType fishingType);
};

class FishingNet : public FishingEntity
{
	SDL_SYNTHESIZE(int, m_nLv, Level); // 当前等级
public:
	FishingNet();
	~FishingNet();
	CREATE_FUNC(FishingNet);
	static FishingNet *create(int lv);
	bool init();
	bool initWithLv(int lv);

public:
	virtual float showAt();
	virtual void collided();
};

class Laser : public FishingEntity
{
public:
	CREATE_FUNC(Laser);
	bool init();

	virtual float showAt();
	virtual void collided();
};
#endif