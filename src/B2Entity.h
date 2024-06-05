
#ifndef __B2Entity_H__
#define __B2Entity_H__
#include "Entity.h"
#include "box2dx.h"

class B2Entity : public Entity
{

	SDL_BOOL_SYNTHESIZE(m_bCollided, Collided);

protected:
	b2Body *m_pBody;
	bool m_bActive; // 是否存活，如果是则存在box2d碰撞面积
public:
	B2Entity();
	~B2Entity();

	void initBody();

	void addFixturesToBody(const std::string &shape);
	// 清除原来body的全部fixture
	void clearFixtures();
	// 获取当前形状
	b2Fixture *getFixtureList() const;

	void setActive(bool active);
	bool isActive() const;
};
#endif