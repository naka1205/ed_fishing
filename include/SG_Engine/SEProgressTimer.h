#ifndef __SDL_ProgressTimer_H__
#define __SDL_ProgressTimer_H__

#include "SG_Engine/SENode.h"
#include "SG_Engine/SEPoint.h"
#include "SG_Engine/extensions/SDL_gfx_mod.h"

NS_SDL_BEGIN
class Sprite;

class ProgressTimer : public Node
{
public:
	enum class Type
	{
		RADIAL,
		BAR
	};

private:
	float _percentage;
	Type _type;
	Sprite *_sprite; // 起辅助作用
	Sprite *_bg;	 // 显示的主要是这个
	Point _midpoint;
	Point _barChangeRate;

	bool _reverseDirection;

public:
	ProgressTimer();
	~ProgressTimer();

	static ProgressTimer *create(Sprite *sprite);
	bool initWithSprite(Sprite *sprite);
	// 设置分量
	inline float getPercentage() const { return _percentage; }
	void setPercentage(float percentage);
	// 设置类型
	inline Type getType() const { return _type; }
	void setType(Type type);
	// 设置精灵
	Sprite *getSprite() const { return _sprite; }
	void setSprite(Sprite *sprite);
	// 设置mid point
	Point getMidpoint() const { return _midpoint; }
	void setMidpoint(const Point &midpoint);
	//
	Point getBarChangeRate() const { return _barChangeRate; }
	void setBarChangeRate(const Point &barChangeRate);
	// override
	void setRotation(float rotation);

private:
	void updateBar();
	void updateRadial();
	Point boundaryTexCoord(int index);
	void texturedPolygon(Sint16 *vx, Sint16 *vy, int n);
	Point textureCoordFromAlphaPoint(Point alpha);
};
NS_SDL_END
#endif
